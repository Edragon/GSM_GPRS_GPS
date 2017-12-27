/**********************************************************************************
 * 工程名  :
 * 描述    :通过手机通话控制LED
 * 实验平台:STM32F10X
 * 库版本  :

**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"

#define Automatic_Startup 1     //定义自启动 V1.2版本起有自启动功能

//#define Buf1_Max 60 					  //串口1缓存长度
#define Buf2_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/


/*************  本地变量声明	**************/
//char Uart1_Buf[Buf1_Max];
char Uart2_Buf[Buf2_Max];

u8 Times=0,First_Int = 0,shijian=0;

vu8 Timer0_start;	//定时器0延时启动计数器
vu8 Uart2_Start;	//串口2开始接收数据
vu8 Uart2_End;	  //串口2接收数据结束


/*************	本地函数声明	**************/
void CLR_Buf2(void);
u8 Find(char *a);
void Second_AT_Command(char *b,u8 wait_time);
void Set_TTS_Mode(void);
void Check_New_Message(void);
void Wait_CREG(void);

/*************  外部函数和变量声明*****************/




/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与GPRS模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
int main(void)
{
	SysTick_Init_Config();
	GPIO_Config();
	
	USART1_Init_Config(115200);
	USART2_Init_Config(115200);
	Timer2_Init_Config();
	UART1_SendString("GPRS模块语音台程序\r\n");
	UART1_SendString("GPRS模块在注册网络\r\n");
	Wait_CREG();
	UART1_SendString("GPRS模块注册成功\r\n");
	UART1_SendString("GPRS模块TTS，DTMF设置\r\n");
	Set_TTS_Mode();
	UART1_SendString("设置成功\r\n");
	//Second_AT_Command("AT+CMGD=1,4",3);//删除卡上所有短信
	
	UART1_SendString("初始化完成\r\n");
	while(1)
	{
		Check_New_Message();
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
			Res =USART_ReceiveData(USART2);
			Uart2_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
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
		
		if(Timer0_start)
		Times++;
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
	
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
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(char *b,u8 wait_time)         
{
	u8 i;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find("OK")) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//将字符串地址给b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					USART_SendData(USART2,*b);//UART2_SendData(*b);
				}
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 
}

/*******************************************************************************
* 函数名 : Set_TTS_Mode
* 描述   : 设置TTS和DTMF模式
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_TTS_Mode(void)
{
	Second_AT_Command("ATE0",3);										//取消回显	
	Second_AT_Command("AT+CTTSPARAM=100,0,50,50,1",3);				    //设置TTS参数
	Second_AT_Command("AT+DDET=1",3);								    //打开DTMF解码
    //Second_AT_Command("ATS0=1",3);                                      //设置自动接听
}

/*******************************************************************************
* 函数名 : Check_Message_rec
* 描述   : 检查是否有新信息，并执行信息内容指令
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Check_New_Message(void)
{
    static u8 Ring_flag=0;//来电标志
    
    if(Ring_flag==0)
    {
        if(strstr(Uart2_Buf,"RING")!=NULL)   		//若缓存字符串中含有"+CMT"就表示有新的短信
        {
            Second_AT_Command("ATA",3);             //接听电话
            UART1_SendString("新来电\r\n");
            Ring_flag=1;
            Second_AT_Command("AT+CTTS=2,\"欢迎使用泥人电子语音测试程序\"",3); //播放语音测试
            CLR_Buf2();
        }
    }else
    {
        if(strstr(Uart2_Buf,"+DTMF:")!=NULL)   		//有按键按下
        {
            UART1_SendString("有按键按下\r\n");
            if(strstr(Uart2_Buf,"1")!=NULL)//打开第一路
            {
                LED1_ON();
                Second_AT_Command("AT+CTTS=2,\"打开第一路\"",3); //播放语音测试
                UART1_SendString("打开第一路\r\n");
            }else
            if(strstr(Uart2_Buf,"2")!=NULL)//关闭第一路
            {
                LED1_OFF();
                Second_AT_Command("AT+CTTS=2,\"关闭第一路\"",3); //播放语音测试
                UART1_SendString("关闭第一路\r\n");
            }else
            if(strstr(Uart2_Buf,"3")!=NULL)//打开第二路
            {
                LED2_ON();
                Second_AT_Command("AT+CTTS=2,\"打开第二路\"",3); //播放语音测试
                UART1_SendString("打开第二路\r\n");
            }else
            if(strstr(Uart2_Buf,"4")!=NULL)//关闭第二路
            {
                LED2_OFF();
                Second_AT_Command("AT+CTTS=2,\"关闭第二路\"",3); //播放语音测试
                UART1_SendString("关闭第二路\r\n");
            }
            CLR_Buf2();
        }else
        if(strstr(Uart2_Buf,"NO CARRIER")!=NULL)
        {
            UART1_SendString("挂断\r\n");
            Ring_flag=0;
            CLR_Buf2();
        }
    }
        

    
}

/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf2();
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT+CREG?");
		UART2_SendLR();
		Delay_nMs(5000);  						
	    for(k=0;k<Buf2_Max;k++)      			
    	{
			if(Uart2_Buf[k] == ':')
			{
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5'))
				{
					i = 1;
					UART1_SendLR();
				  break;
				}
			}
		}
		UART1_SendString("注册中.....");
	}
}
