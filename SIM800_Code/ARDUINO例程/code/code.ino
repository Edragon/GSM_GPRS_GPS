#include <TimerOne.h>


#define DebugSerial Serial
#define GprsSerail Serial3
#define GpsSerial  Serial2

struct
{
	char GPS_Buffer[80];
	bool isGetData;   //是否获取到GPS数据
	bool isParseData; //是否解析完成
	char UTCTime[11];   //UTC时间
	char latitude[11];    //纬度
	char N_S[2];    //N/S
	char longitude[12];   //经度
	char E_W[2];    //E/W
	bool isUsefull;   //定位信息是否有效
} Save_Data;

const unsigned int gpsRxBufferLength = 600;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int gpsRxCount = 0;

#define Success 1U
#define Failure 0U

int L = 13; //LED指示灯引脚

unsigned long  Time_Cont = 0;       //定时器计数器

const unsigned int gprsRxBufferLength = 600;
char gprsRxBuffer[gprsRxBufferLength];
unsigned int gprsBufferCount = 0;
char OneNetServer[] = "api.heclouds.com";       //不需要修改


char device_id[] = "3225187";    //修改为自己的设备ID
char API_KEY[] = "R9xO5NZm6oVI4YBHvCPKEqtwYtMA";    //修改为自己的API_KEY
char sensor_gps[] = "location";
char sensor_temp[] = "TEMP";

void setup() {
	pinMode(L, OUTPUT);
	digitalWrite(L, LOW);

	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	clrGpsRxBuffer();

	DebugSerial.begin(9600);
	GprsSerail.begin(9600);
	GpsSerial.begin(115200);      //115200，和我们店铺的GPS输出的波特率一致

	Timer1.initialize(1000);
	Timer1.attachInterrupt(Timer1_handler);

	initGprs();

	DebugSerial.println("\r\nsetup end!");
}

void loop() {
	Time_Cont = 0;
	while (Time_Cont < 5000)	//5s内不停读取GPS
	{
		gpsRead();  //获取GPS数据
		parseGpsBuffer();//解析GPS数据		
	}
	printGpsBuffer();//输出解析后的数据  ,包括发送到OneNet服务器
}

void postDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, float data_value)
{
	char send_buf[400] = {0};
	char text[100] = {0};
	char tmp[25] = {0};

	char value_str[15] = {0};


	dtostrf(data_value, 3, 2, value_str); //转换成字符串输出


	//连接服务器
	memset(send_buf, 0, 400);    //清空
	strcpy(send_buf, "AT+CIPSTART=\"TCP\",\"");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\",80\r\n");
	if (sendCommand(send_buf, "CONNECT", 10000, 5) == Success);
	else errorLog(7);

	//发送数据
	if (sendCommand("AT+CIPSEND\r\n", ">", 3000, 1) == Success);
	else errorLog(8);

	memset(send_buf, 0, 400);    //清空

	/*准备JSON串*/
	//ARDUINO平台不支持sprintf的double的打印，只能转换到字符串然后打印
	sprintf(text, "{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%s}]}]}"
	        , sensor_id_temp, value_str);

	/*准备HTTP报头*/
	send_buf[0] = 0;
	strcat(send_buf, "POST /devices/");
	strcat(send_buf, device_id_temp);
	strcat(send_buf, "/datapoints HTTP/1.1\r\n"); //注意后面必须加上\r\n
	strcat(send_buf, "api-key:");
	strcat(send_buf, API_VALUE_temp);
	strcat(send_buf, "\r\n");
	strcat(send_buf, "Host:");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\r\n");
	sprintf(tmp, "Content-Length:%d\r\n\r\n", strlen(text)); //计算JSON串长度
	strcat(send_buf, tmp);
	strcat(send_buf, text);

	if (sendCommand(send_buf, send_buf, 3000, 1) == Success);
	else errorLog(9);

	char sendCom[2] = {0x1A};
	if (sendCommand(sendCom, "\"succ\"}", 3000, 1) == Success);
	else errorLog(10);

	if (sendCommand("AT+CIPCLOSE\r\n", "CLOSE OK", 3000, 1) == Success);
	else errorLog(11);

	if (sendCommand("AT+CIPSHUT\r\n", "SHUT OK", 3000, 1) == Success);
	else errorLog(11);
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;

		DebugSerial.print("Save_Data.UTCTime = ");
		DebugSerial.println(Save_Data.UTCTime);

		if (Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			DebugSerial.print("Save_Data.latitude = ");
			DebugSerial.println(Save_Data.latitude);
			DebugSerial.print("Save_Data.N_S = ");
			DebugSerial.println(Save_Data.N_S);
			DebugSerial.print("Save_Data.longitude = ");
			DebugSerial.println(Save_Data.longitude);
			DebugSerial.print("Save_Data.E_W = ");
			DebugSerial.println(Save_Data.E_W);

			postGpsDataToOneNet(API_KEY, device_id, sensor_gps, Save_Data.longitude, Save_Data.latitude);
		}
		else
		{
			DebugSerial.println("GPS DATA is not usefull!");
		}

	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		DebugSerial.println("**************");
		DebugSerial.println(Save_Data.GPS_Buffer);


		for (int i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(12);  //解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2];
					switch (i)
					{
					case 1: memcpy(Save_Data.UTCTime, subString, subStringNext - subString); break; //获取UTC时间
					case 2: memcpy(usefullBuffer, subString, subStringNext - subString); break; //获取UTC时间
					case 3: memcpy(Save_Data.latitude, subString, subStringNext - subString); break; //获取纬度信息
					case 4: memcpy(Save_Data.N_S, subString, subStringNext - subString); break; //获取N/S
					case 5: memcpy(Save_Data.longitude, subString, subStringNext - subString); break; //获取纬度信息
					case 6: memcpy(Save_Data.E_W, subString, subStringNext - subString); break; //获取E/W

					default: break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if (usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if (usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(13);  //解析错误
				}
			}


		}
	}
}


void gpsRead() {
	while (GpsSerial.available())
	{
		gpsRxBuffer[gpsRxCount] = GpsSerial.read();
		if (gpsRxBuffer[gpsRxCount++] == '\n')
		{
			char* GPS_BufferHead;
			char* GPS_BufferTail;
			if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL )
			{
				if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) && (GPS_BufferTail > GPS_BufferHead))
				{
					memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, GPS_BufferTail - GPS_BufferHead);
					Save_Data.isGetData = true;

					clrGpsRxBuffer();
				}
			}
			clrGpsRxBuffer();
		}
		if (gpsRxCount == gpsRxBufferLength)clrGpsRxBuffer();
	}


}

void clrGpsRxBuffer(void)
{
	memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空
	gpsRxCount = 0;
}

double longitudeToOnenetFormat(char *lon_str_temp)
{
	double lon_temp = 0;
	long lon_Onenet = 0;
	int dd_int = 0;
	long mm_int = 0;
	double lon_Onenet_double = 0;

	lon_temp = atof(lon_str_temp);
	lon_Onenet = lon_temp * 100000; //转换为整数

	dd_int = lon_Onenet / 10000000; //取出dd

	mm_int = lon_Onenet % 10000000; //取出MM部分


	lon_Onenet_double = dd_int + (double)mm_int / 60 / 100000; //换算为Onenet格式


	return lon_Onenet_double;
}

double latitudeToOnenetFormat(char *lat_str_temp)
{
	double lat_temp = 0;
	long lat_Onenet = 0;
	int dd_int = 0;
	long mm_int = 0;

	double lat_Onenet_double = 0;

	lat_temp = atof(lat_str_temp);
	lat_Onenet = lat_temp * 100000; //转换为整数

	dd_int = lat_Onenet / 10000000; //取出dd

	mm_int = lat_Onenet % 10000000; //取出MM部分

	lat_Onenet_double = dd_int + (double)mm_int / 60 / 100000; //换算为Onenet格式


	return lat_Onenet_double;
}

void postGpsDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, char* lon_temp, char* lat_temp)
{
	char send_buf[400] = {0};
	char text[100] = {0};
	char tmp[25] = {0};

	char lon_str_end[15] = {0};
	char lat_str_end[15] = {0};

	dtostrf(longitudeToOnenetFormat(lon_temp), 3, 6, lon_str_end); //转换成字符串输出
	dtostrf(latitudeToOnenetFormat(lat_temp), 2, 6, lat_str_end); //转换成字符串输出

	//连接服务器
	memset(send_buf, 0, 400);    //清空
	strcpy(send_buf, "AT+CIPSTART=\"TCP\",\"");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\",80\r\n");
	if (sendCommand(send_buf, "CONNECT", 10000, 5) == Success);
	else errorLog(7);

	//发送数据
	if (sendCommand("AT+CIPSEND\r\n", ">", 3000, 1) == Success);
	else errorLog(8);

	memset(send_buf, 0, 400);    //清空

	/*准备JSON串*/
	//ARDUINO平台不支持sprintf的double的打印，只能转换到字符串然后打印
	sprintf(text, "{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%s,\"lat\":%s}}]}]}"
	        , sensor_id_temp, lon_str_end, lat_str_end);

	/*准备HTTP报头*/
	send_buf[0] = 0;
	strcat(send_buf, "POST /devices/");
	strcat(send_buf, device_id_temp);
	strcat(send_buf, "/datapoints HTTP/1.1\r\n"); //注意后面必须加上\r\n
	strcat(send_buf, "api-key:");
	strcat(send_buf, API_VALUE_temp);
	strcat(send_buf, "\r\n");
	strcat(send_buf, "Host:");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\r\n");
	sprintf(tmp, "Content-Length:%d\r\n\r\n", strlen(text)); //计算JSON串长度
	strcat(send_buf, tmp);
	strcat(send_buf, text);

	if (sendCommand(send_buf, send_buf, 3000, 1) == Success);
	else errorLog(9);

	char sendCom[2] = {0x1A};
	if (sendCommand(sendCom, "\"succ\"}", 3000, 1) == Success);
	else errorLog(10);

	if (sendCommand("AT+CIPCLOSE\r\n", "CLOSE OK\r\n", 3000, 1) == Success);
	else errorLog(11);

	if (sendCommand("AT+CIPSHUT\r\n", "SHUT OK\r\n", 3000, 1) == Success);
	else errorLog(11);
}

void initGprs()
{
	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(1);

	if (sendCommand("AT+CGCLASS=\"B\"\r\n", "OK\r\n", 3000, 2) == Success);
	else errorLog(3);

	if (sendCommand("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK", 3000, 2) == Success);
	else errorLog(4);

	if (sendCommand("AT+CGATT=1\r\n", "OK\r\n", 3000, 2) == Success);
	else errorLog(5);

	if (sendCommand("AT+CLPORT=\"TCP\",\"2000\"\r\n", "OK\r\n", 3000, 2) == Success);
	else errorLog(6);
}

void(* resetFunc) (void) = 0; //制造重启命令

void errorLog(int num)
{
	DebugSerial.print("ERROR");
	DebugSerial.println(num);
	while (1)
	{
		digitalWrite(L, HIGH);
		delay(300);
		digitalWrite(L, LOW);
		delay(300);

		if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
		{
			DebugSerial.print("\r\nRESET!!!!!!\r\n");
			resetFunc();
		}
	}
}



unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	clrGprsRxBuffer();
	for (unsigned char n = 0; n < Retry; n++)
	{
		DebugSerial.print("\r\n---------send AT Command:---------\r\n");
		DebugSerial.write(Command);

		GprsSerail.write(Command);

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			gprsReadBuffer();
			if (strstr(gprsRxBuffer, Response) != NULL)
			{
				DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
				DebugSerial.print(gprsRxBuffer); //输出接收到的信息
				clrGprsRxBuffer();
				return Success;
			}
		}
		Time_Cont = 0;
	}
	DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
	DebugSerial.print(gprsRxBuffer);//输出接收到的信息
	clrGprsRxBuffer();
	return Failure;
}




void Timer1_handler(void)
{
	Time_Cont++;
}



void gprsReadBuffer() {
	while (GprsSerail.available())
	{
		gprsRxBuffer[gprsBufferCount++] = GprsSerail.read();
		if (gprsBufferCount == gprsRxBufferLength)clrGprsRxBuffer();
	}
}

void clrGprsRxBuffer(void)
{
	memset(gprsRxBuffer, 0, gprsRxBufferLength);      //清空
	gprsBufferCount = 0;
}
