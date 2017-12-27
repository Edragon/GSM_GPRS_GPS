/*********************************************************************
		 ���ߣ����زر���
		 ���̣�ILoveMCU.taobao.com

	�����̽��������ڱ��깺��ģ����û�ʹ�ã������������̵ĵ�����Ϊ
	��Ȩ���У�����ؾ�������
	A6ģ�����ӣ�
	A6 GSM/GPRSģ�飺	
	https://item.taobao.com/item.htm?id=542988971078
	A7 GSM/GPRS/GPS ����һģ�飺
	https://item.taobao.com/item.htm?id=542988971078
	IAP15W4K58S4 STC15W4K56S4 ��Сϵͳ��
	https://item.taobao.com/item.htm?id=539809674723
	USB-TTLģ�飺
	https://item.taobao.com/item.htm?id=39481188174
	�ƶ����Ϲ�����﮵����װ��
	https://item.taobao.com/item.htm?id=530904849115

	A7ģ��		STC15W��Сϵͳ
	GND	 	-->		GND
	U_TXD	-->		P0.0/RXD3
	U_RXD	<--	 	P0.1/TXD3

	GPS_TXD	-->		P1.0/RXD2
*********************************************************************/
#include "main.h"
#include "delay.h"
#include "uart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*************	����˵��	**************



******************************************/

/*************	���س�������	**************/
#define Success 1U
#define Failure 0U

/*************	���ر�������	**************/
unsigned long  Time_Cont = 0;       //��ʱ��������
bit ledState = LED_OFF;

unsigned int gprsBufferCount = 0;
char phoneNumber[] = "1771922xxxx";		//�滻����Ҫ������绰�ĺ���
char msg[] = "ILoveMCU.taobao.com";		//��������	


/*************	���غ�������	**************/

void phone(char *number);
void sendMessage(char *number,char *msg);

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

	sendMessage(phoneNumber,msg);		//���Ͷ���
	while(1)
	{
		
	}
}

void sendMessage(char *number,char *msg)
{
	xdata char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog(1);
	delay_ms(100);

	if (sendCommand(msg, msg, 3000, 5) == Success);
	else errorLog(2);
	delay_ms(100);

	memset(send_buf, 0, 20);    //���
	send_buf[0] = 0x1a;
	if (sendCommand(send_buf, send_buf, 5000, 5) == Success);
	else errorLog(3);
	delay_ms(100);

}

void phone(char *number)
{
	xdata char send_buf[30] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "SOUNDER", 10000, 10) == Success);
	else errorLog(5);
}

void initGprs()
{
	if (sendCommand("AT\r\n", "OK", 3000, 10) == Success);
	else errorLog(1);

	if (sendCommand("AT+CPIN?\r\n", "READY", 1000, 10) == Success);
	else errorLog(2);
	delay_ms(10);

	if (sendCommand("AT+CMGF=1\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog(3);
	delay_ms(10);

	if (sendCommand("AT+CSCS=\"GSM\"\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog(4);
	delay_ms(10);

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

			LED = ledState;
			ledState = ~ledState;

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
