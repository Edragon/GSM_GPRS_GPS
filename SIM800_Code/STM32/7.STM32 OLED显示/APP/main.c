#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "oledbmp.h"

int main(void)
{
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	OLED_Init();
  OLED_Clear();
 
  while(1)
  {
    OLED_ShowCHinese(0,0,0);   //��
    OLED_ShowCHinese(18,0,1);  //��
    OLED_ShowCHinese(36,0,2);  //��
    OLED_ShowCHinese(54,0,3);  //ʾ
    
    OLED_ShowString(0,2,"STM32 OLED");  
    OLED_ShowString(8,6,"2015/xx/xx"); 
    LED0=!LED0;
    delay_ms(1000);
    OLED_Clear();
    
    OLED_DrawBMP(0,0,128,8,BMP1);
    LED0=!LED0;
    delay_ms(1000);
    OLED_Clear();
  }
}
