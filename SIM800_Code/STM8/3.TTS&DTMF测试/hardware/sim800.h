//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __SIM800_H
#define __SIM800_H

/**************** PWR , LED**************************/
#define LED_PORT                GPIOF
#define LED_PIN                 GPIO_PIN_4
#define PWR_PORT                GPIOB
#define PWR_PIN                 GPIO_PIN_5
/****************************************************/

void delay(u16 t);
void SIM800_Init(void);
void test(void);

#endif