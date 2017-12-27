#ifndef __UART_H
#define __UART_H

#include	"config.h"

#define		S1_USE_P30P31()		P_SW1 &= ~0xc0					//UART1 ʹ��P30 P31��	Ĭ��
#define		S1_USE_P36P37()		P_SW1 = (P_SW1 & ~0xc0) | 0x40	//UART1 ʹ��P36 P37��
#define		S1_USE_P16P17()		P_SW1 = (P_SW1 & ~0xc0) | 0x80	//UART1 ʹ��P16 P17��

#define		TI2					(S2CON & 2) != 0
#define		RI2					(S2CON & 1) != 0
#define		SET_TI2()			S2CON |=  2
#define		CLR_TI2()			S2CON &= ~2
#define		CLR_RI2()			S2CON &= ~1

#define		UART2_INT_ENABLE()		IE2 |=  1	//������2�ж�
#define		UART2_INT_DISABLE()		IE2 &= ~1	//������2�ж�

#define		TI3					(S3CON & 2) != 0
#define		RI3					(S3CON & 1) != 0
#define		SET_TI3()			S3CON |=  2
#define		CLR_TI3()			S3CON &= ~2
#define		CLR_RI3()			S3CON &= ~1

#define		UART3_INT_ENABLE()		IE2 |=  (1<<3)	//������3�ж�
#define		UART3_INT_DISABLE()		IE2 &= ~(1<<3)	//������3�ж�

#define false 0
#define true 1

//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} xdata _SaveData;

#define gpsRxBufferLength  76
extern char idata gpsRxBuffer[gpsRxBufferLength];
extern unsigned char RX_Count;
extern _SaveData Save_Data;

extern u8 xdata  Uart3_Rec_Buf[];



void UartInit();
void SendData(unsigned char ch);
void SendString(char *s);
bit Hand(unsigned char *a) ;
void CLR_Buf(void) ;

void Uart2SendData(unsigned char ch);
void Uart2SendString(char *s);
//bit Uart2Hand(unsigned char *a) ;
//void Uart2CLR_Buf(void) ;

void Uart3SendData(unsigned char ch);
void Uart3SendString(char *s);
bit Uart3Hand(unsigned char *a) ;
void Uart3CLR_Buf(void) ;



#endif