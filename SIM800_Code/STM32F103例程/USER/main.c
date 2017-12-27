/**********************************************************
						���ߣ����زر���
						���̣�ILoveMCU.taobao.com
												
����˵����
STM32					SIM868
GND		<----->	GND
PA2/TX2------>	U_RXD
PA3/RX2<------	U_TXD
RX1/PA10<------ GPS_TXD

//���ڵ��Կɲ���
STM32					USB-TTLģ��
GND		------>	GND
TX1/PA9------>	RXD

����ʹ��STM32��Сϵͳ��
	https://item.taobao.com/item.htm?id=523336310868
����ʹ��USB-TTLģ�飺
	https://item.taobao.com/item.htm?id=39481188174
����ʹ��SIM868 GSM/GPRS/GPSģ�飺
	https://item.taobao.com/item.htm?spm=a1z10.3-c.w4002-175517240.12.AJQuCN&id=553243675157
	�ƶ����Ϲ�����﮵����װ��
	https://item.taobao.com/item.htm?id=530904849115
***********************************************************/

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usart2.h"


//����
#define Success 1U
#define Failure 0U

//�������
unsigned long  Time_Cont = 0;       //��ʱ��������


char OneNetServer[] = "api.heclouds.com";       //����Ҫ�޸�


char device_id[] = "3225187";    //�޸�Ϊ�Լ����豸ID
char API_KEY[] = "R9xO5NZm6oVI4YBHvCPKEqtwYtMA";    //�޸�Ϊ�Լ���API_KEY
char sensor_gps[] = "location";				//����Ҫ�޸�

unsigned int count = 0;

void errorLog(int num);
void phone(char *number);
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
void sendMessage(char *number,char *msg);
void Sys_Soft_Reset(void);
void parseGpsBuffer(void);
void printGpsBuffer(void);
void postGpsDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, char* lon_temp, char* lat_temp);
char* longitudeToOnenetFormat(char *lon_str_temp);
char* latitudeToOnenetFormat(char *lat_str_temp);
int Digcount(long num);

int main(void)
{	
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	USART2_Init(9600);	//����2������9600
	u2_printf("AT\r\n");
	Init_LEDpin();
	
	
	
	printf("Welcome to use!\r\n");
	printf("ILoveMcu.taobao.com!\r\n");
	
	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(1);
	delay_ms(100);

	if (sendCommand("AT+CGCLASS=\"B\"\r\n", "OK\r\n", 3000, 2) == Success);
	else errorLog(3);
	delay_ms(100);

	if (sendCommand("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK", 3000, 2) == Success);
	else errorLog(4);
	delay_ms(100);

	if (sendCommand("AT+CGATT=1\r\n", "OK\r\n", 3000, 2) == Success);
	else errorLog(5);
	delay_ms(100);


	clrStruct();
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//��������
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
			
			
			postGpsDataToOneNet(API_KEY, device_id, sensor_gps, Save_Data.longitude, Save_Data.latitude);		//�������ݵ�Onenet
			
			LED1 = 0;
			delay_ms(100);
			LED1 = 1;
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

int Digcount(long num)
{
	int i=0;	
	while(num>0)
	{ 
		i++;
		num=num/10;
	}
  return i;
}

char* longitudeToOnenetFormat(char *lon_str_temp) 		//����
{
	unsigned long lon_Onenet = 0;
	unsigned int dd_int = 0;
	unsigned long mm_int = 0;
	float lon_Onenet_double = 0;
	int i = 0;

	unsigned long tempInt = 0;
	unsigned long tempPoint = 0;
	char result[20];
	char point_result[15];
	int pointLength = 0;

	//51��Ƭ��û��double��double��float�����Ȳ���������ֻ�ֿܷ�������С�����㡣
	sscanf(lon_str_temp, "%ld.%ld", &tempInt,&tempPoint);
	lon_Onenet = tempInt%100;
	pointLength = strlen(lon_str_temp) - 2 - Digcount(tempInt);		
	for( i = 0 ; i < pointLength ; i++)	//С���㼸λ���������־ͷŴ�10�ļ��η�
	{
		lon_Onenet *= 10; 	
	}

	dd_int = tempInt / 100; //ȡ��dd

	mm_int = lon_Onenet + tempPoint; //ȡ��MM����

	mm_int = mm_int*10/6;	 		//�����ǳ���60������*10/6Ϊ�˶�2λС��������Ч����


   	sprintf(result,"%d.",dd_int);
	for( i = 0 ; i < pointLength + 2 - Digcount(mm_int) ; i++)
	{
		strcat(result, "0");	
	}
	sprintf(point_result,"%ld",mm_int);
	strcat(result, point_result);

//	SendString("\r\n==========ONENET FORMART==========\r\n");
//	SendString(result);
	return result;
}



char* latitudeToOnenetFormat(char *lat_str_temp) 		//γ��
{
	unsigned long lat_Onenet = 0;
	int dd_int = 0;
	unsigned long mm_int = 0;

	int i = 0;

	unsigned long tempInt = 0;
	unsigned long tempPoint = 0;
	char result[20];
	char  point_result[15];
	int pointLength = 0;
//	char xdata debugTest[30];
	
	//51��Ƭ��û��double��double��float�����Ȳ���������ֻ�ֿܷ�������С�����㡣
	sscanf(lat_str_temp, "%ld.%ld", &tempInt,&tempPoint);
	lat_Onenet = tempInt%100;
	
//	SendString("\r\n==========ONENET FORMART strlen(lat_str_temp)==========\r\n");
//	sprintf(debugTest,"%d",strlen(lat_str_temp));
//	SendString(debugTest);

	pointLength = strlen(lat_str_temp) - 2 - Digcount(tempInt);	

//	SendString("\r\n==========ONENET FORMART pointLength==========\r\n");
//	sprintf(debugTest,"%d",pointLength);
//	SendString(debugTest);
	for( i = 0 ; i < pointLength ; i++)	//С���㼸λ���������־ͷŴ�10�ļ��η�
	{
		lat_Onenet *= 10; 	
	}

//	SendString("\r\n==========ONENET FORMART tempPoint==========\r\n");
//	sprintf(debugTest,"%ld",tempPoint);
//	SendString(debugTest);
//
//	SendString("\r\n==========ONENET FORMART tempInt==========\r\n");
//	sprintf(debugTest,"%ld",tempInt);
//	SendString(debugTest);
//
//	SendString("\r\n==========ONENET FORMART lat_Onenet==========\r\n");
//	sprintf(debugTest,"%ld",lat_Onenet);
//	SendString(debugTest);

	dd_int = tempInt / 100; //ȡ��dd

	mm_int = lat_Onenet + tempPoint; //ȡ��MM����

	mm_int = mm_int*10/6;	 		//�����ǳ���60������*10/6Ϊ�˶�2λС��������Ч����

//	SendString("\r\n==========ONENET FORMART mm_int==========\r\n");
//	sprintf(debugTest,"%ld",mm_int);
//	SendString(debugTest);

	
	sprintf(result,"%d.",dd_int);
	for( i = 0 ; i < pointLength + 2 - Digcount(mm_int) ; i++)
	{
		strcat(result, "0");	
	}
	sprintf(point_result,"%ld",mm_int);
	strcat(result, point_result);

//	SendString("\r\n==========ONENET FORMART==========\r\n");
//	SendString(result);
	return result;
}

void postGpsDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, char* lon_temp, char* lat_temp)
{
	char send_buf[400] = {0};
	char text[200] = {0};
	char tmp[25] = {0};

	char lon_str_end[15] = {0};
	char lat_str_end[15] = {0};

	char sendCom[2] = {0x1A};

//	dtostrf(longitudeToOnenetFormat(lon_temp), 3, 6, lon_str_end); //ת�����ַ������
//	dtostrf(latitudeToOnenetFormat(lat_temp), 2, 6, lat_str_end); //ת�����ַ������

//	lon_temp = "11224.4992";
//	lat_temp = "3438.1633";

	sprintf(lon_str_end,"%s", longitudeToOnenetFormat(lon_temp)); 
	sprintf(lat_str_end,"%s", latitudeToOnenetFormat(lat_temp)); 

	//���ӷ�����
	memset(send_buf, 0, 400);    //���
	strcpy(send_buf, "AT+CIPSTART=\"TCP\",\"");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\",80\r\n");
	if (sendCommand(send_buf, "CONNECT", 10000, 3) == Success);
	else errorLog(7);

	//��������
	if (sendCommand("AT+CIPSEND\r\n", ">", 3000, 1) == Success);
	else errorLog(8);

	memset(send_buf, 0, 400);    //���

	/*׼��JSON��*/
	//ARDUINOƽ̨��֧��sprintf��double�Ĵ�ӡ��ֻ��ת�����ַ���Ȼ���ӡ
	sprintf(text, "{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%s,\"lat\":%s}}]}]}"
	        , sensor_id_temp, lon_str_end, lat_str_end);

	/*׼��HTTP��ͷ*/
	send_buf[0] = 0;
	strcat(send_buf, "POST /devices/");
	strcat(send_buf, device_id_temp);
	strcat(send_buf, "/datapoints HTTP/1.1\r\n"); //ע�����������\r\n
	strcat(send_buf, "api-key:");
	strcat(send_buf, API_VALUE_temp);
	strcat(send_buf, "\r\n");
	strcat(send_buf, "Host:");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\r\n");
	sprintf(tmp, "Content-Length:%d\r\n\r\n", strlen(text)); //����JSON������
	strcat(send_buf, tmp);
	strcat(send_buf, text);

	if (sendCommand(send_buf, send_buf, 3000, 1) == Success);
	else errorLog(9);

	
	if (sendCommand(sendCom, "\"succ\"}", 3000, 1) == Success);
	else errorLog(10);

	delay_ms(100);
	if (sendCommand("AT+CIPCLOSE\r\n", "CLOSE OK", 3000, 1) == Success);
	else errorLog(11);
	delay_ms(100);

	if (sendCommand("AT+CIPSHUT\r\n", "SHUT OK", 3000, 1) == Success);
	else errorLog(11);
	delay_ms(100);

}


void sendMessage(char *number,char *msg)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog(6);


	if (sendCommand(msg, msg, 3000, 1) == Success);
	else errorLog(7);
	delay_ms(100);

	memset(send_buf, 0, 100);    //���
	send_buf[0] = 0x1a;
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog(8);
	delay_ms(100);
}

void errorLog(int num)
{
	printf("ERROR%d\r\n",num);
	while (1)
	{
		if (sendCommand("AT\r\n", "OK", 100, 10) == Success)
		{
			Sys_Soft_Reset();
		}
		delay_ms(200);
	}
}

void Sys_Soft_Reset(void)
{  
    SCB->AIRCR =0X05FA0000|(u32)0x04;      
}

void phone(char *number)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "SOUNDER", 10000, 10) == Success);
	else errorLog(4);
}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	USART2_CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		u2_printf(Command); 		//����GPRSָ��
		
		printf("\r\n***************send****************\r\n");
		printf(Command);
		
		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(USART2_RX_BUF, Response) != NULL)
			{				
				printf("\r\n***************receive****************\r\n");
				printf(USART2_RX_BUF);
				USART2_CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	printf("\r\n***************receive****************\r\n");
	printf(USART2_RX_BUF);
	USART2_CLR_Buf();
	return Failure;
}







