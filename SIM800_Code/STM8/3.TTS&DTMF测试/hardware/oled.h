//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm8s.h"

//-----------------OLED�˿ڶ���----------------  
/****************ʱ��*********************/
#define OLED_SCLK_PORT  (GPIOD)
#define OLED_SCLK_PINS  (GPIO_PIN_1)

/****************����*********************/
#define OLED_SDIN_PORT  (GPIOD)
#define OLED_SDIN_PINS  (GPIO_PIN_3)

/****************��λ*********************/
#define OLED_RST_PORT  (GPIOD)
#define OLED_RST_PINS  (GPIO_PIN_4)

/****************����/����*********************/
#define OLED_DC_PORT  (GPIOD)
#define OLED_DC_PINS  (GPIO_PIN_7)

/****************Ƭѡ*********************/
#define OLED_CS_PORT  (GPIOD)
#define OLED_CS_PINS  (GPIO_PIN_2)

#define OLED_SCLK_Clr() GPIO_WriteLow(OLED_SCLK_PORT, OLED_SCLK_PINS)
#define OLED_SCLK_Set() GPIO_WriteHigh(OLED_SCLK_PORT, OLED_SCLK_PINS)

#define OLED_SDIN_Clr() GPIO_WriteLow(OLED_SDIN_PORT, OLED_SDIN_PINS)
#define OLED_SDIN_Set() GPIO_WriteHigh(OLED_SDIN_PORT, OLED_SDIN_PINS)

#define OLED_RST_Clr() GPIO_WriteLow(OLED_RST_PORT, OLED_RST_PINS)
#define OLED_RST_Set() GPIO_WriteHigh(OLED_RST_PORT, OLED_RST_PINS)

#define OLED_DC_Clr() GPIO_WriteLow(OLED_DC_PORT, OLED_DC_PINS)
#define OLED_DC_Set() GPIO_WriteHigh(OLED_DC_PORT, OLED_DC_PINS)

#define OLED_CS_Clr()  GPIO_WriteLow(OLED_CS_PORT, OLED_CS_PINS)
#define OLED_CS_Set()  GPIO_WriteHigh(OLED_CS_PORT, OLED_CS_PINS)


//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
					  
//OLED�����ú���
void delay_ms(unsigned int ms);
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif  
	 



