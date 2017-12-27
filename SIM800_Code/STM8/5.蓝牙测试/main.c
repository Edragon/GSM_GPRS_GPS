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

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)        
{ 
  
  u8 err=0;
  u8 bl_status=0;
  u8 *p1,*p2;
//  u8 sendbuf[20];

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
  OLED_ShowNum(112,0,err,2,16);  
  SIM800_Send_CMD("AT+BTPOWER=1",""); //打开蓝牙电源
  SIM800_Send_CMD("AT+BTUNPAIR=0",""); //删除所有已配对设备
  SIM800_Send_CMD("AT+BTSCAN=1,50","OK");  //开始搜索50S  
  OLED_ShowString(0,0,"Scaning..");  
  while(1)
  {
    delay(500);
    if(UART_RX_STA==1)
    {
      if(bl_status==0)   //未连接成功
      {
        if(strstr((char*)UART_RX_BUF,"+BTSCAN: 0,1"))  //搜索到蓝牙设备
        {
          SIM800_Send_CMD("AT+BTPAIR=0,1","OK");    //主动请求配对
        }
        else if(strstr((char*)UART_RX_BUF,"+BTPAIRING: "))   //收到配对请求
        {
          SIM800_Send_CMD("AT+BTPAIR=1,1","OK"); //应答配对
        }
        else if(strstr((char*)UART_RX_BUF,"+BTPAIR: 1"))     //配对成功
        {
          OLED_ShowString(0,0,"Paired   "); 
        }
        else if(strstr((char*)UART_RX_BUF,"+BTCONNECT: 1"))  //连接成功
        {
          bl_status=1;
          OLED_ShowString(0,0,"Connected"); 
        }
        else if(strstr((char*)UART_RX_BUF,"+BTDISCONN: "))   //断开连接 
        {
          bl_status=0;
          OLED_ShowString(0,0,"DisConect");
        }	
      }
      else 
      {
        if(strstr((char*)UART_RX_BUF,"+BTCONNECTING: "))  //收到手机端SPP连接请求
        {
          if(SIM800_Send_CMD("AT+BTACPT=1","+BTCONNECT: "))//应答手机端spp连接请求 7S
          {
            OLED_ShowString(0,0,"SPP Connect Fail"); 
          }else
          {
            OLED_ShowString(0,0,"SPP Connected   "); 
            SIM800_Send_CMD("AT+BTSPPSEND",">");
            SIM800_Send_CMD("SIM800 Bluetooth SPP test\r\n\x1A","OK");//发送数据
          }
        }
        else if(p1=(u8*)strstr((char*)UART_RX_BUF,"+BTSPPDATA: ")) //接收到数据
        {
          p2 = (u8*)strstr((const char *)p1,"\x0d\x0a");  //+BTSPPDATA: 2,6,ryjjgf
          p2[0]=0;//添加结束符
          p2 =(u8*)strstr((const char *)p1,",");
          p1 =(u8*)strstr((const char *)p2+1,",");
          OLED_ShowString(0,2,"                ");
          OLED_ShowString(0,4,"                ");
          OLED_ShowString(0,6,"                ");
          OLED_ShowString(0,2,(u8*)(p1+1)); 
        }
        else if(strstr((char*)UART_RX_BUF,"+BTDISCONN:"))   //断开连接 
        {
          bl_status=0;
          OLED_Clear();
          OLED_ShowString(0,0,"DisConect");
        }
      }

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
