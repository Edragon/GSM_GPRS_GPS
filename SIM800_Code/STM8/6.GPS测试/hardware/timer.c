//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#include "stm8s.h"
#include "timer.h"

void TIM1_Config(void)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
  TIM1_DeInit();
  TIM1_TimeBaseInit(8000,TIM1_COUNTERMODE_UP,5,0);  //1ms*10=10ms
//  TIM1_TimeBaseInit(8,TIM1_COUNTERMODE_UP,70000,0);
//  TIM1_ARRPreloadConfig(ENABLE);
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);
  TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE); 
  TIM1_Cmd(ENABLE);
}
