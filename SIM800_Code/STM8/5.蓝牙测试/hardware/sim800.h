//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
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