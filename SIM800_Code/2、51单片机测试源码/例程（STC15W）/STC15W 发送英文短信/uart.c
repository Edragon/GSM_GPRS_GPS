#include "uart.h"
#include<string.h>

#define Uart1_Buf_Max 20               //�������ݻ��泤��
u8 idata  Rec_Buf[Uart1_Buf_Max];  //�������ݻ���
u8 point1 = 0;             //練�ָ��

//#define Uart2_Buf_Max 20               //�������ݻ��泤��
//u8 xdata  Uart2_Rec_Buf[Uart2_Buf_Max];  //�������ݻ���
u8 point2 = 0;             //練�ָ��
bit	B_TX2_Busy = 0;	//����æ��־

char idata gpsRxBuffer[gpsRxBufferLength];
unsigned char RX_Count = 0;
_SaveData Save_Data;

#define Uart3_Buf_Max 600               //�������ݻ��泤��
u8 xdata  Uart3_Rec_Buf[Uart3_Buf_Max];  //�������ݻ���
u8 point3 = 0;             //練�ָ��
bit	B_TX3_Busy = 0;	//����æ��־




void UartInit(void)		//9600bps@11.0592MHz
{
//ע��: STC15W4K32S4ϵ�е�оƬ,�ϵ��������PWM��ص�IO�ھ�Ϊ
//      ����̬,�轫��Щ������Ϊ׼˫��ڻ�ǿ����ģʽ��������ʹ��
//���IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5
//	P3M0 &= ~(1<<7);		//����P3.7Ϊ׼˫��IO
//	P3M1 &= ~(1<<7);
//
//	S1_USE_P36P37();

	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xDC;		//�趨��ʱ��ֵ
	TH1 = 0xDC;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1


	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xE0;		//�趨��ʱ��ֵ
	T2H = 0xFE;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2

	S3CON = 0x10;		//8λ����,�ɱ䲨����
	S3CON |= 0x40;		//����3ѡ��ʱ��3Ϊ�����ʷ�����
	T4T3M |= 0x02;		//��ʱ��3ʱ��ΪFosc,��1T
	T3L = 0xE0;		//�趨��ʱ��ֵ
	T3H = 0xFE;		//�趨��ʱ��ֵ
	T4T3M |= 0x08;		//������ʱ��3

	UART2_INT_ENABLE();
	UART3_INT_ENABLE();
	ES = 1;
	EA = 1;
}

/*----------------------------
���ʹ�������
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //д���ݵ�UART���ݼĴ���
		while(TI == 0);
		TI = 0;
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}

bit Hand(unsigned char *a)                   // ��������ʶ����
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // ���ڻ�������
{
	memset(Rec_Buf, 0, Uart1_Buf_Max);      //���

    point1 = 0;                    
}

void Usart() interrupt 4 using 1            // �����жϺ���
{
	ES = 0;
	if (RI)
    {
      RI = 0;                                //���RIλ
		Rec_Buf[point1] = SBUF; 
//		if (Rec_Buf[0] == 0xd9)
//			{
//				IAP_CONTR = 0x60;
//			}
			
		point1++;               
		if(point1>=Uart1_Buf_Max)          
		{
			point1 = 0;
		}           

    }
    if (TI)
    {
        TI = 0;                 //���TIλ

    }
	ES =  1;
}

/*----------------------------
ͨ������2���ʹ�������
----------------------------*/
void Uart2SendData(unsigned char ch)
{
	while(B_TX2_Busy);	//��æ��������
    S2BUF = ch;                 //д���ݵ�UART���ݼĴ���
	B_TX2_Busy = 1;
}

/*----------------------------
ͨ������2�����ַ���
----------------------------*/
void Uart2SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        Uart2SendData(*s++);         //���͵�ǰ�ַ�
    }
}

//bit Uart2Hand(unsigned char *a)                   // ��������ʶ����
//{ 
//    if(strstr(Uart2_Rec_Buf,a)!=NULL)
//	    return 1;
//	else
//		return 0;
//}

//void Uart2CLR_Buf(void)                           // ���ڻ�������
//{
//	memset(Uart2_Rec_Buf, 0, Uart2_Buf_Max);      //���
//
//    point2 = 0;                    
//}

void Usart2() interrupt 8 using 1 
{
	unsigned char temp = 0;
	char i = 0;

	UART2_INT_DISABLE();		
	if(RI2)
    {
		temp = S2BUF;
		CLR_RI2();                               //���RIλ

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
         

    }
    if (TI2)
    {
        CLR_TI2();                //���TIλ
		B_TX2_Busy = 0;	//���æ��־

    }
	UART2_INT_ENABLE();
	
	

}

/*----------------------------
ͨ������3���ʹ�������
----------------------------*/
void Uart3SendData(unsigned char ch)
{	
    while(B_TX3_Busy);	//��æ��������
    S3BUF = ch;                 //д���ݵ�UART���ݼĴ���
	B_TX3_Busy = 1;
}

/*----------------------------
ͨ������3�����ַ���
----------------------------*/
void Uart3SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        Uart3SendData(*s++);         //���͵�ǰ�ַ�
    }
}

bit Uart3Hand(unsigned char *a)                   // ��������ʶ����
{ 
    if(strstr(Uart3_Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

void Uart3CLR_Buf(void)                           // ���ڻ�������
{
	memset(Uart3_Rec_Buf, 0, Uart3_Buf_Max);      //���

    point3 = 0;                    
}

void Usart3() interrupt 17 
{
	UART3_INT_DISABLE();	
	if(RI3)
    {
		CLR_RI3();                               //���RIλ
		Uart3_Rec_Buf[point3] = S3BUF; 
//		if (Rec_Buf[0] == 0xd9)
//			{
//				IAP_CONTR = 0x60;
//			}
			
		point3++;               
		if(point3>=Uart3_Buf_Max)          
		{
			point3 = 0;
		}           

    }
    if (TI3)
    {
        CLR_TI3();                //���TIλ
		B_TX3_Busy = 0;
    }
	UART3_INT_ENABLE();	
}

