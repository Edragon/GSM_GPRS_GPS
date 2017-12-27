//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stdio.h"
#include "string.h"
#include "uart.h"
#include "oled.h"
#include "oledbmp.h"

/* Private defines -----------------------------------------------------------*/
#define LED_PORT  GPIOF
#define LED_PIN   GPIO_PIN_4

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)        
{ 
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
  
  enableInterrupts();  //�����ж�  
  
  GPIO_Init( LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  //IO��ʼ�� 

  UART2_Config(9600);  //9600bps 
 
  OLED_Init();	       //��ʼ��OLED  
  OLED_Clear();
   
  while(1)
  {
    OLED_ShowCHinese(0,0,0);   //Զ
    OLED_ShowCHinese(18,0,1);  //��
    OLED_ShowCHinese(36,0,2);  //��
    OLED_ShowCHinese(54,0,3);  //��
    OLED_ShowCHinese(72,0,4);  //ģ
    OLED_ShowCHinese(90,0,5);  //��
    
    OLED_ShowString(0,2,"SIM800H+STM8");  
    OLED_ShowString(8,6,"2015/08/07"); 
    GPIO_WriteReverse(LED_PORT, LED_PIN); 
    delay_ms(1000);
    OLED_Clear();
    
    OLED_DrawBMP(0,0,128,8,BMP1);
    GPIO_WriteReverse(LED_PORT, LED_PIN); 
    delay_ms(1000);
    OLED_Clear();
  }
}
  
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
