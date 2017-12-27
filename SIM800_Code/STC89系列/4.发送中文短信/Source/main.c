/**********************************************************************************

**********************************************************************************/
#include "config.h"
#include "string.h"
#include "delay.h"
#include "uart.h"


#define Buf1_Max 200 					  //����2���泤��
/*************	���س�������	**************/
sbit RUNING_LED = P3^2;					//����ָʾ��
u8 key_buf=0;
sbit KEY        = P3^5;         //����
char *p2; 
static unsigned char *phone="\"15124532672\"";     //���Ž��շ��ֻ���
xdata unsigned char content[100]="GPS positioning success";//���Ͷ�������

/*************  ���ر�������	**************/

xdata u8 Uart1_Buf[Buf1_Max];

u8 Times=0,First_Int = 0,shijian=0;
bdata u8 Flag;//��ʱ����־λ
sbit Timer0_start =Flag^0;	//��ʱ��0��ʱ����������

/*************	���غ�������	**************/
void GPIO_config(void); //�˿ڳ�ʼ������
void Timer0Init(void);  //��ʱ��0��ʼ��
void CLR_Buf1(void);    //�������2���ջ���
u8 Find(u8 *a);         //�����ַ���
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time); //����ATָ���
void Set_Text_Mode(void);//���ö���ģʽTEST
void Wait_CREG(void);   //��ѯ�ȴ�ģ��ע��ɹ�
void Send_Text_Sms(void);//����һ�����Ķ���
/*************  �ⲿ�����ͱ�������*****************/

/*******************************************************************************
* ������ : main 
* ����   : ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : ���ڲ�������9600��GPRSģ��Ĭ�ϲ�������115200����Ҫ�Լ�ͨ�����������޸�
				   Ϊ9600����ʹ�á� 
*******************************************************************************/
void main(void)
{
	Timer0Init();  //��ʼ����ʱ��0
	GPIO_config();
	EA=1;	//�����ж�
	Uart1Init();    //��ʼ������9600
	Wait_CREG();    //��ѯ�ȴ�ģ��ע��ɹ�
	Set_Text_Mode();//���ö���ΪTEXTģʽ	 
	//UART1_SendString("SIM808ģ��GPS���Գ���\r\n");
	Second_AT_Command("AT+CGNSPWR=1\r\n","OK",2);   //��GPS��Դ
	delay_ms(100); 
	Second_AT_Command("AT+CGNSSEQ=\"RMC\"\r\n","OK",2);//����NMEA����
	Second_AT_Command("AT+CGNSINF\r\n","+CGNSINF: 1,1,",3);//�ȴ���λ�ɹ�
	//UART1_SendString("SIM808ģ�鶨λ�ɹ�\r\n");
	Second_AT_Command("AT+CGNSURC=0\r\n","OK",1); //����GPS�������
	CLR_Buf1();
	Send_Text_Sms();
	
	while(1)
	{
		if(KEY)key_buf=1;//�����ɿ� 
		if((KEY==0)&&(key_buf))//��������
		{ 
			key_buf=0;
			content[0]=0;//����ַ���������
			strcat(content,"GPS data:");
			Second_AT_Command("AT+CGNSINF\r\n","OK",1);
			if((p2=(char*)strstr(Uart1_Buf,"OK")),(p2!=NULL))//Ѱ�ҽ�����
			{
				*p2=0;//��ӽ�����
				p2=strtok(Uart1_Buf,",");
				p2=(char*)strtok(NULL,",");
				p2=(char*)strtok(NULL,",");
				//UART1_SendString("ʱ��:");UART1_SendString((char *)p2);UART1_SendLR();
				//strcat(content,"TIME:");
				//strcat(content,(char *)p2);
				p2=(char*)strtok(NULL,",");
				//UART1_SendString("γ��:");UART1_SendString((char *)p2);UART1_SendLR();
				strcat(content,"latitude:");
				strcat(content,(char *)p2);
				p2=(char*)strtok(NULL,",");
				//UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
				strcat(content,"longitude:");
				strcat(content,(char *)p2);
				p2=(char*)strtok(NULL,",");
				//UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
				strcat(content,"elevation:");
				strcat(content,(char *)p2);UART1_SendString((char *)content);UART1_SendLR();
				Send_Text_Sms();//���Ͷ�λ��Ϣ
				delay_ms(1000); 
				CLR_Buf1();	
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
*******************************************************************************/
void Uart1() interrupt 4
{
    if (RI)
    {
      RI = 0;                 //���RIλ
			Uart1_Buf[First_Int] = SBUF;  	  //�����յ����ַ����浽������
			First_Int++;                			//����ָ������ƶ�
			if(First_Int > Buf1_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
    }
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
  TL0 = 0x00;		  //���ö�ʱ����ֵ
	TH0 = 0xB8;		  //���ö�ʱ����ֵ	
	TR0=0;//�ض�ʱ��
	Time_count++;
	if(Time_count>=50)
	{
		Time_count = 0;
		RUNING_LED =~RUNING_LED;
	}
	if(count_20ms) //20ms��ʱ������
		count_20ms--;
	if(Timer0_start)
	Times++;
	if(Times > (50*shijian))
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
		RUNING_LED=0;
}
/*******************************************************************************
* ������ : Timer0Init
* ����   : ��ʱ��0��ʼ����20ms��ʱ
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Timer0Init(void)		//20����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//
	TMOD |= 0x01;		//���ö�ʱ��ģʽ��16λ��ʱ��
	TL0 = 0x00;		  //���ö�ʱ����ֵ
	TH0 = 0xB8;		  //���ö�ʱ����ֵ
	TF0 = 0;		    //��TF0��־
	TR0 = 1;		    //��ʱ��0��ʼ��ʱ
	ET0 = 1;    	  //ʹ�ܶ�ʱ��0�ж�
}
/*******************************************************************************
* ������ : CLR_Buf1
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //��������������
	{
		Uart1_Buf[k] = 0x00;
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
  if(strstr(Uart1_Buf,a)!=NULL)
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
	CLR_Buf1(); 
    i = 0;
	while(i == 0)                    
	{
		if(!Find(a))           //������ҪӦ����ַ�
		{
			if(Timer0_start == 0)//��ʱ���·�������
			{
				b = c;						 //���ַ�����ַ��b
				for (b; *b!='\0';b++)
				{
					UART1_SendData(*b);
				}
				UART1_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;  //��ʼ��ʱ
		   }
    	}
 	  	else
		{
			i = 1;
			Timer0_start = 0;  
		}
	}
	CLR_Buf1(); 
}
/*******************************************************************************
* ע��   : 
*******************************************************************************/
void Set_Text_Mode(void)
{	
	Second_AT_Command("ATE1","OK",3);										//ȡ������	
	Second_AT_Command("AT+CMGF=1","OK",3);	
	Second_AT_Command("AT+CSMP=17,167,0,241","OK",3);							//TEXTģʽ	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//���в�������SIM���н���
	Second_AT_Command("AT+CSCA?","OK",3);
}
/*******************************************************************************
*******************************************************************************/
void Send_Text_Sms(void)
{
	unsigned char temp[50]="AT+CMGS=";		
	strcat(temp,phone); //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(temp,">",3); //�ȴ����յ���>���ŷ��Ͷ�������
	UART1_SendString(content);     //���Ͷ�������
	UART1_SendData(0X1A);          //���ͽ�����
	while(strstr(Uart1_Buf,"OK")==NULL);
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
	CLR_Buf1();
  	while(i == 0)        			
	{
		CLR_Buf1();        
		UART1_SendString("AT+CREG?");//��ѯģ������ע��״̬
		UART1_SendLR();
		delay_ms(250);  						
	    for(k=0;k<Buf1_Max;k++)      			
    	{
			if(Uart1_Buf[k] == ':')
			{
				if((Uart1_Buf[k+4] == '1')||(Uart1_Buf[k+4] == '5')) //��������ע��ɹ�
				{
					i = 1;
				  break;
				}
			}
		}
	}
}
