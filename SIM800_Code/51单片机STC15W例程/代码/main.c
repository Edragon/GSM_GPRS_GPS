/******************************************************************************
	���ߣ����زر���
	���̣�ILoveMCU.taobao.com
	���ܣ�
		���� 11.0592MHz
		
		����1���ڷ��͸����Ե�����Ϣ	  ������9600
		����2���ڽ�GPSģ��    GPS��TXD �� P1.0 ����λ�ɹ���PPSָʾ�ƻ���˸  //115200������
		����3���ڽ�GPRSģ��	  GPRS��TXD��P0.0,GPRS��RXD��P0.1	 ������9600

******************************************************************************/
#include "main.h"
#include "delay.h"
#include "uart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*************	����˵��	**************

��������ʾLED��˸  


******************************************/

/*************	���س�������	**************/
#define Success 1U
#define Failure 0U

/*************	���ر�������	**************/
unsigned long  Time_Cont = 0;       //��ʱ��������
bit ledState = LED_OFF;

unsigned int gprsBufferCount = 0;
code char OneNetServer[] = "api.heclouds.com";       //����Ҫ�޸�


code char device_id[] = "3225187";    //�޸�Ϊ�Լ����豸ID
code char API_KEY[] = "R9xO5NZm6oVI4YBHvCPKEqtwYtMA";    //�޸�Ϊ�Լ���API_KEY
code char sensor_gps[] = "location";				//����Ҫ�޸�



/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



/******************** IO���ú��� **************************/
void	GPIO_config(void)
{
	P5M0 = 0; 		//����׼˫���
	P5M1 = 0;
}

/******************** ������ **************************/
void main(void)
{	
	GPIO_config();
	UartInit();

	SendString("ILoveMCU.taobao.com");
	delay_ms(200);
	clrStruct();

	initGprs();
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();	
	}
}

void initGprs()
{

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

//	if (sendCommand("AT+CLPORT=\"TCP\",\"2000\"\r\n", "OK\r\n", 3000, 2) == Success);
//	else errorLog(6);
//	delay_ms(100);


}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	Uart3CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		SendString("\r\n---------send AT Command:---------\r\n");
		SendString(Command);

		Uart3SendString(Command); 		//����GPRSָ��

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(Uart3_Rec_Buf, Response) != NULL)
			{
				SendString("\r\n==========receive AT Command:==========\r\n");
				SendString(Uart3_Rec_Buf); //������յ�����Ϣ
				Uart3CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	SendString("\r\n==========receive AT Command:==========\r\n");
	SendString(Uart3_Rec_Buf);//������յ�����Ϣ
	Uart3CLR_Buf();
	return Failure;
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
	unsigned long xdata lon_Onenet = 0;
	unsigned int xdata dd_int = 0;
	unsigned long xdata mm_int = 0;
	float xdata lon_Onenet_double = 0;
	int i = 0;

	unsigned long tempInt = 0;
	unsigned long tempPoint = 0;
	char xdata result[20];
	char xdata point_result[15];
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
	unsigned long xdata lat_Onenet = 0;
	int xdata dd_int = 0;
	unsigned long xdata mm_int = 0;

	int i = 0;

	unsigned long tempInt = 0;
	unsigned long tempPoint = 0;
	char xdata result[20];
	char xdata point_result[15];
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
	char xdata send_buf[400] = {0};
	char xdata text[200] = {0};
	char xdata tmp[25] = {0};

	char xdata lon_str_end[15] = {0};
	char xdata lat_str_end[15] = {0};

	char xdata sendCom[2] = {0x1A};

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

void soft_reset(void)	 //������������
{
   ((void (code *) (void)) 0x0000) ();
}

void errorLog(int num)
{
	SendString("ERROR");
	SendData(num+0x30);
	SendString("\r\n");
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK", 100, 10) == Success)
		{
			SendString("\r\nRESET!!!!!!\r\n");

			soft_reset();
		}
		delay_ms(200);
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
		SendString("**************\r\n");
		SendString(Save_Data.GPS_Buffer);

		
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
		
		SendString("Save_Data.UTCTime = ");
		SendString(Save_Data.UTCTime);
		SendString("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			SendString("Save_Data.latitude = ");
			SendString(Save_Data.latitude);
			SendString("\r\n");


			SendString("Save_Data.N_S = ");
			SendString(Save_Data.N_S);
			SendString("\r\n");

			SendString("Save_Data.longitude = ");
			SendString(Save_Data.longitude);
			SendString("\r\n");

			SendString("Save_Data.E_W = ");
			SendString(Save_Data.E_W);
			SendString("\r\n");

			LED = LED_ON;
			delay_ms(100);
			LED = LED_OFF;

			postGpsDataToOneNet(API_KEY, device_id, sensor_gps, Save_Data.longitude, Save_Data.latitude);		//�������ݵ�Onenet

		}
		else
		{
			SendString("GPS DATA is not usefull!\r\n");
		}
		
	}
}


void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}
