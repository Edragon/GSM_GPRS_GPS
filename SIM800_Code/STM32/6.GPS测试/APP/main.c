#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "gps.h"
#include "string.h"    
#include "usart2.h" 
#include "sim800c.h"

u8 USART1_TX_BUF[USART2_MAX_RECV_LEN]; 					//串口1,发送缓存区
nmea_msg gpsx; 											//GPS信息
__align(4) u8 dtbuf[50];   								//打印缓存器

void Gps_Msg_Show(void);

int main(void)
{
	u16 i,rxlen;
	u8 err=0;
	
	NVIC_Configuration(); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	USART2_Init(115200);	//与GSM模块通信
	OLED_Init();
	OLED_Clear();

	USART2_RX_STA=0;	
	if(sim800c_send_cmd("AT","OK",200)) err|=1<<0;    //	
	if(sim800c_send_cmd("ATE0","OK",200)) err|=1<<1;  //关回显
	if(sim800c_send_cmd("AT+CGPSPWR=1","OK",200)) err|=1<<2; //打开GPS电源
	if(sim800c_send_cmd("AT+CGPSOUT=255","OK",200)) err|=1<<3; //打开NMEA数据输出
	OLED_ShowNum(112,0,err,2,16);
	
  while(1)
  {
		delay_ms(1);
		if(USART2_RX_STA&0X8000)		//接收到一次数据了
		{
			rxlen=USART2_RX_STA&0X7FFF;	//得到数据长度
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART2_RX_BUF[i];	   
 			USART2_RX_STA=0;		   	//启动下一次接收
			USART1_TX_BUF[i]=0;			//自动添加结束符
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//分析字符串
			Gps_Msg_Show();				//显示信息	
			#if 0
			printf("\r\n%s\r\n",USART1_TX_BUF);//发送接收到的数据到串口1
			#endif
 		}
  }
}

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
