/**********************************************************
						���ߣ����زر���
						���̣�ILoveMCU.taobao.com

	�����̽��������ڱ��깺��ģ����û�ʹ�ã������������̵ĵ�����Ϊ
	��Ȩ���У�����ؾ�������
	SIM800 GSM/GPRSģ��
	https://item.taobao.com/item.htm?id=521599424969
	����ʹ��STM32��Сϵͳ��
	https://item.taobao.com/item.htm?id=523336310868
	STC12C5A60S2��Сϵͳ��
	https://item.taobao.com/item.htm?id=524701473371
	USB-TTLģ�飺
	https://item.taobao.com/item.htm?id=39481188174
	�ƶ���Դ﮵����װ
	https://item.taobao.com/item.htm?id=530904849115
	�ƶ����Ϲ�����﮵����װ��
	https://item.taobao.com/item.htm?id=530904849115
	
	����˵����
	STM32					SIM800C
	3.3V	------>	V_MCU
	GND		<----->	GND
	PA3		<------	T_TX
	PA2		------>	T_RX
	

	//���ڵ��Կɲ���
	STM32					USB-TTLģ��
	GND		------>	GND
	TX1		------>	RXD
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

char phoneNumber[] = "1771922XXXX";		//�滻����Ҫ������绰�ĺ���


void errorLog(int num);
void phone(char *number);
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
void Sys_Soft_Reset(void);

int main(void)
{	
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	USART2_Init(9600);	//����2������9600
	u2_printf("AT\r\n");
	Init_LEDpin();
	
	
	
	printf("Welcome to use!\r\n");
	printf("ILoveMcu.taobao.com!\r\n");
	
	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(1);
	delay_ms(10);

	if (sendCommand("AT+CPIN?\r\n", "READY", 1000, 10) == Success);
	else errorLog(2);
	delay_ms(10);


	phone(phoneNumber);		//��绰
	
	while(1)
	{
		
	}
}

void errorLog(int num)
{
	printf("ERROR%d\r\n",num);
	while (1)
	{
		if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
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

	if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
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





