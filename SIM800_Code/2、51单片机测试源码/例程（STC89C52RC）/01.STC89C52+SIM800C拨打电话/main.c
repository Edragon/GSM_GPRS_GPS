/*********************************************************************
		 ���ߣ����زر���
		 ���̣�ILoveMCU.taobao.com

	�����̽��������ڱ��깺��ģ����û�ʹ�ã������������̵ĵ�����Ϊ
	��Ȩ���У�����ؾ�������
	SIM800 GSM/GPRSģ��
	https://item.taobao.com/item.htm?id=521599424969
	STC89C52��Сϵͳ��
	https://item.taobao.com/item.htm?id=524713612727
	STC12C5A60S2��Сϵͳ��
	https://item.taobao.com/item.htm?id=524701473371
	USB-TTLģ�飺
	https://item.taobao.com/item.htm?id=39481188174
	�ƶ���Դ﮵����װ
	https://item.taobao.com/item.htm?id=530904849115
	�ƶ����Ϲ�����﮵����װ��
	https://item.taobao.com/item.htm?id=530904849115
*********************************************************************/

#include "main.h"
#include "uart.h"

//����
#define Success 1U
#define Failure 0U

//�������
unsigned long  Time_Cont = 0;       //��ʱ��������

char phoneNumber[] = "1771922xxxx";		//�滻����Ҫ������绰�ĺ���

//****************************************************
//������
//****************************************************
void main()
{

    Uart_Init();						  

	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CPIN?\r\n", "READY", 1000, 10) == Success);
	else errorLog();
	delay_ms(10);


	phone(phoneNumber);		//��绰

	while(1)
	{	

	}
}

void phone(char *number)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
	else errorLog();
}

void errorLog()
{
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
		{
			soft_reset();
		}
		delay_ms(200);
	}
}

void soft_reset(void)	 //������������
{
   ((void (code *) (void)) 0x0000) ();
}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		SendString(Command); 		//����GPRSָ��

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(Rec_Buf, Response) != NULL)
			{
				
				CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	
	CLR_Buf();
	return Failure;
}

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void delay_ms(unsigned int n)
{
	unsigned int  i,j;
		for(i=0;i<n;i++)
			for(j=0;j<123;j++);
}