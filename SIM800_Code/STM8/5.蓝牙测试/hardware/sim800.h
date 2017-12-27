//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//diy Audio
//技术支持:aemails@163.com
//版权所有，盗版必究。
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __SIM800_H
#define __SIM800_H


extern u8 UART_RX_BUF[1024];
extern u16 UART_RX_NUM;
extern u8 UART_RX_STA;

void delay(u16 t);
u8 SIM800_Send_CMD(u8 *cmd,u8 *ack);

#endif