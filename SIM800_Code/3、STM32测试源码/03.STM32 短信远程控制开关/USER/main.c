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

/**********************************************************************
		 ����������
		 ���Ͷ��� shutdown ���� �ػ����رռ̵�����
		 ���Ͷ��� reboot ���� �������򿪼̵���
***********************************************************************/


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
char msg[] = "ILoveMCU.taobao.com";		//��������	


void errorLog(int num);
void phone(char *number);
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
void Sys_Soft_Reset(void);
void sendMessage(char *number,char *msg);
void parseMessage();

u8 messageBuffer[100];
u8 isTimeOut = 0;

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

	if (sendCommand("AT+CMGF=1\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog(2);
	delay_ms(10);

	if (sendCommand("AT+CSCS=\"GSM\"\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog(3);
	delay_ms(10);

	if (sendCommand("AT+CNMI=2,2\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(4);
	delay_ms(10);


	
	while(1)
	{
		if(isStartGetMessage)
		{
			printf("isStartGetMessage\r\n");
			isStartGetMessage = 0;
			Time_Cont = 0;
			
			while (Time_Cont < 1000)	//�ȴ�1s���ն�������
			{
				delay_ms(100);
				Time_Cont += 100;
			}
			isGetMessage = 	Success;
		}
		if( isGetMessage == Success)
		{
			printf("isGetMessage\r\n");
			parseMessage();
			
			printf("GET MESSAGE:%s\r\n",messageBuffer);
	
			if(strstr(messageBuffer, "shutdown") != NULL || strstr(messageBuffer, "5173673A") != NULL)	//5173 673A //�ػ�
			{
			  	LED1 = 1;
			}
			
	
			if(strstr(messageBuffer, "reboot") != NULL || strstr(messageBuffer, "91CD542F") != NULL)	//91CD 542F //����
			{
					LED1 = 0;
			}
			isGetMessage = Failure	;
			USART2_CLR_Buf();
		}
	}
}

void parseMessage()
{
	char *messageHead=NULL;
	char *messageEnd1=NULL;
	char *messageEnd2=NULL;

	memset(messageBuffer, 0, sizeof(messageBuffer)-1); 	//��ն������ݻ�����
	messageHead = strstr(USART2_RX_BUF, "CMT");
	if (messageHead != NULL)
	{
		messageEnd1 = strstr(messageHead, "\n");
		if (messageEnd1 != NULL)
		{
			messageEnd1 ++;
			messageEnd2 = strstr(messageEnd1, "\n");
			if (messageEnd2 != NULL)
			{	
				memcpy(messageBuffer, messageEnd1, messageEnd2 - messageEnd1);
	
			}
		}

	}

	CLR_Buf();
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

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else errorLog(7);

	memset(send_buf, 0, 40);    //���
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog(8);
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





