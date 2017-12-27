//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//diy Audio
//技术支持:aemails@163.com
//版权所有，盗版必究。
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#include "stm8s.h"
#include "uart.h"

void UART2_Config(u32 bps)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
  UART2_DeInit();
  UART2_Init( bps, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
  UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
  UART2_Cmd(ENABLE);
}

/******支持printf方法***********/
//1.添加putchar函数
//2.修改Options->General Options->Library Options->Printf formatter->Large 
int putchar(int ch)  
{  
  UART2_SendData8((u8)ch);
  while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
  return ch;  
}

void UART2_SendByte(u8 data)
{
  UART2_SendData8((unsigned char)data);  
  /* Loop until the end of transmission */
  while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
}

void UART2_SendString(u8* Data)
{
  while(*Data!='\0')
  {
    UART2_SendByte(*Data); 
    Data++;
  }
}



