/**********************************************************************************

 * 软件功能说明
   板子上电后运行指示灯RUNING_LED会以一秒的频率闪烁
	 服务器发送“onled”点亮LED；
   服务器发送“offled”熄灭LED；
	 单片机每隔10秒会向服务器发送心跳帧“OK”
**********************************************************************************/
#include "config.h"
#include "string.h"
#include "delay.h"
#include "uart.h"

#define Buf1_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/

static unsigned char *phone="\"17086558896\"";     //短信接收方手机号  ，实际应用中只需要改这个号码即可
/*************  本地变量声明	**************/
xdata u8 Uart1_Buf[Buf1_Max];
xdata unsigned char content[100]="UNV-SIM808-GPS-TEST";//发送短信内容
u8 Times=0,First_Int = 0,shijian=0;
u8 key_buf=0;
char *p2;
//u8 Time_count=0;
bdata u8 Flag;//定时器标志位
sbit Timer0_start =Flag^0;	//定时器0延时启动计数器
sbit Heart_beat		=Flag^1;	//发送心跳帧标志位
sbit KEY        = P3^5;         //按键		,低电平触发
sbit RUNING_LED =P1^0;


/*************	本地函数声明	**************/
void GPIO_config(void);
void Timer0Init(void);
void CLR_Buf2(void);
u8 Find(u8 *a);
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time);
void Wait_CREG(void);

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
	if(Time_count>=50)//1秒运行指示灯闪烁
	{
		Time_count = 0;
		RUNING_LED =~RUNING_LED;
	}
	if(count_20ms) //20ms延时计数器
		count_20ms--;

	if(Timer0_start)
	Times++;
	if(Times > (50*shijian))
	{
		Timer0_start = 0;
		Times = 0;
	}
	TR0=1;//开定时器
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
    First_Int = 0;             //接收字符串的起始存储位置
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
	CLR_Buf1();
 	while(i == 0)        			
	{
		CLR_Buf1();        
		UART1_SendString("AT+CREG?");
		UART1_SendLR();
		delay_ms(250);  						
	    for(k=0;k<Buf1_Max;k++)      			
    	{
			if(Uart1_Buf[k] == ':')
			{
				if((Uart1_Buf[k+4] == '1')||(Uart1_Buf[k+4] == '5'))
				{
					i = 1;
				  break;
				}
			}
		}
	}
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
* 输入   : 发送数据的指针、发送等待时间(单位：S)
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
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
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
	//CLR_Buf1(); 
}

void Set_Text_Mode(void)
{
	//unsigned char temp[25]="AT+CSCA=";
	Second_AT_Command("ATE1","OK",3);									//取消回显	
	Second_AT_Command("AT+CMGF=1","OK",3);								//TEXT模式	
	Second_AT_Command("AT+CSCS=\"GSM\"","OK",3);								
	Second_AT_Command("AT+CSMP=17,167,0,241","OK",3);		
	Second_AT_Command("AT+CSCA?","OK",3);//设置短信中心号
}

void Send_Text_Sms(u8 *temp_data)
{
	unsigned char temp[50]="AT+CMGS=";
	
	strcat(temp,phone); //字符串拼接函数（库函数）
	Second_AT_Command(temp,">",3); //等待接收到“>”才发送短信内容
	UART1_SendString(temp_data);     //发送短信内容
	UART1_SendData(0X1A);          //发送结束符	
}
u8 GPS_Datt_handle(void)
{	
	content[0]=0;//添加字符串结束符
	strcat(content,"GPS data:");
	Second_AT_Command("AT+CGNSINF\r\n","OK",3);
	if((p2=(char*)strstr(Uart1_Buf,"OK")),(p2!=NULL))//寻找结束符
	{
		*p2=0;//添加结束符
		p2=strtok(Uart1_Buf,",");
		if(p2==NULL) return 1;
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 2;
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 3;
		//UART1_SendString("时间:");UART1_SendString((char *)p2);UART1_SendLR();
		//strcat(content,"TIME:");
		//strcat(content,(char *)p2);
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 4;
		//UART1_SendString("纬度:");UART1_SendString((char *)p2);UART1_SendLR();
		strcat(content,"latitude:N");
		strcat(content,(char *)p2);
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 5;
		//UART1_SendString("经度:");UART1_SendString((char *)p2);UART1_SendLR();
		strcat(content,"longitude:E");
		strcat(content,(char *)p2);
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 6;
		//UART1_SendString("海拔:");UART1_SendString((char *)p2);UART1_SendLR();
		strcat(content,"elevation:");
		strcat(content,(char *)p2);//UART1_SendString((char *)content);							
		//delay_ms(1000); 
		CLR_Buf1();	
		return 0;
	}
	return 7;
}
 /*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口波特率是9600，GPRS模块默认波特率是115200，需要自己通过串口助手修改
				   为9600方可使用。
*******************************************************************************/
void main(void)
{	
	Timer0Init();  //初始化定时器0
	EA=1;	//开总中断
	Uart1Init();    //初始化串口9600

	Wait_CREG();    //查询等待模块注册成功
	Set_Text_Mode();//设置短信为TEXT模式		 
	
	Second_AT_Command("AT+CGNSPWR=1\r\n","OK",2);   //打开GPS电源
//	delay_ms(100); 
//	Second_AT_Command("AT+CGNSSEQ=\"RMC\"\r\n","OK",2);//定义NMEA解析
//	Second_AT_Command("AT+CGNSINF\r\n","+CGNSINF: 1,1,",3);//等待定位成功
//	Second_AT_Command("AT+CGNSURC=0\r\n","OK",1); //设置GPS输出参数
//	CLR_Buf1();
	Send_Text_Sms("UNV-SIM808-TEST");//发送问候信息
	while(1)
	{
		if(!KEY)delay_ms(10);
		{
			if((KEY==0))//按键按下
			{ 								
			 	if(!GPS_Datt_handle())
					Send_Text_Sms(content);//发送定位信息				
			}
		}		
	}
}


