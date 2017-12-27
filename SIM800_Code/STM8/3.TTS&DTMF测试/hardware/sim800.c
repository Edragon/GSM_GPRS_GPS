//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//diy Audio
//技术支持:aemails@163.com
//版权所有，盗版必究。
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#include "stm8s.h"
#include "string.h"
#include "stdio.h"
#include "sim800.h"
#include "oled.h"

extern u8 UART_RX_BUF[300];
extern u16 UART_RX_NUM;
extern u8 UART_RX_STA;


void delay(u16 t)
{
  u8 i=0;
  for(;t>0;t--)
  {
    for(i=10;i>0;i--)
      asm("nop"); 
  }
}

u8 SIM800_Send_CMD(u8 *cmd,u8 *ack)
{
  u8 res=1; 
  
  UART_RX_STA=0;
  UART_RX_NUM=0;  
  memset(UART_RX_BUF,'\0',sizeof(UART_RX_BUF)); //清空数组

  printf("%s\r",cmd); //发送命令

  while(UART_RX_STA==0);  //等待接收完成     
  if(ack)	      //应答校验
  { 
    if(strstr((char*)UART_RX_BUF,(char*)ack))
      res=0;                //校验成功
    else
      res=1;
  }
  return res;
}

void SIM800_Init(void)
{ 
  u8 flag=0;   //错误标志
  
  if(SIM800_Send_CMD("ATE0","OK")) flag|=1<<0;          //关回显

  if(SIM800_Send_CMD("AT+CMGF=1","OK")) flag|=1<<1;     //短信 text模式  

  if(SIM800_Send_CMD("AT+CLIP=1","OK")) flag|=1<<2;     //来电显示
  
  if(SIM800_Send_CMD("AT+COLP=1","OK")) flag|=1<<3;     //被叫显示
  
  if(SIM800_Send_CMD("AT+DDET=1,0,0","OK")) flag|=1<<4; //开启DTMF  

  if(SIM800_Send_CMD("AT+CTTSPARAM=50,0,50,55,1","OK")) flag|=1<<5; //TTS设置
                         // 音量 模式 音调 速度 通道

  OLED_ShowNum(110,2,flag,2,16);  
  
  SIM800_Send_CMD( "ATD18127870064;" ,""); //拨号

}

void test(void)
{
  char *res,i=0;
  
  if(UART_RX_STA==1)
  {
    if((res=strstr((char*)UART_RX_BUF,"+CLIP:")))   //接到来电
    {
      for(i=0;i<11;i++)   //+CLIP: "18127870064",161,"",0,"",0
      {
        OLED_ShowChar(40+8*i,6,res[8+i]);
      }
      SIM800_Send_CMD("ATA","OK");
      delay(6000);
      delay(6000);  
      SIM800_Send_CMD("AT+CTTS=2,\"欢迎使用远程控制，打开LED请按1，关闭LED请按2\"","OK");
    }
    else if((res=strstr((char*)UART_RX_BUF,"+COLP:")))  //被叫接通
    {
      for(i=0;i<11;i++)     //+COLP: "18127870064",129,"",0,""
      {
        OLED_ShowChar(40+8*i,6,res[8+i]);
      }
      delay(6000);  
      SIM800_Send_CMD("AT+CTTS=2,\"欢迎使用远程控制，打开LED请按1，关闭LED请按2\"","OK");
    }
    else if((res=strstr((char*)UART_RX_BUF,"+DTMF:")))
    {
      OLED_ShowChar(48,4,res[7]);        
      if(res[7]=='1')
      {
        SIM800_Send_CMD("AT+CTTS=0","OK");  //停止播放
        SIM800_Send_CMD("AT+CTTS=2,\"LED已打开\"","OK");
        GPIO_WriteHigh(LED_PORT,LED_PIN );     
        OLED_ShowString(40,2,"ON ");
      }
      else if(res[7]=='2')
      {
        SIM800_Send_CMD("AT+CTTS=0","OK");  //停止播放
        SIM800_Send_CMD("AT+CTTS=2,\"LED已关闭\"","OK");
        GPIO_WriteLow(LED_PORT,LED_PIN );  
        OLED_ShowString(40,2,"OFF");
      }
      else
      {
        SIM800_Send_CMD("AT+CTTS=0","OK");  //停止播放
        SIM800_Send_CMD("AT+CTTS=2,\"无效按键\"","OK"); 
      }
    }
    else if(strstr((char*)UART_RX_BUF,"NO CARRIER"))   //挂电话
    {
      OLED_ShowChar(48,4,' ');  
      OLED_ShowString(40,6,"            "); 
    }
    
    UART_RX_STA=0;
    UART_RX_NUM=0;  
    memset(UART_RX_BUF,'\0',sizeof(UART_RX_BUF)); //
  }
}
