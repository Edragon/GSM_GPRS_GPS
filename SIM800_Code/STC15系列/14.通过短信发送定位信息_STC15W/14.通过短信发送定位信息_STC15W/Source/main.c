/**********************************************************************************
 * ������  :GPS��λ���ܲ��Գ���
 * ����    :ͨ��STC�����崮��2����ģ���GPS��������λ��Ϣ�����󷢵�����1
 * ʵ��ƽ̨:STC15XX
 * ��汾  :
 * ����    :
 * ����    :
 * �Ա�    :
**********************************************************************************/

#include "string.h"
#include "delay.h"
#include "uart.h"

#define S3RI  0x01              //S3CON.0
#define S3TI  0x02              //S3CON.1

#define Buf2_Max 200 					  //����2���泤��
/*************	���س�������	**************/
sbit RUNING_LED = P0^4;					//����ָʾ��
sbit LED  			= P0^5;					//����ָʾ��
sbit KEY        = P3^5;         //����
static unsigned char *SMS_Num="\"+8613800517500\"";//�������ĺ� 
static unsigned char *phone="\"13338915761\"";     //���Ž��շ��ֻ���
xdata unsigned char content[100]="GPS positioning success";//���Ͷ�������
/*************  ���ر�������	**************/
xdata u8 Uart2_Buf[Buf2_Max];
char *p1,*p2; 

u16 Times=0;
u8 First_Int = 0,shijian=0;
u8 key_buf=0;
//u8 Time_count=0;
bdata u8 Flag;//��ʱ����־λ
sbit Timer0_start =Flag^0;	//��ʱ��0��ʱ����������
sbit Uart2_Start  =Flag^1;	//����2��ʼ��������
sbit Uart2_End		=Flag^2;	//����2�������ݽ���

/*************	���غ�������	**************/
void GPIO_config(void);
void Timer0Init(void);
void CLR_Buf2(void);
u8 Find(u8 *a);
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time);
void Set_Text_Mode(void);
void Wait_CREG(void);
void Send_Text_Sms(void);
/*************  �ⲿ�����ͱ�������*****************/
/*******************************************************************************
* ������ : main 
* ����   : ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : ����2������GPRSģ��ͨ�ţ�����1���ڴ��ڵ��ԣ����Ա��������س���ʱ����
					 �����͵�ģ��
*******************************************************************************/
void main(void)
{
	GPIO_config();
	Uart1Init();
	Uart2Init();
	Timer0Init();
	EA=1;	//�����ж�
			   	
    UART1_SendString("SIM808ģ����Ų��Գ���\r\n");
		Wait_CREG();    //��ѯ�ȴ�ģ��ע��ɹ�
		Set_Text_Mode();//���ö���ΪTEXTģʽ
		 
		UART1_SendString("SIM808ģ��GPS���Գ���\r\n");
		Second_AT_Command("AT+CGNSPWR=1\r\n","OK",2);   //��GPS��Դ
		delay_ms(100); 
		Second_AT_Command("AT+CGNSSEQ=\"RMC\"\r\n","OK",2);//����NMEA����
		Second_AT_Command("AT+CGNSINF\r\n","+CGNSINF: 1,1,",3);//�ȴ���λ�ɹ�
		UART1_SendString("SIM808ģ�鶨λ�ɹ�\r\n");
		Second_AT_Command("AT+CGNSURC=0\r\n","OK",1); //����GPS�������
		CLR_Buf2();
 	/**************************************************************/	
	Send_Text_Sms();//����һ������Ϣ	
	/*************************************************************/
	while(1)
	{
		if(KEY)key_buf=1;//�����ɿ� 
		if((KEY==0)&&(key_buf))//��������
		{ 
				key_buf=0;
				content[0]=0;//����ַ���������
				strcat(content,"GPS data:");
				Second_AT_Command("AT+CGNSINF\r\n","OK",1);
				if((p2=(char*)strstr(Uart2_Buf,"OK")),(p2!=NULL))//Ѱ�ҽ�����
				{
					*p2=0;//��ӽ�����
					p2=strtok(Uart2_Buf,",");
					p2=(char*)strtok(NULL,",");
					p2=(char*)strtok(NULL,",");
					UART1_SendString("ʱ��:");UART1_SendString((char *)p2);UART1_SendLR();
					//strcat(content,"TIME:");
					//strcat(content,(char *)p2);
					p2=(char*)strtok(NULL,",");
					UART1_SendString("γ��:");UART1_SendString((char *)p2);UART1_SendLR();
					strcat(content,"latitude:");
					strcat(content,(char *)p2);
					p2=(char*)strtok(NULL,",");
					UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
					strcat(content,"longitude:");
					strcat(content,(char *)p2);
					p2=(char*)strtok(NULL,",");
					UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
					strcat(content,"elevation:");
					strcat(content,(char *)p2);UART1_SendString((char *)content);UART1_SendLR();
					Send_Text_Sms();//���Ͷ�λ��Ϣ
					delay_ms(1000); 
					CLR_Buf2();	
				}
		}
		
	}
}

/*******************************************************************************
* ������ : Uart1 
* ����   : ����1�жϷ�����ں���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
******************************************************************************
void Uart1() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //���RIλ
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
    }
}
*/
/*----------------------------
UART3 �жϷ������
-----------------------------*/
void Uart3() interrupt 17 using 1
{
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI;         //���S3RIλ
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI;         //���S3TIλ
    }
}


/*******************************************************************************
* ������ : Uart2
* ����   : ����2�жϷ�����ں���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Uart2() interrupt 8
{
		IE2  &= ~0x01;   //�رմ���2�ж�
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //���S2RIλ
			Uart2_Buf[First_Int] = S2BUF;  	  //�����յ����ַ����浽������
			First_Int++;                			//����ָ������ƶ�
			if(First_Int > Buf2_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
			}
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //���S2TIλ
    }
		IE2  |= 0x01;   //ʹ�ܴ���2�ж�
}
/*******************************************************************************
* ������ : Timer0_ISR
* ����   : ��ʱ��0�жϷ�����ں���,20ms�ж�һ��
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Timer0_ISR() interrupt 1
{
	static u8 Time_count=0; 
	TR0=0;//�ض�ʱ��
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
	TR0=1;//����ʱ��
}
/*******************************************************************************
* ������ : GPIO_config
* ����   : IO�����ú���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void	GPIO_config(void)
{
		P3M1 &= 0XC3;  //����P32~P35Ϊ�������
		P3M0 |=	~0XC3;
		LED=0;
		RUNING_LED=0;
}
void Timer0Init(void)		//20����@22.1184MHz
{
	AUXR &= 0x7F;	//12Tģʽ
	TMOD &= 0xF0;	//���ö�ʱ��ģʽ 16λ����
	TL0 = 0x00;		//�趨��ʱ����ֵ
	TH0 = 0x70;		//�趨��ʱ����ֵ
	TF0 = 0;			//���TF0��־
	TR0 = 1;			//��ʱ��0��ʼ��ʱ
	ET0 = 1;    	//ʹ�ܶ�ʱ��0�ж�
}
/*******************************************************************************
* ������ : CLR_Buf2
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //��������������
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(u8 *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Second_AT_Command(u8 *b,u8 *a,u8 wait_time)         
{
	u8 i;
	u8 *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				CLR_Buf2(); 
				b = c;							//���ַ�����ַ��b
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

/*******************************************************************************
* ������ : Set_Text_Mode
* ����   : ���ö���ΪTEXT�ı�ģʽ
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_Text_Mode(void)
{
	unsigned char temp[25]="AT+CSCA=";
	Second_AT_Command("ATE1","OK",3);										//ȡ������	
	Second_AT_Command("AT+CMGF=1","OK",3);								//TEXTģʽ	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//���в�������SIM���н���
	strcat(temp,SMS_Num); //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(temp,"OK",3);//���ö������ĺ�
}
/*******************************************************************************
* ������ : Send_Text_Sms
* ����   : ����TEXT�ı�����
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Send_Text_Sms(void)
{
	unsigned char temp[50]="AT+CMGS=";
	
	//strcat(temp,phone); //�ַ���ƴ�Ӻ������⺯����
	//Second_AT_Command(temp,">",3); //�ȴ����յ���>���ŷ��Ͷ�������
	//UART2_SendString(content);     //���Ͷ�������
	//UART2_SendData(0X1A);          //���ͽ�����
	//while(strstr(Uart2_Buf,"OK")==NULL);
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
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
		UART2_SendString("AT+CREG?");//��ѯģ������ע��״̬
		UART2_SendLR();
		delay_ms(5000);  						
	    for(k=0;k<Buf2_Max;k++)      			
    	{
			if(Uart2_Buf[k] == ':')
			{
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5')) //��������ע��ɹ�
				{
					i = 1;
				  break;
				}
			}
		}
	}
}

