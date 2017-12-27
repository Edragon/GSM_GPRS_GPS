#ifndef __UART_H__
#define __UART_H__

#include	"config.h"

void Uart1Init(void);
void UART1_SendData(u8 dat);
void UART1_SendString(char *s);


//串口1发送回车换行
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)

#endif