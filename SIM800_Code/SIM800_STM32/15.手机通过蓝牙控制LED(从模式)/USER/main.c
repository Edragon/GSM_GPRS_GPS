/**********************************************************************************
 * 工程名  :蓝牙功能测试程序
 * 描述    :通过STM32开发板串口2控制模块打开蓝牙，并将连接过程发送到串口1；当与手机
						建立蓝牙SPP连接后，会定时发送固定数据给手机蓝牙串口，手机蓝牙发送的数据
						会自动转发到串口1。
						运行程序前，建议先打开我们配套的手机蓝牙串口助手。
 * 实验平台:STM32F10X
 * 库版本  :


 * 硬件连接说明
	 使用单片串口2与GPRS模块通信  注：使用串口2可以避免下载和通信不会冲突
	 STM32      GPRS模块
	 PA3 (RXD2)->RXD
	 PA2 (TXD2)->TXD
	 GND	      ->GND

 * 软件功能说明
   打开我们配套的手机APP，切换模式到监控站模式，通过“按键1”和“按键2”就可以控制LED的亮灭。
**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"

#define Buf2_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/
const char *sendtable="Niren-elec Bluetooth test\r\n\32";
/*************  本地变量声明	**************/

char Uart2_Buf[Buf2_Max]; //串口2接收缓存

char *p1,*p2; 

vu8 Times=0,First_Int = 0,shijian=0;

vu8 Timer0_start;	//定时器0延时启动计数器

vu8 Timer_send;//定时发送计时器


/*************	本地函数声明	**************/
void System_Initialization(void); //STM32系统初始化
void Ram_Initialization(void);    //内存变量初始化
void NVIC_Configuration(void);    //中断向量组初始化
void CLR_Buf2(void);              //清串口2接收缓存
u8 Find(char *a);                 //查找字符串
void Second_AT_Command(char *b,char *a,u8 wait_time);  //发送AT指令



/*************  外部函数和变量声明*****************/




/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
int main(void)
{
	System_Initialization(); //系统初始化
	Ram_Initialization();    //变量初始化
    LED4_OFF();
    
	UART1_SendString("SIM800模块蓝牙测试程序\r\n");
	UART1_SendString("请打开手机蓝牙串口助手\r\n");
	UART1_SendString("SIM800模块打开蓝牙功能\r\n");
    Second_AT_Command("ATE1\r\n","AT",2);//打开回显
	Second_AT_Command("AT+BTPOWER=1\r\n","AT",2);//打开蓝牙电源,这个不判断OK，因为电源原本开启再发送打开的话会返回错误
	Delay_nMs(100); 
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
					UART1_SendString("接收到的数据:");
					UART1_SendString(p1+1);
					UART1_SendLR();
                    if(*(p1+4)==(char)0xB1)//手机APP 按按键1
					{
						LED4_ON();
					}
					if(*(p1+4)==(char)0xB2)//按按键2
					{
						LED4_OFF();
					}
					CLR_Buf2(); 
				}
		}else
		if(Timer_send>5)//空闲时定时发送数据
		{	
			Second_AT_Command("AT+BTSPPSEND",">",1);
			Second_AT_Command((char *)sendtable,"OK",1);
			Timer_send=0;
		}
		
	}
	
	
}

/*******************************************************************************
* 函数名  : System_Initialization
* 描述    : STM32系统初始化函数(初始化STM32时钟及外设)
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void System_Initialization(void)
{
	RCC_Configuration();		//设置系统时钟为72MHZ(这个可以根据需要改)
  SysTick_Init_Config();  //初始化系统滴答时钟SysTick
  NVIC_Configuration();		//STM32中断向量表配配置
	Timer2_Init_Config();		//定时器2初始化配置
	USART1_Init_Config(115200);	//串口1初始化配置
	USART2_Init_Config(115200);	//串口2初始化配置	
	GPIO_Config();          //初始化GPIO
}
/*******************************************************************************
* 函数名  : Ram_Initialization
* 描述    : 变量初始化函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Ram_Initialization(void)
{
	Times=0;
	shijian=0;
	Timer0_start=0;
	Timer_send=0;
}

/*******************************************************************************
* 函数名  : NVIC_Configuration
* 描述    : STM32中断向量表配配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 设置KEY1(PC11)的中断优先组
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;						//定义NVIC初始化结构体

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				    //设置中断优先级组为2，先占优先级和从优先级各两位(可设0～3)
	
	/*定时器2中断向量配置*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				    //设置中断向量号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//设置抢先优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
	NVIC_Init(&NVIC_InitStructure);
	/*串口1中断向量配置*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				  //设置中断向量号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//设置抢先优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
	NVIC_Init(&NVIC_InitStructure);
  /*串口2中断向量配置*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;				  //设置中断向量号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//设置抢先优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
	NVIC_Init(&NVIC_InitStructure);
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
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
		{
			Res =USART_ReceiveData(USART2);
			Uart2_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			} 
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
void TIM2_IRQHandler(void)   //TIM2中断
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
			//LED4_ON(); 
			flag=0;
		}
		else
		{
			//LED4_OFF(); 
			flag=1;
		}
		Timer_send++;
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
	First_Int=0;
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

void Second_AT_Command(char *b,char *a,u8 wait_time)         
{
	u8 i;
	char *c;
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
				//UART1_SendString(b);
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
	//UART1_SendString(Uart2_Buf); 
}

