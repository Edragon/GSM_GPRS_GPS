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

#include "oled.h"
#include "oledbmp.h"
#include "timer.h"
#include "uart.h"
#include "sim800.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)        
{ 
  
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
  
//  disableInterrupts();
  enableInterrupts();  //�����ж�  
  
  GPIO_Init( LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  //IO��ʼ��
  GPIO_Init( PWR_PORT, PWR_PIN, GPIO_MODE_OUT_PP_LOW_FAST);   

  UART2_Config(9600);   //9600bps 
  TIM1_Config();        //������ɶ�ʱ��

  OLED_Init();	       //��ʼ��OLED  
  OLED_Clear();
  
  OLED_ShowString(8,0,"Message Test");  
 
  SIM800_Init();
  
  while(1)
  { 
    test();
    GPIO_WriteReverse( LED_PORT, LED_PIN ); 
    delay_ms(300);
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
