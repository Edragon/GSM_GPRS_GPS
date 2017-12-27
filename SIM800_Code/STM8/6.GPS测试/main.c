//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//diy Audio
//技术支持:aemails@163.com
//版权所有，盗版必究。
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
#include "gps.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

nmea_msg gpsx; 	        //GPS信息
u8 dtbuf[50];   	//打印缓存

//显示GPS定位信息 
void Gps_Msg_Show(void)
{
  float tp;

  tp=gpsx.longitude;	   
  sprintf((char *)dtbuf,"%.5f %1c",tp/=100000,gpsx.ewhemi);	//得到经度字符串
  OLED_ShowString(0,0,dtbuf);	 	   
  tp=gpsx.latitude;	   
  sprintf((char *)dtbuf,"%.5f %1c",tp/=100000,gpsx.nshemi);	//得到纬度字符串
  OLED_ShowString(0,2,dtbuf);

  sprintf((char *)dtbuf,"%02d/%02d",gpsx.posslnum,gpsx.svnum); //用于定位的GPS卫星数/可见卫星数
  OLED_ShowString(88,6,dtbuf);    
  
  sprintf((char *)dtbuf,"%04d/%02d/%02d",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//显示UTC日期
  OLED_ShowString(0,4,dtbuf);	
  sprintf((char *)dtbuf,"%02d:%02d:%02d",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//显示UTC时间
  OLED_ShowString(0,6,dtbuf);			  
}

void main(void)        
{ 
  
  u8 err=0;

  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
  
//  disableInterrupts();
  enableInterrupts();  //开启中断  

  UART2_Config(115200);   //9600bps 
  TIM1_Config();        //接收完成定时器

  OLED_Init();	       //初始化OLED  
  OLED_Clear();

  if(SIM800_Send_CMD("AT","OK")) err|=1<<0;    //	
  if(SIM800_Send_CMD("ATE0","OK")) err|=1<<1;  //关回显
  if(SIM800_Send_CMD("AT+CGPSPWR=1","OK")) err|=1<<2; //打开GPS电源
  if(SIM800_Send_CMD("AT+CGPSOUT=255","OK")) err|=1<<3; //打开NMEA数据输出
  OLED_ShowNum(112,0,err,2,16);
  
  while(1)
  {
    delay(1000);
    if(UART_RX_STA==1)
    {
      OLED_ShowNum(96,2,UART_RX_NUM,4,16);  //显示字符串长度	
      GPS_Analysis(&gpsx,(u8*)UART_RX_BUF); //分析字符串
      Gps_Msg_Show();		            //显示信息	

      UART_RX_STA=0;
      UART_RX_NUM=0;  
      memset(UART_RX_BUF,'\0',sizeof(UART_RX_BUF)); //清空
    }  
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
