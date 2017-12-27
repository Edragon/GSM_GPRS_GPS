#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "sim800c.h"
#include "string.h"    
#include "usart2.h" 

void sim800c_init(void);

int main(void)
{
	u8 callbuf[]="18127870064"; //呼叫的号码
	u8 p[20],*p1,*p2;

	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	USART2_Init(115200);	//与GSM模块通信
	OLED_Init();
	OLED_Clear();

	sim800c_init();

	sim800c_send_cmd("AT+CLIP=1","OK",200);	//设置来电显示  
	sim800c_send_cmd("AT+COLP=1","OK",200);	//设置被叫号码显示
	u2_printf("ATD%s;\r\n",callbuf);//拨号	
	
  while(1)
  {
		if(USART2_RX_STA&0X8000)		//接收到数据
		{
			if(sim800c_check_cmd("+CLIP:"))//接收到来电
			{
				p1=sim800c_check_cmd("+CLIP:");  //+CLIP: "18127870064",161,"",0,"",0
				p1+=8;
				p2=(u8*)strstr((const char *)p1,"\"");
				p2[0]=0;//添加结束符 
				strcpy((char*)p,(char*)p1);
				OLED_ShowString(0,6,p);
				sim800c_send_cmd("ATA","OK",200);//发送应答指令
			}
			else if(sim800c_check_cmd("+COLP:"))//拨通电话
			{
				p1=sim800c_check_cmd("+COLP:");  //+COLP: "18127870064",129,"",0,""
				p1+=8;
				p2=(u8*)strstr((const char *)p1,"\"");
				p2[0]=0;//添加结束符 
				strcpy((char*)p,(char*)p1);
				OLED_ShowString(0,6,p);
			}
			else if(sim800c_check_cmd("NO CARRIER"))   //挂电话
			{
				OLED_ShowString(0,6,"            ");     //清空号码显示区域
			}
			USART2_RX_STA=0;
		}
    LED0=!LED0;
    delay_ms(1000);
  }
}

void sim800c_init(void)
{
	u8 p[50],*p1,*p2;
	u8 err=0;
	USART2_RX_STA=0;	
	if(sim800c_send_cmd("AT","OK",100))err|=1<<0;//检测是否应答AT指令 
	USART2_RX_STA=0;		
	if(sim800c_send_cmd("ATE0","OK",200))err|=1<<1;//不回显	
	USART2_RX_STA=0;
	if(sim800c_send_cmd("AT+CGMM","OK",200)==0)//查询模块名字
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n"); 
		p1[0]=0;//加入结束符
		sprintf((char*)p,"%s",USART2_RX_BUF+2);
		OLED_ShowString(0,0,p); 
		USART2_RX_STA=0;		
	}else err|=1<<2;
	if(sim800c_send_cmd("AT+CPIN?","OK",200))err|=1<<3;	//查询SIM卡是否在位 
	USART2_RX_STA=0;  
	if(sim800c_send_cmd("AT+COPS?","OK",200)==0)		//查询运营商名字
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),"\""); 
		if(p1)//有有效数据
		{
			p2=(u8*)strstr((const char*)(p1+1),"\"");
			p2[0]=0;//加入结束符			
			sprintf((char*)p,"%s",p1+1);
			OLED_ShowString(0,2,p);
		}else err|=1<<4; 
		USART2_RX_STA=0;		
	}else err|=1<<4;
	
	OLED_ShowString(0,4,"err=");
	OLED_ShowNum(32,4,err,2,16);
} 
