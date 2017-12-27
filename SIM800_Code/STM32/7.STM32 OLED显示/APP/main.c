#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "oledbmp.h"

int main(void)
{
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	OLED_Init();
  OLED_Clear();
 
  while(1)
  {
    OLED_ShowCHinese(0,0,0);   //汉
    OLED_ShowCHinese(18,0,1);  //字
    OLED_ShowCHinese(36,0,2);  //显
    OLED_ShowCHinese(54,0,3);  //示
    
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
