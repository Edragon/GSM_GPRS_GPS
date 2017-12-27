/**********************************************************************************
 * 工程名  :
 * 描述    :通过手机蓝牙控制LED
						通过STC开发板串口2控制模块打开蓝牙，并将连接过程发送到串口1；当与手机
						建立蓝牙SPP连接后，会定时发送固定数据给手机蓝牙串口，手机蓝牙发送的数据
						会自动转发到串口1。
						运行程序前，建议先打开我们配套的手机蓝牙串口助手。
 * 实验平台:STC12XX
 * 库版本  :

 * 软件功能说明
   
**********************************************************************************/

#include "string.h"
#include "delay.h"
#include "uart.h"


#define Buf1_Max 60 					  //串口1缓存长度
#define Buf2_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/
sbit RUNING_LED = P1^0;					//运行指示灯
sbit LED  			= P1^1;					//控制指示灯

const char *sendtable="Niren-elec Bluetooth test\r\n\32";

/*************  本地变量声明	**************/
xdata u8 Uart1_Buf[Buf1_Max];
xdata u8 Uart2_Buf[Buf2_Max];
char *p1,*p2; 

u16 Times=0;
u8 First_Int = 0,shijian=0;
u8 Timer_send;//定时发送计时器
//u8 Time_count=0;
bdata u8 Flag;//定时器标志位
sbit Timer0_start =Flag^0;	//定时器0延时启动计数器
sbit Uart2_Start  =Flag^1;	//串口2开始接收数据
sbit Uart2_End		=Flag^2;	//串口2接收数据结束


/*************	本地函数声明	**************/
void GPIO_config(void);
void Timer0Init(void);
void CLR_Buf2(void);
u8 Find(u8 *a);
void Second_AT_Command(u8 *b,char *a,u8 wait_time);


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
void main(void)
{
	GPIO_config();
	Uart1Init();
	Uart2Init();
	Timer0Init();
	EA=1;	//开总中断
	UART1_SendString("SIM800模块蓝牙测试程序\r\n");
	UART1_SendString("请打开手机蓝牙串口助手\r\n");
	UART1_SendString("SIM800模块打开蓝牙功能\r\n");
    Second_AT_Command("ATE1\r\n","AT",2);//打开回显
	Second_AT_Command("AT+BTPOWER=1\r\n","AT",2);//打开蓝牙电源,这个不判断OK，因为电源原本开启再发送打开的话会返回错误
	delay_ms(100); 
    UART1_SendString("等待手机APP连接模块\r\n");
    while(1)
    {
        if(strstr((const char*)Uart2_Buf,"BTPAIRING")!=NULL)//请求配对
        {
            CLR_Buf2();
            Second_AT_Command("AT+BTPAIR=1,1\r\n","OK",3);//允许手机配对
            while(strstr((const char*)Uart2_Buf,"BTCONNECTING")==NULL);   //等待配对模块  
            break;
        }else
        if(strstr((const char*)Uart2_Buf,"BTCONNECTING")!=NULL)//请求链接
        {
            break;
        }
    } 
    Second_AT_Command("AT+BTACPT=1\r\n","OK",3);//响应连接
	UART1_SendString("SIM800模块蓝牙建立SPP服务成功\r\n");
	CLR_Buf2(); 
	while(1)
	{
		//接收到数据
		if((p1=(char*)strstr((const char*)Uart2_Buf,"DATA:")),(p1!=NULL))//寻找开始符
		{		
				if((p2=(char*)strstr((const char*)p1,"\x0d\x0a")),(p2!=NULL))//寻找结束符
				{
					char *p3;
					*p2=0;//添加结束符
					p3=strstr((const char*)p1,",");//收索第一个','
					p1=strstr((const char*)p3+1,",");//收索第二个','
					UART1_SendString("接收到的内容:");
					UART1_SendString(p1+1);
					UART1_SendLR();
					CLR_Buf2(); 
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
        RI = 0;                 //清除RI位
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
    }
}

/*******************************************************************************
* 函数名 : Uart2
* 描述   : 串口2中断服务入口函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Uart2() interrupt 8
{
		IE2  &= ~0x01;   //关闭串口2中断
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //清除S2RI位
			Uart2_Buf[First_Int] = S2BUF;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //清除S2TI位
    }
		IE2  |= 0x01;   //使能串口2中断
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
	TF0 = 0;		//清除TF0标志
	
	Time_count++;
	if(Time_count>=50)
	{
		Time_count = 0;
		RUNING_LED =~RUNING_LED;
	}
	if(Timer0_start)
	Times++;
	if(Times > (u16)(50*shijian))
	{
		Timer0_start = 0;
		Times = 0;
	}
	Timer_send++;
	
	TL0 = 0x00;		//设置定时器初值
	TH0 = 0xB8;		//设置定时器初值
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
		P3M1 &= 0XC3;  //配置P32~P35为推挽输出
		P3M0 |=	~0XC3;
		LED=0;
		RUNING_LED=0;
}

void Timer0Init(void)		//20毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//12T模式
	TMOD &= 0xF0;		//
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时器初值
	TH0 = 0xB8;		//设置定时器初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;    	//使能定时器0中断
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

u8 Find(u8 *a)
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

void Second_AT_Command(u8 *b,char *a,u8 wait_time)         
{
	u8 i;
	u8 *c;
	c = b;										//保存字符串地址到c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				CLR_Buf2(); 
				b = c;							//将字符串地址给b
				for (b; *b!='\0';b++)
				{
					UART2_SendData(*b);
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
}

