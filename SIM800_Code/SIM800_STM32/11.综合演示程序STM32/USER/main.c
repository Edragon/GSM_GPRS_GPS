/**********************************************************************************
 * 工程名  :GSM
 * 描述    :通过串口测试模块所有功能
 * 实验平台:STM32F10X
 * 库版本  :

**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"


#define Buf1_Max 200 					  //串口1缓存长度
#define Buf2_Max 200 					  //串口2缓存长度

/*************  本地变量声明	**************/
u16 USART_RX_STA;
u8 Uart1_Buf[Buf1_Max];//串口1数据缓存区
u8 Uart2_Buf[Buf2_Max];//串口2数据缓存区
u8 First_Int = 0,shijian=0;
u16 Times=0;      //延时变量
u8 Time_UART1=0;  //串口1计时器
u8 Time_UART2=0;  //串口2计时器
u8 Time_miao=0;

u8 Timer0_start=0;	//定时器0延时启动计数器
u8 Uart2_Start=0;	  //串口2开始接收数据
u8 Uart2_End=0;	    //串口2接收数据结束

/*************	本地函数声明	**************/
void GPIO_config(void);//GPIO端口配置
void Timer0Init(void);//定时器0初始化
void CLR_Buf2(void);//清除串口2接收缓存
u8 GSM_send_cmd(u8 *cmd,u8 *ack,u8 wait_time);//模块指令发送函数
void GSM_test(void);//模块测试主程序
void GSM_mtest(void);//模块信息检测
u8 GSM_gsminfo(void);//GSM信息显示(信号质量,电池电量,日期时间)
u8 GSM_call_test(void);//拨号测试程序
u8 GSM_sms_test(void);//短信测试程序
u8 GSM_gprs_test(void);//GPRS数据传输测试程序
u8 change_str_Data(u8 *p,u8 len);//字符转整型
void change_Data_str(int n, char str[]);//整型转字符
void Swap(char *ch1, char *ch2);
void change_hex_str(u8 dest[],u8 src[],u8 len);//十六进制转字符串
u8 GSM_jz_test(void);//基站定位
u8 GSM_tts_test(void);//TTS文本语音测试程序

/*************  外部函数和变量声明*****************/




/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与GSM模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
int main(void)
{
	SysTick_Init_Config();
	GPIO_Config();
	
	USART1_Init_Config(115200);
	USART2_Init_Config(115200);
	Timer2_Init_Config();
	UART1_SendString("GSM模块短信测试程序\r\n");//打印信息
	UART1_SendString("GSM模块在注册网络\r\n");
  GSM_test(); //GSM测试程序
	while(1)
	{
	}
}
/*******************************************************************************
* 函数名  : USART1_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 1)、只启动了USART1中断接收，未启动USART1中断发送。
*           2)、接收到0x0d 0x0a(回车、"\r\n")代表帧数据接收完成
*******************************************************************************/
void USART1_IRQHandler(void)                	
{
		u8 res;

		res=USART_ReceiveData(USART1);
		Time_UART1=0;
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					Uart1_Buf[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(Buf1_Max-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  

} 
/*******************************************************************************
* 函数名  : USART2_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
			u8 Res=0;
      Time_UART2=0;
			Res= USART_ReceiveData(USART2);		//将接收到的字符串存到缓存中
			Uart2_Buf[First_Int]= Res; 
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}
} 	

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3中断
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 

		if(flag)
		{
			LED4_ON(); 
			flag=0;
		}
		else
		{
			LED4_OFF(); 
			flag=1;
		}
		if(Time_miao>=1)Time_miao++;//Time_miao==1时启动计数，溢出自动停止
		if(Timer0_start)
		Times++;
		if(Times > (shijian))
		{
			Timer0_start = 0;
			Times = 0;
		}
		Time_UART2++;
		if(USART_RX_STA&(~0x8000))//正在接收状态中
		{
			Time_UART1++;
			if(Time_UART1>=200)USART_RX_STA=0;//接收超时，复位接收
		}
	}	
}

/*******************************************************************************
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}
/*******************************************************************************
* 函数名 : GSM_send_cmd
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 0:正常  1:错误
* 注意   : 
*******************************************************************************/

u8 GSM_send_cmd(u8 *cmd,u8 *ack,u8 wait_time)         
{
	u8 res=1;
	//u8 *c;
	//c = cmd;										//保存字符串地址到c
	CLR_Buf2(); 
	for (; *cmd!='\0';cmd++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
	  USART_SendData(USART2,*cmd);
	}
	UART2_SendLR();	
	if(wait_time==0)return res;
	Times = 0;
	shijian = wait_time;
	Timer0_start = 1;
	while(Timer0_start&res)                    
	{
		if(strstr((const char*)Uart2_Buf,(char*)ack)==NULL)
			 res=1;
		else
		{
			 res=0;
		}

	}
	return res;
}
/*******************************************************************************
* 函数名 : GSM_test
* 描述   : GSM主测试程序
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void GSM_test(void)
{
	u8 sim_ready=0;
	while(GSM_send_cmd((u8*)"AT",(u8*)"OK",5))//查询是否应到AT指令
	{
		UART1_SendString("未检测到模块\r\n");
		Delay_nMs(800);
		UART1_SendString("正在尝试连接\r\n");
		Delay_nMs(400);  
	} 	 
	GSM_send_cmd((u8*)"ATE0",(u8*)"OK",200);//不回显
	GSM_mtest();
	if(GSM_gsminfo()==0)
	{
		sim_ready=1;
		UART1_SendString("请选择:选择相应的中文+回车，再发送\r\n"); 				    	 
		UART1_SendString("拨号\t"); 				    	 
		UART1_SendString("短信\t");				    	 
		UART1_SendString("GPRS\t");
		UART1_SendString("定位\t");
		UART1_SendString("语音\r\n");
	}
	while(1)
	{
		if(sim_ready)
		{
			if(USART_RX_STA&0x8000)
			{
				USART_RX_STA=0;
				if(strstr((char*)Uart1_Buf,"拨号"))
					GSM_call_test();	//电话测试
				else
				if(strstr((char*)Uart1_Buf,"短信"))
					GSM_sms_test();		//短信测试
				else
				if(strstr((char*)Uart1_Buf,"GPRS"))
					GSM_gprs_test();	//GPRS测试
				else
				if(strstr((char*)Uart1_Buf,"定位"))
				  GSM_jz_test();		//基站测试
				else				if(strstr((char*)Uart1_Buf,"语音"))
					GSM_tts_test();		//语音测试
				
				UART1_SendString("请选择:选择相应的中文+回车，再发送\r\n"); 				    	 
				UART1_SendString("拨号\t"); 				    	 
				UART1_SendString("短信\t");				    	 
				UART1_SendString("GPRS\t");
				UART1_SendString("定位\t");
				UART1_SendString("语音\r\n");
			}
			
		}			
	} 	
}
/*******************************************************************************
* 函数名 : GSM_mtest
* 描述   : GSM/GPRS主测试控制部分
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void GSM_mtest()
{
	u8 *p1; 
	UART1_SendString("\r\nNiRen-Elec GSM/GPRS 测试程序\r\n");  
	CLR_Buf2(); 
	if(GSM_send_cmd((u8*)"AT+CGMI",(u8*)"OK",5)==0)//查询制造商名称
	{ 
		UART1_SendString("制造商:");
		p1=(u8*)strstr((const char*)(Uart2_Buf+2),"\r\n");
		UART1_Send_Len((char*)Uart2_Buf+2,p1-Uart2_Buf);
		CLR_Buf2(); 		
	} 
	if(GSM_send_cmd("AT+CGMM","OK",5)==0)//查询模块名字
	{ 
		UART1_SendString("模块型号:");
		p1=(u8*)strstr((const char*)(Uart2_Buf+2),"\r\n"); 
		UART1_Send_Len((char*)Uart2_Buf+2,p1-Uart2_Buf);
		CLR_Buf2();
	} 
	if(GSM_send_cmd("AT+CGSN","OK",5)==0)//查询产品序列号
	{ 
		UART1_SendString("产品序列号:");
		p1=(u8*)strstr((const char*)(Uart2_Buf+2),"\r\n"); 
		UART1_Send_Len((char*)Uart2_Buf+2,p1-Uart2_Buf);
		CLR_Buf2();		
	}
	if(GSM_send_cmd("AT+CNUM","+CNUM",2)==0)//查询本机号码
	{ 
		u8 *p2;
		UART1_SendString("本机号码:");
		p1=(u8*)strstr((const char*)(Uart2_Buf),"\""); 
		p2=(u8*)strstr((const char*)(p1+1),"\"");
    p1=(u8*)strstr((const char*)(p2+1),"\"");
		UART1_Send_Len((char*)(p1+1),11);
		CLR_Buf2();		
	}
	UART1_SendLR();
}

/*******************************************************************************
* 函数名 : GSM_gsminfo_gsminfo
* 描述   : GSM/GPRS状态信息检测(信号质量,电池电量,日期时间)
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 GSM_gsminfo()
{
	u8 *p1;
	u8 *p2;
	u8 res=0;
	CLR_Buf2();
	if(GSM_send_cmd("AT+CPIN?","OK",3))
	{
    UART1_SendString("请检查手机卡是否有插入\r\n");
		res|=1<<0;	//查询SIM卡是否在位 
	}
	CLR_Buf2(); 
	
	if(GSM_send_cmd("AT+COPS?","OK",3)==0)		//查询运营商名字
	{ 
		p1=(u8*)strstr((const char*)(Uart2_Buf),"\""); 
		if(p1)//有有效数据
		{
			p2=(u8*)strstr((const char*)(p1+1),"\"");				
			UART1_SendString("运营商:");
			UART1_Send_Len((char*)p1+1,p2-p1-1);
			UART1_SendLR();
		}
	}else res|=1<<1;
	CLR_Buf2();
 
	if(GSM_send_cmd("AT+CSQ","OK",3)==0)		//查询信号质量
	{ 
		p1=(u8*)strstr((const char*)(Uart2_Buf),":");
		if(p1)
		{
			p2=(u8*)strstr((const char*)(p1+1),",");
			UART1_SendString("信号质量:");
			UART1_Send_Len((char*)p1+2,p2-p1-2);
			UART1_SendLR();
		}		
	}else res|=1<<2;

  if(GSM_send_cmd("AT+DDET=1","OK",3)==0)
	{
		UART1_SendString("模块支持DTMF音频解码，可以直接识别对方通话过程中按下的按键");
    UART1_SendLR();	
	}else
	{
    UART1_SendString("模块不支持DTMF音频解码，不可以识别对方通话过程中按下的按键");
    UART1_SendLR();	
	}
	if(GSM_send_cmd("AT+CTTS=?","OK",3)==0)
	{
		UART1_SendString("模块支持TTS本地语音，可以直接将文本播放成语音");
    UART1_SendLR();	
	}
	else
	{
		UART1_SendString("模块不支持TTS本地语音");
    UART1_SendLR();			
	}
	if(GSM_send_cmd("AT+CIPGSMLOC=?","OK",3)==0)
	{
		UART1_SendString("模块支持基站定位，可以直接获取定位信息");
    UART1_SendLR();	
	}
	else
	{
		UART1_SendString("模块不支持基站定位");
    UART1_SendLR();			
	}
	CLR_Buf2();
	
	return res;
}
/*******************************************************************************
* 函数名 : GSM_call_test
* 描述   : 拨号测试代码
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 GSM_call_test()
{	
	u8 temp[50];
	u16 len=0;
	u16 i=0;
	u8 mode=0;
	u8 flag=0;
  u8 *p1=NULL;
	UART1_SendString("@@@@@@@@@@@拨号测试@@@@@@@@@@@\r\n");
	UART1_SendString("拨打电话：输入“致电xxxx+回车”\r\n");
	UART1_SendString("接听电话：输入“接听+回车”\r\n");
  UART1_SendString("挂断电话：输入“挂断+回车”\r\n");
	UART1_SendString("DTMF语音：输入“单个字符+回车”\r\n");
	UART1_SendString("退出测试：输入“退出+回车”\r\n");
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{
			len=USART_RX_STA&0X3FFF;
			USART_RX_STA=0;
			if(strstr((const char*)Uart1_Buf,"退出"))return 0;
			if(strstr((const char*)Uart1_Buf,"接听"))mode=1;
			else
			if(strstr((const char*)Uart1_Buf,"挂断"))mode=2;
			else
			if(strstr((const char*)Uart1_Buf,"致电"))mode=3;
			else
			if(mode==0)mode=0;//无效
			else 
			{
				mode=4;
				flag=1;
			}
		}
		switch(mode)
		{
			case 0://有来电
				if(strstr((const char*)Uart2_Buf,"RING"))
				{
					CLR_Buf2();
					UART1_SendString("有来电\r\n");
				}
				break;
			case 1:
						 if(GSM_send_cmd("ATA","OK",2)==0)//接听
						 {
							UART1_SendString("接听成功\r\n");
						  mode=4; 
						 }
				break;
			case 2:if(GSM_send_cmd("ATH","OK",2)==0)//挂断
							UART1_SendString("挂断成功\r\n");		
						 mode=0;
				break;
			case 3://拨号
						UART1_SendString("致电:");
						UART1_Send_Len((char*)Uart1_Buf+4,len-4);
						strcpy((char*)temp,"ATD");
						for(i=3;i<(len-4+3);i++)
						temp[i]=Uart1_Buf[i+1];
						temp[i++]=';';
						temp[i++]='\0';
						if(GSM_send_cmd(temp,"OK",8)==0)
						{
							UART1_SendString("拨叫成功\r\n");
							mode=4;
						}
						else
						{
							UART1_SendString("请重新呼叫\r\n");
						  mode=0;
						}
			  break;
			case 4://发送DTMF
				if(flag)
				{
				flag=0;
				strcpy((char*)temp,"AT+VTS=");
				temp[7]=Uart1_Buf[0];
				temp[8]='\0';
				GSM_send_cmd(temp,"OK",3);
				}
				if(strstr((const char*)Uart2_Buf,"DTMF:"))
				{
          Delay_nMs(10);
					p1=(u8*)strstr((const char*)(Uart2_Buf),":");
					UART1_SendString("对方按下按键：");
					UART1_Data(*(p1+2));
					UART1_SendLR();	
					CLR_Buf2();
				}
				break;
		}
		
	}
}
/*******************************************************************************
* 函数名 : GSM_sms_test
* 描述   : 短信测试代码
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 GSM_sms_test()
{
	u16 len=0;
	u8 mode=0;
	u8 temp[50];
	u8 *p,*p1;
	u8 loc=0;
	UART1_SendString("@@@@@@@@@@@@@@@@@@@@@@@@@短信测试@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
	UART1_SendString("设置中心号：如：设置+8613800756500，必需设置好中心号才能发送短信\r\n");
	UART1_SendString("发送短信：请按要求输入电话+内容(非中文)，以回车结束:如137xxxx+niren-electron\r\n");
  UART1_SendString("查看短信：查看：xx，xx代表短信的存储位置\r\n");
	UART1_SendString("退出测试：输入“退出+回车”\r\n");
	GSM_send_cmd("AT+CMGF=1","OK",2);//设置短信发送模式
  GSM_send_cmd("AT+CNMI=3,1,0,0,0","OK",2);//设置短信上报模式，上报位置
  GSM_send_cmd("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);		//所有操作在SIM卡中进行
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{
			len=USART_RX_STA&0X3FFF;
			if(len>Buf2_Max-2)len=Buf2_Max-2;
			USART_RX_STA=0;
			if(strstr((const char*)Uart1_Buf,"退出"))return 0;
			if(strstr((const char*)Uart1_Buf,"设置"))mode=3;//设置短信中心号
			else
			if(strstr((const char*)Uart1_Buf,"查看"))mode=2;//查看短信
			else 
      if(strstr((const char*)Uart1_Buf,"+"))mode=1;//发送短信
			else
			  mode=0;

		}
		switch(mode)
		{
			case 0:
        if(strstr((const char*)Uart2_Buf,"+CMTI:"))
				{
          UART1_SendString("有新短信\r\n");
					UART1_SendString("短信位置: ");
					p=(u8*)strstr((const char*)Uart2_Buf,",");
					UART1_SendString((char*)p+1);
					CLR_Buf2();
				}
				break;
			case 1:
				UART1_SendString("发送短信\r\n");
				Uart1_Buf[len]='\0';//添加结束符;
				strcpy((char*)temp,(const char *)"AT+CMGS=\"+86");
				loc=sizeof("AT+CMGS=\"+86");
				p=(u8*)strstr((char*)Uart1_Buf,(char*)"+");//查找发送内容
				Uart1_Buf[(p-Uart1_Buf)]='\0';//添加结束符
				strcpy((char*)(&temp[loc-1]),(const char*)Uart1_Buf);
				loc=strlen((const char*)temp);
			  temp[loc]='\"';
				temp[loc+1]='\0';
				GSM_send_cmd(temp,">",2);//发送接收方号码
        strcpy((char*)&temp,(const char*)(p+1));
			  loc=strlen((const char*)temp);
				temp[loc]='\32';
				temp[loc+1]='\0';
			  if(GSM_send_cmd(temp,"OK",20)==0)//发送短信内容
					UART1_SendString("发送短信成功\r\n");
        else
					UART1_SendString("发送短信失败\r\n");
        mode=0;
			break;
			case 2:
				UART1_SendString("查看短信\r\n");
        Uart1_Buf[len]='\0';//添加结束符;
				p=(u8*)strstr((const char*)Uart1_Buf,"：");
				strcpy((char*)temp,(const char*)"AT+CMGR=");
			  loc=strlen((const char*)temp);
				strcpy((char*)(&temp[loc]),(const char*)(p+2));
				loc=strlen((const char*)temp);
			  if(GSM_send_cmd(temp,"OK",5)==0)
				{
					p=(u8*)strstr((const char*)(Uart2_Buf+2),"\r\n");
					p1=(u8*)strstr((const char*)(p+2),"\r\n");
					loc=p1-p;
					UART1_SendString("短信内容：");
					UART1_Send_Len((char*)p+2,loc);
				}
        CLR_Buf2();
				mode=0;
				break;
			case 3:
				UART1_SendString("设置中心号\r\n");
				Uart1_Buf[len]='\0';//添加结束符;
				strcpy((char*)temp,(const char *)"AT+CSCA=");
			  loc=sizeof("AT+CSCA=");
				strcpy((char*)(&temp[loc-1]),(const char*)&Uart1_Buf[4]);
				if(GSM_send_cmd(temp,"OK",3)==0)
					UART1_SendString("短信中心号设置成功\r\n");
				else
					UART1_SendString("短信中心号设置成功\r\n");
				mode=0;
				break;
		}
	}
}

/*******************************************************************************
* 函数名 : GSM_gprs_test
* 描述   : GPRS测试代码
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   :  为了保持连接，每空闲隔10秒发送一次心跳
*******************************************************************************/
u8 GSM_gprs_test()
{	
  u16 len=0;
	u8 mode=0;
	u8 temp[200];
	u8 flag=0;
	u8 *p1,*p2;
	UART1_SendString("@@@@@@@@@@@@@@GPRS测试@@@@@@@@@@@@@@@@@@@@@\r\n");
	UART1_SendString("设置参数：设置\"模式\",\"IP\",端口+回车\r\n如：设置\"TCP\",\"125.89.18.79\",12345\r\n");
	UART1_SendString("发送内容：发送+内容+回车\r\n如：发送泥人电子\r\n");
	UART1_SendString("退出测试：退出+回车\r\n");
	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{
			len=USART_RX_STA&0X3FFF;
			if(len>Buf2_Max-2)len=Buf2_Max-2;
			USART_RX_STA=0;
			if(strstr((const char*)Uart1_Buf,"退出"))mode=4;//退出
			else
			if(strstr((const char*)Uart1_Buf,"设置"))mode=1;//设置配置
			else
			if(strstr((const char*)Uart1_Buf,"发送"))mode=2;//发送内容
			else 												mode=0;			
		}
		else
		{
			if(strstr((const char*)Uart2_Buf,"CONNECT OK")&&(flag==1))//连接成功
			{
					UART1_SendString("连接成功\r\n");
					Time_miao=1;
					flag=2;
					mode=3;
					CLR_Buf2();
			}else
			if((flag==1)&&((strstr((const char*)Uart2_Buf,"CLOSED"))&&(Time_miao>10)))//连接失败或超时
			{
					UART1_SendString("连接失败\r\n");
					
				  while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
					USART_SendData(USART1 ,Time_miao);//发送当前字符
				
					Time_miao=0;
					flag=0;
					mode=1; //重新连接
					CLR_Buf2();
			}	
			if((flag==2)&&(Time_miao>10))//每10秒一个心跳包
			{
				Time_miao=1;
				mode=3;
			}
	  }
    switch(mode)
		{
			case 0:        
						if(strstr((const char*)Uart2_Buf,"+IPD"))//判断上位机是否有数据下发
						{
							UART1_SendString("有新内容\r\n");
							p1=(u8*)strstr((const char*)Uart2_Buf,",");
							p2=(u8*)strstr((const char*)Uart2_Buf,":");
							UART1_Send_Len((char*)(p2+1),change_str_Data((p1+1),(p2-p1-1)));
							UART1_SendLR();
							CLR_Buf2();
						}
			break;
			case 1: 
				     if(strstr((const char*)Uart1_Buf,"TCP")||strstr((const char*)Uart1_Buf,"UDP"))
						 {
                strcpy((char*)&temp,(const char*)"AT+CIPSTART=");
							  memcpy((char*)&temp[12],(const char*)(&Uart1_Buf[4]),len-4);
							  temp[len+12]='\0';
							  GSM_send_cmd("AT+CIPCLOSE=1","CLOSE OK",2);	//关闭连接
								GSM_send_cmd("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
 							  GSM_send_cmd("AT+CGCLASS=\"B\"","OK",2);//设置GPRS移动台类别为B,支持包交换和数据交换 
								GSM_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//设置PDP上下文,互联网接协议,接入点等信息
								GSM_send_cmd("AT+CGATT=1","OK",2);//附着GPRS业务
								GSM_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",2);//设置为GPRS连接模式
								GSM_send_cmd("AT+CIPHEAD=1","OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
                GSM_send_cmd("AT+CIPMUX=0","OK",2);//设置单路连接
                if(GSM_send_cmd(temp,"OK",2 )==0)//发起连接
								{
                  UART1_SendString("正在发起连接\r\n");
									Time_miao=1;
									flag=1;
								}
								else
								{
									UART1_SendString("输入错误，请重输\r\n");
									Time_miao=0;
									flag=0;
								}
						 }
						 else
						 {
							  UART1_SendString("输入错误，请重输\r\n");
						 }
             mode=0;
			break;
			case 2:
              if(flag==2)
							{
                UART1_SendString("开始发送...........\r\n");
								if(GSM_send_cmd("AT+CIPSEND",">",2)==0)
								{
										 Uart1_Buf[len]='\32';
										 Uart1_Buf[len+1]='\0';
										 if(GSM_send_cmd(&Uart1_Buf[4],"SEND OK",8)==0)
										 { 								
													UART1_SendString("发送成功\r\n");
													Time_miao=1;
										 }
										 else
											 UART1_SendString("发送失败\r\n");
										 
								}else
								{
										 UART2_Data(0X1B);//ESC,取消发送
                     UART1_SendString("发送失败\r\n");
								}
						  }else UART1_SendString("还没连接\r\n");
							mode=0;
			break;
			case 3:
						if(GSM_send_cmd("AT+CIPSEND",">",2)==0)
						{
							UART2_Data(0x00);
							UART2_Data(0X1A);//CTRL+Z,结束数据发送,启动一次传输								
							UART1_SendString("心跳成功\r\n");

								 
						}else
						{
								 UART2_Data(0X1B);//ESC,取消发送
				         UART1_SendString("心跳失败\r\n");
						}
				mode=0;
			break;
			case 4:
						GSM_send_cmd("AT+CIPCLOSE=1","CLOSE OK",5);	//关闭连接
						GSM_send_cmd("AT+CIPSHUT","SHUT OK",5);		//关闭移动场景
						return 1;
		}      
	}
}
/*******************************************************************************
* 函数名 : change_str_Data
* 描述   : 字符串转整型
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 change_str_Data(u8 *p,u8 len)
{
  u8 i=0;
	u8 value=0;
	for(i=0;i<len;i++)
	{
    value=value*10;
		value+=(*(p++)-'0');
	}
	return value;
}
/*******************************************************************************
* 函数名 : Swap
* 描述   : 交换
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Swap(char *ch1, char *ch2)
{
	char tmp = *ch1;
	*ch1 = *ch2;
	*ch2 = tmp;
}
/*******************************************************************************
* 函数名 : Convert
* 描述   : 转化成整型
* 输入   : n:要转换的整型数据 ,str:保存的字符
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void change_Data_str(int n, char str[])
{
	int i, len;
	for(i = 0; n != 0; ++i)
	{
		str[i] = n % 10 + '0';
		n /= 10;
	}
	str[i] = '\0';
	len = i;
	/* 翻转 */
	for(i = 0; i < len/2; ++i)
		Swap(str+i, str+len-i-1);
	str[len] = '\0';
}
/*******************************************************************************
* 函数名 : GSM_JZ_test
* 描述   : 基站定位测试代码
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
/*
u8 GSM_jz_test()
{
  u8 *p1,*p2,*p3;
	UART1_SendString("@@@@@@@@@@@@@@@@@@@@@基站定位测试@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
	UART1_SendString("退出测试：退出+回车\r\n");
	GSM_send_cmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"","OK",2);
	GSM_send_cmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"","OK",2);
  if(GSM_send_cmd("AT+SAPBR=1,1","OK",5))//激活
	{
		//sim900a_send_cmd("AT+SAPBR=0,1","OK",2);//关闭
		UART1_SendString("打开失败，请确认模块是否有该功能\r\n");
		return 1;		
	}
	if(GSM_send_cmd("AT+SAPBR=2,1","OK",5))
	{
    UART1_SendString("获取本地IP错误\r\n");
		return 2;
	}
  Time_miao=1;
	while(1)
	{
    if(USART_RX_STA&0x8000)
		{
			USART_RX_STA=0;
			if(strstr(Uart1_Buf,"退出"))//退出
			{
				 GSM_send_cmd("AT+SAPBR=0,1","OK",2);//关闭
				 return 0;
			}
			else
			UART1_SendString("输入错误\r\n");
		}
    if(GSM_send_cmd("AT+CIPGSMLOC=1,1","OK",10)==0)//获取经纬度和时间
		{
        Time_miao=1;
 			  p1=(u8*)strstr((const char*)(Uart2_Buf),",");
				p3=(u8*)strstr(p1,"\r\n");
				if(p1)//有效数据
				{	
					p2=(u8*)strtok((char*)(p1),",");
          UART1_SendString("经度:");UART1_SendString(p2);UART1_SendData('\t');
					
					p2=(u8*)strtok(NULL,",");
          UART1_SendString("纬度:");UART1_SendString(p2);UART1_SendData('\t');
					
					p2=(u8*)strtok(NULL,",");
          UART1_SendString("日期:");UART1_SendString(p2);UART1_SendData('\t');
					
					p2=(u8*)strtok(NULL,",");
					*p3='\0';//插入结束符
          UART1_SendString("时间:");UART1_SendString(p2);UART1_SendLR();
				}
		}
		if(Time_miao>11)
		{
      Time_miao=0;
			//sim900a_send_cmd("AT+SAPBR=0,1","OK",2);//关闭
			UART1_SendString("打开失败，请确认模块是否有该功能\r\n");
			return 1;
		}
	}
}

*/

/*******************************************************************************
* 函数名 : change_hex_str
* 描述   : 十六进制转换成字符串 如0xAC->'A''C';
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void change_hex_str(u8 dest[],u8 src[],u8 len)
{
    u8 i=0;
    u8 temp_h;
    u8 temp_l;
    for(i=0;i<len;i++)
    {
        temp_h=src[i]>>4;
        temp_l=src[i]&0X0F;
        if(temp_h>9)
            dest[2*i]=(src[i]>>4)+55;
        else
            dest[2*i]=(src[i]>>4)+48;
        if(temp_l>9)
            dest[2*i+1]=(src[i]&0X0F)+55;
        else
            dest[2*i+1]=(src[i]&0X0F)+48;
    }
		dest[2*len]='\0';
}
/*******************************************************************************
* 函数名 : GSM_jz_test
* 描述   : 基站定位测试代码
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 GSM_jz_test()
{
  u8 *p1,*p2,*p3;
	UART1_SendString("@@@@@@@@@@@@@@@@@@@@@基站定位测试@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
	UART1_SendString("退出测试：退出+回车\r\n");
	GSM_send_cmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"","OK",2);
	GSM_send_cmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"","OK",2);
  if(GSM_send_cmd("AT+SAPBR=1,1","OK",5))//激活
	{
	
		UART1_SendString("打开失败，请确认模块是否有该功能\r\n");
		return 1;		
	}
	if(GSM_send_cmd("AT+SAPBR=2,1","OK",5))
	{
    UART1_SendString("获取本地IP错误\r\n");
		return 2;
	}
  Time_miao=1;
	while(1)
	{
    if(USART_RX_STA&0x8000)
		{
			USART_RX_STA=0;
			if(strstr((const char*)Uart1_Buf,"退出"))//退出
			{
				 GSM_send_cmd("AT+SAPBR=0,1","OK",2);//关闭
				 return 0;
			}
			else
			UART1_SendString("输入错误\r\n");
		}
    if(GSM_send_cmd("AT+CIPGSMLOC=1,1","OK",10)==0)//获取经纬度和时间
		{
        Time_miao=1;
 			  p1=(u8*)strstr((const char*)(Uart2_Buf),",");
				p3=(u8*)strstr((const char*)p1,"\r\n");
				if(p1)//有效数据
				{	
					p2=(u8*)strtok((char*)(p1),",");
          UART1_SendString("经度:");UART1_SendString((char *)p2);UART1_Data('\t');
					
					p2=(u8*)strtok(NULL,",");
          UART1_SendString("纬度:");UART1_SendString((char *)p2);UART1_Data('\t');
					
					p2=(u8*)strtok(NULL,",");
          UART1_SendString("日期:");UART1_SendString((char *)p2);UART1_Data('\t');
					
					p2=(u8*)strtok(NULL,",");
					*p3='\0';//插入结束符
          UART1_SendString("时间:");UART1_SendString((char *)p2);UART1_SendLR();
				}
		}
		if(Time_miao>11)
		{
      Time_miao=0;
			//sim900a_send_cmd("AT+SAPBR=0,1","OK",2);//关闭
			UART1_SendString("打开失败，请确认模块是否有该功能\r\n");
			return 1;
		}
	}
}
#define MAXlen 200 //定义最多播放的字节数
/*******************************************************************************
* 函数名 : GSM_tts_test
* 描述   : TTS文本语音测试代码
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 GSM_tts_test()
{
  u16 len=0;
  u8 temp_src[]="你好感谢你购买泥人电子GSM开发板";
	u8 temp[MAXlen+15];
	u8 loc=0;
	UART1_SendString("@@@@@@@@@@@@@@@@@@@@@TTS文本语音测试@@@@@@@@@@@@@@@@@@@@@@@\r\n");
	UART1_SendString("退出测试：退出+回车\r\n");
	UART1_SendString("语音播放：直接输入播放的内容+回车\r\n");
  UART1_SendString("注意：最大支持100个字节\r\n");
	if(GSM_send_cmd("AT+CTTS=?","OK",2))
	{
		 UART1_SendString("错误：请查询模块是否具备该功能\r\n");
		 return 1;
	}
	strcpy((char*)&temp,(const char*)"AT+CTTS=2,\"");
	loc=sizeof("AT+CTTS=2,\"");
  len=strlen((const char*)temp_src);
	strcpy((char*)&temp[loc-1],(const char*)temp_src);
	temp[loc+len-1]='\"';
	temp[loc+len]='\0';
  GSM_send_cmd(temp,"OK",3);
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{
      len=USART_RX_STA&0X3FFF;
			if(len>(MAXlen/2))len=(MAXlen/2);
      USART_RX_STA=0;
			if((len==4)&&(strstr((const char*)Uart1_Buf,"退出")))//退出	
			return 0;
			UART2_SendString("AT+CTTS=2,\"");
			/*
      UART2_SendData('A');
			UART2_SendData('T');
			UART2_SendData('+');
			UART2_SendData('C');
			UART2_SendData('T');
			UART2_SendData('T');
			UART2_SendData('S');
			UART2_SendData('=');
			UART2_SendData('2');
			UART2_SendData(',');
			UART2_SendData('\"');
      */
			Uart1_Buf[len]='\"';
			Uart1_Buf[len+1]='\0';
			if(GSM_send_cmd(Uart1_Buf,"OK",2))
				UART1_SendString("等待上一帧数据播放完毕\r\n");
		}
	}
}
