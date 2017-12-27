/**********************************************************************************
 * 文件名  ：SysTick.c
 * 描述    ：通过系统滴答时钟SysTick中断实现nMs(n毫秒)、nS(n秒)的延时        
 * 实验平台：NiRen_TwoHeart系统板
 * 硬件连接：无须外接电路       
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "SysTick.h"

static u32 SysTickDelayTime;

/*******************************************************************************
* 函数名  : SysTick_Init_Config
* 描述    : 初始化系统滴答时钟SysTick
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 1)、SystemFrequency / 1000		1ms中断一次
*			2)、SystemFrequency / 100000	10us中断一次
*			3)、SystemFrequency / 1000000	1us中断一次
*			计算方法:(SystemFrequency / Value)个系统时钟节拍中断一次
*******************************************************************************/
void SysTick_Init_Config(void)
{

	while(SysTick_Config(SystemCoreClock / 1000));	//初始化并使能系统滴答时钟,返回1表示计数设置太大			 
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;		  //失能滴答定时器 
}

/*******************************************************************************
* 函数名  : Delay_nMs
* 描述    : 延时程序(n毫秒)
* 输入    : nm：延时时间(n毫秒)
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Delay_nMs(u32 nms)
{ 
	SysTickDelayTime = nms;		 
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//使能滴答定时器 
	while(SysTickDelayTime != 0);					//等待延时时间到
}

/*******************************************************************************
* 函数名  : Delay_S
* 描述    : 延时程序(n秒)
* 输入    : nS：延时时间(n秒)
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Delay_nS(u32 ns)
{ 
	SysTickDelayTime = ns*1000;		 
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//使能滴答定时器 
	while(SysTickDelayTime != 0);					//等待延时时间到
}


/*******************************************************************************
* 函数名  : SysTickDelayTime_Counter
* 描述    : 获取节拍程序
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 在SysTick中断程序SysTick_Handler()调用(stm32f10x_it.c)
*******************************************************************************/ 
void SysTickDelayTime_Counter(void)
{
	if (SysTickDelayTime > 0)
	{ 
		SysTickDelayTime--;
	}
}

/*******************************************************************************
* 函数名  : RCC_Configuration
* 描述    : 设置系统时钟为72MHZ(这个可以根据需要改)
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : STM32F107x和STM32F105x系列MCU与STM32F103x系列MCU时钟配置有所不同
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;               //外部高速时钟(HSE)的工作状态变量
  
  RCC_DeInit();                               //将所有与时钟相关的寄存器设置为默认值
  RCC_HSEConfig(RCC_HSE_ON);                  //启动外部高速时钟HSE 
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); //等待外部高速时钟(HSE)稳定

  if(SUCCESS == HSEStartUpStatus)             //如果外部高速时钟已经稳定
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Flash设置
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
  
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //设置AHB时钟等于系统时钟(1分频)/72MHZ
    RCC_PCLK2Config(RCC_HCLK_Div1);  //设置APB2时钟和HCLK时钟相等/72MHz(最大为72MHz)
    RCC_PCLK1Config(RCC_HCLK_Div2);  //设置APB1时钟是HCLK时钟的2分频/36MHz(最大为36MHz)
  
#ifndef STM32F10X_CL                 //如果使用的不是STM32F107x或STM32F105x系列MCU,PLL以下配置  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLLCLK = 8MHz * 9 = 72 MHz 
#else                                //如果使用的是STM32F107x或STM32F105x系列MCU,PLL以下配置
    /***** 配置PLLx *****/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    RCC_PLL2Cmd(ENABLE); //使能PLL2 
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);//等待PLL2稳定

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#endif

    RCC_PLLCmd(ENABLE); //使能PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //等待PLL稳定

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);          //设置系统时钟的时钟源为PLL

    while(RCC_GetSYSCLKSource() != 0x08);               //检查系统的时钟源是否是PLL
    RCC_ClockSecuritySystemCmd(ENABLE);                 //使能系统安全时钟 

	/* Enable peripheral clocks --------------------------------------------------*/
  	/* Enable I2C1 and I2C1 clock */
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  	/* Enable GPIOA GPIOB SPI1 and USART1 clocks */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
					| RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
					| RCC_APB2Periph_AFIO, ENABLE);    
  }
}
