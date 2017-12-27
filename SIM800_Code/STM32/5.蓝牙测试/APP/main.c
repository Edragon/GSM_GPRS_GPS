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
	u8 *p1,*p2;
	u8 bl_status=0;
	u8 sendbuf[20];
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	USART2_Init(115200);	//与GSM模块通信
	OLED_Init();
	OLED_Clear();

	sim800c_init();

	if(sim800c_send_cmd("AT+BTPOWER=1","",200));  //打开蓝牙电源
	delay_ms(5);
	sim800c_send_cmd("AT+BTUNPAIR=0","",200);     //删除所有已配对设备
	delay_ms(5);
	sim800c_send_cmd("AT+BTSCAN=1,50","OK",200);  //开始搜索50S  
	OLED_ShowString(0,6,"Scaning.."); 
  while(1)
  {
		if(USART2_RX_STA&0X8000)		//接收到数据
		{
			if(bl_status==0)   //未连接成功
			{
				if(sim800c_check_cmd("+BTSCAN: 0,1"))   //搜索到蓝牙设备
				{
					sim800c_send_cmd("AT+BTPAIR=0,1","OK",200); //主动请求配对
				}
				else if(sim800c_check_cmd("+BTPAIRING: "))   //收到配对请求
				{
					sim800c_send_cmd("AT+BTPAIR=1,1","OK",200); //应答配对
				}
				else if(sim800c_check_cmd("+BTPAIR: 1"))     //配对成功
				{
					OLED_ShowString(0,6,"Paired   "); 
				}
				else if(sim800c_check_cmd("+BTCONNECT: 1"))  //连接成功
				{
					bl_status=1;
					OLED_ShowString(0,6,"Connected"); 
				}
				else if(sim800c_check_cmd("+BTDISCONN: "))   //断开连接 
				{
					bl_status=0;
					OLED_ShowString(0,6,"DisConect");
				}	
			}else 
			{
				if(sim800c_check_cmd("+BTCONNECTING: "))  //收到手机端SPP连接请求
				{
					if(sim800c_send_cmd("AT+BTACPT=1","+BTCONNECT: ",700))//应答手机端spp连接请求 7S
					{
						OLED_Clear();
						OLED_ShowString(0,0,"SPP Connect Fail"); 
					}else
					{
						OLED_Clear();
						OLED_ShowString(0,0,"SPP Connected"); 
						sim800c_send_cmd("AT+BTSPPSEND",">",100);//发送数据
						sprintf((char*)sendbuf,"%s \r\n\x1A","SIM800C Bluetooth SPP test"); //
						sim800c_send_cmd((u8*)sendbuf,"OK",100);//发送数据
					}
				}
				else if(sim800c_check_cmd("+BTSPPDATA: ")) //接收到数据
				{
					p1=sim800c_check_cmd("+BTSPPDATA: ");   //+BTSPPDATA: 2,6,ryjjgf
					p2 = (u8*)strstr((const char *)p1,"\x0d\x0a"); 
					p2[0]=0;//添加结束符
					p2 =(u8*)strstr((const char *)p1,",");
					p1 =(u8*)strstr((const char *)p2+1,",");
					//printf("%s",p1+1);//打印到串口
					OLED_ShowString(0,2,"                ");
					OLED_ShowString(0,4,"                ");
					OLED_ShowString(0,6,"                ");
					OLED_ShowString(0,2,(u8*)(p1+1)); 
				}
				else if(sim800c_check_cmd("+BTDISCONN:"))   //断开连接 
				{
					bl_status=0;
					OLED_Clear();
					OLED_ShowString(0,6,"DisConect");
				}
			}
			USART2_RX_STA=0;			
		}
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
