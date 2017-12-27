#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "sim800c.h"
#include "string.h"    
#include "usart2.h" 


//短信类型
typedef enum {EN = 0, CN = 1} MsgType;
          // 18127870064            //unicode码:18127870064
u8 *NUM[2]={"18127870064","00310038003100320037003800370030003000360034"}; 
                                                  //unicode码:SIM800接收到短信
u8 *Msg[2]={"sim800 received your message","00530049004D00380030003063A56536523077ED4FE1"};  

void sim800c_init(void);
void sim800c_sendmsg( u8 *Msg ,MsgType type);

int main(void)
{

	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	USART2_Init(115200);	//与GSM模块通信
	OLED_Init();
	OLED_Clear();

	sim800c_init();

  while(1)
  {
		if(USART2_RX_STA&0X8000)		
		{
			if(sim800c_check_cmd("+CMT:"))  //接收到短信
			{                               //+CMT: "18127870064","","16/11/25,10:27:31+32"
				OLED_ShowString(0,6,"receive message");
				sim800c_sendmsg(Msg[CN] ,CN);  
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

//Msg[]:短信内容
//type:  0->英文  1->中文
void sim800c_sendmsg( u8 *Msg ,MsgType type)
{
  u8 p[70]="";
	
	sim800c_send_cmd("AT+CMGF=1","OK",200);			//设置文本模式 
	sim800c_send_cmd("AT+CNMI=2,2","OK",200); 	//设置新消息提示
  if( type == EN )
  {
    sim800c_send_cmd("AT+CSCS=\"GSM\"","OK",200);     //设置GSM字符集
    sim800c_send_cmd("AT+CSMP=17,167,0,0","OK",200);	//设置短消息文本模式参数 
  }
  else if( type == CN ) 
  {
    sim800c_send_cmd("AT+CSCS=\"UCS2\"","OK",200);    //设置UCS2字符集
    sim800c_send_cmd("AT+CSMP=17,167,0,8","OK",200);  //设置短消息文本模式参数 
  }
	sprintf((char*)p,"AT+CMGS=\"%s\"",NUM[type]); 
	if(sim800c_send_cmd(p,">",200)==0)					//发送短信命令+电话号码
	{ 		 				 													 
		u2_printf("%s",Msg);		 						//发送短信内容到GSM模块
		delay_ms(90);                     //必须延时，否则不能发送短信						
		sim800c_send_cmd((u8*)0X1A,"+CMGS:",1000); //发送结束符,等待发送完成
	} 
}
