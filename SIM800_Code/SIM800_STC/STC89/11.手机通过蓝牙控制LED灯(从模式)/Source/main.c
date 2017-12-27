/**********************************************************************************
 * 工程名  :蓝牙控制LED
 * 描述    :通过STM32开发板串口1控制模块打开蓝牙；
						当与手机建立蓝牙SPP连接后，通过我们配套的手机APP，控制LED的亮灭；
						运行程序前，建议先打开我们配套的手机蓝牙串口助手。
 * 实验平台:C51

 * 硬件连接说明
	 使用单片串口与GPRS模块通信 
	 C51        GPRS模块
	 P30 (RXD)->RXD
	 P31 (TXD)->TXD
	 GND	    ->GND

 * 软件功能说明
   打开我们配套的手机APP，切换模式到监控站模式，通过“按键1”就可以控制LED的亮灭。
**********************************************************************************/
#include "config.h"
#include "string.h"
#include "delay.h"
#include "uart.h"

#define Buf1_Max 200 					  //串口1缓存长度
/*************	本地常量声明	**************/
sbit RUNING_LED = P1^0;					//运行指示灯
sbit LED  			= P1^1;					//控制指示灯
sbit LED1       = P1^4;
sbit LED2       = P1^5;
sbit LED3       = P1^6;
sbit LED4       = P1^7;


/*************  本地变量声明	**************/
xdata u8 Uart1_Buf[Buf1_Max];

u16 Times=0;
u8 First_Int = 0,shijian=0;
u8 Timer_send=0;
char *p1,*p2;
bdata u8 Flag;//定时器标志位
sbit Timer0_start =Flag^0;	//定时器0延时启动计数器

const char *sendtable="Niren-elec Bluetooth LED control\r\n\32";

/*************	本地函数声明	**************/
void GPIO_config(void);   //引脚初始化
void Timer0Init(void);    //定时器0初始化
void CLR_Buf1(void);      //清串口接收缓存
u8 Find(u8 *a);           //查找字符串
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time);//发送指令



/*************  外部函数和变量声明*****************/




/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口波特率是9600，GPRS模块默认波特率是115200，需要自己通过串口助手修改
				   为9600方可使用。 
					 程序会删除手机卡上的短信，请注意
*******************************************************************************/
void main(void)
{
	Timer0Init();  //初始化定时器0
	GPIO_config();
	EA=1;	//开总中断
	Uart1Init();    //初始化串口9600
    Second_AT_Command("ATE1\r\n","AT",2);//打开回显
	Second_AT_Command("AT+BTPOWER=1\r\n","AT",2);//打开蓝牙电源,这个不判断OK，因为电源原本开启再发送打开的话会返回错误
	delay_ms(100);
	LED1=0;
     while(1)
    {
        if(strstr((const char*)Uart1_Buf,"BTPAIRING")!=NULL)//请求配对
        {
            CLR_Buf1();
            Second_AT_Command("AT+BTPAIR=1,1\r\n","OK",3);//允许手机配对
            while(strstr((const char*)Uart1_Buf,"BTCONNECTING")==NULL);   //等待配对模块  
            break;
        }else
        if(strstr((const char*)Uart1_Buf,"BTCONNECTING")!=NULL)//请求链接
        {
            break;
        }
    } 
	LED3=0;
    Second_AT_Command("AT+BTACPT=1\r\n","OK",3);//响应连接
	CLR_Buf1();
	LED4=0;
	while(1)
	{
		//接收到数据
		if((p1=(char*)strstr((const char*)Uart1_Buf,"DATA:")),(p1!=NULL))//寻找开始符
		{		
				if((p2=(char*)strstr((const char*)p1,"\x0d\x0a")),(p2!=NULL))//寻找结束符
				{
					char *p3;
					*p2=0;//添加结束符
					p3=strstr((const char*)p1,",");//收索第一个','
					p1=strstr((const char*)p3+1,",");//收索第二个','
					//判断按键
					if(*(p1+4)==(char)0XB1)
					{
						LED =~LED;
					}

					CLR_Buf1(); 
				}
		}else
		if(Timer_send>100)//空闲时定时发送数据
		{	
			Second_AT_Command("AT+BTSPPSEND",">",1);
			Second_AT_Command((char *)sendtable,"OK",1);
			Timer_send=0;
		}
	}
	
}

/*******************************************************************************
* 函数名 : Uart1 
* 描述   : 串口1中断服务入口函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Uart1() interrupt 4
{
    if (RI)
    {
      RI = 0;                           //清除RI位
			Uart1_Buf[First_Int] = SBUF;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf1_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                          //清除TI位
    }
}

/*******************************************************************************
* 函数名 : Timer0_ISR
* 描述   : 定时器0中断服务入口函数,20ms中断一次
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Timer0_ISR() interrupt 1
{
	static u8 Time_count=0; 
	TR0=0;//关定时器
	TL0 = 0x00;		//重设定时器初值
	TH0 = 0xB8;		//重设定时器初值
	Time_count++;
	if(Time_count>=50)
	{
		Time_count = 0;
		RUNING_LED =~RUNING_LED;
	}
	if(count_20ms) //20ms延时计数器
		count_20ms--;
	if(Timer0_start)
	  Times++;
	if(Times > (u16)(50*shijian))
	{
		Timer0_start = 0;
		Times = 0;
	}
	Timer_send++;
	TR0=1;//开定时器
}
/*******************************************************************************
* 函数名 : GPIO_config
* 描述   : IO口配置函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void	GPIO_config(void)
{
		P1=0XFF;
		LED=1;
		RUNING_LED=1;
}
/*******************************************************************************
* 函数名 : Timer0Init
* 描述   : 定时器0初始化，20ms定时
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Timer0Init(void)		//20毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//
	TMOD |= 0x01;		//设置定时器模式，16位定时器
	TL0 = 0x00;		  //设置定时器初值
	TH0 = 0xB8;		  //设置定时器初值
	TF0 = 0;		    //清TF0标志
	TR0 = 1;		    //定时器0开始计时
	ET0 = 1;    	  //使能定时器0中断
}
/*******************************************************************************
* 函数名 : CLR_Buf1
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //将缓存内容清零
	{
		Uart1_Buf[k] = 0x00;
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

u8 Find(u8 *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、希望收到的应答、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(u8 *b,u8 *a,u8 wait_time)         
{
	u8 i;
	u8 *c;
	c = b;										//保存字符串地址到c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a))            //查找需要应答的字符
		{
			if(Timer0_start == 0)
			{
				CLR_Buf1(); 
				b = c;							//将字符串地址给b
				for (b; *b!='\0';b++)
				{
					UART1_SendData(*b);
				}
				UART1_SendLR();	
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
	
}
