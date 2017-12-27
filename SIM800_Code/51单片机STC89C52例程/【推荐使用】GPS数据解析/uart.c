/******************************************************************
	���ߣ����زر���
	���̣�ILoveMCU.taobao.com
	���ս���Ȩ��ԭ�������У������������̲��Ͷ���ĳ�Ϯ��Ϊ��
******************************************************************/
#include "uart.h"

char idata gpsRxBuffer[gpsRxBufferLength];
unsigned char RX_Count = 0;
_SaveData Save_Data;

void Uart_Init()					  			   
{
	SCON=0x50; //���ڹ�����ʽ1��8λUART�������ʿɱ�  
     TH2=0xFF;           
     TL2=0xFD;    //������:115200 ����=11.0592MHz 
     RCAP2H=0xFF;   
     RCAP2L=0xFD; //16λ�Զ���װ��ֵ



/*****************/
     TCLK=1;   
     RCLK=1;   
     C_T2=0;   
     EXEN2=0; //�����ʷ�����������ʽ



/*****************/
    TR2=1 ; //��ʱ��2��ʼ

	EA   = 1;     //��ȫ���жϿ���
	ES   = 1;     //���п��ж�	
}

void UartPrintf(unsigned char *p)				//�����ַ���
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{

		};
		TI=0;
	}   
}

void UartPrintASCII(unsigned char c)				//����һ���ַ�
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0);   
    TI=0;   
}


void RECEIVE_DATA(void) interrupt 4 using 3	  		
{ 
	unsigned char temp = 0;
	char i = 0;
	ES=0;
	temp = SBUF;
	RI = 0;
	
	if(temp == '$')
	{
		RX_Count = 0;	
	}
		
	if(RX_Count <= 5)
	{
	   gpsRxBuffer[RX_Count++] = temp;
	}
	else if(gpsRxBuffer[0] == '$' &gpsRxBuffer[4] == 'M' && gpsRxBuffer[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
	{
		gpsRxBuffer[RX_Count++] = temp;
		if(temp == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
			memcpy(Save_Data.GPS_Buffer, gpsRxBuffer, RX_Count); 	//��������
			Save_Data.isGetData = true;
			RX_Count = 0;
			memset(gpsRxBuffer, 0, gpsRxBufferLength);      //���				
		}
		
		if(RX_Count >= 75)
		{
			RX_Count = 75;
			gpsRxBuffer[RX_Count] = '\0';//��ӽ�����
		}			
	}
	ES=1; 
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
























