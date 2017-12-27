//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//diy Audio
//技术支持:aemails@163.com
//版权所有，盗版必究。
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