/**********************************************************************************

 * �������˵��
   �����ϵ������ָʾ��RUNING_LED����һ���Ƶ����˸
	 ���������͡�onled������LED��
   ���������͡�offled��Ϩ��LED��
	 ��Ƭ��ÿ��10������������������֡��OK��
**********************************************************************************/
#include "config.h"
#include "string.h"
#include "delay.h"
#include "uart.h"

#define Buf1_Max 200 					  //����2���泤��
/*************	���س�������	**************/

static unsigned char *phone="\"17086558896\"";     //���Ž��շ��ֻ���  ��ʵ��Ӧ����ֻ��Ҫ��������뼴��
/*************  ���ر�������	**************/
xdata u8 Uart1_Buf[Buf1_Max];
xdata unsigned char content[100]="UNV-SIM808-GPS-TEST";//���Ͷ�������
u8 Times=0,First_Int = 0,shijian=0;
u8 key_buf=0;
char *p2;
//u8 Time_count=0;
bdata u8 Flag;//��ʱ����־λ
sbit Timer0_start =Flag^0;	//��ʱ��0��ʱ����������
sbit Heart_beat		=Flag^1;	//��������֡��־λ
sbit KEY        = P3^5;         //����		,�͵�ƽ����
sbit RUNING_LED =P1^0;


/*************	���غ�������	**************/
void GPIO_config(void);
void Timer0Init(void);
void CLR_Buf2(void);
u8 Find(u8 *a);
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time);
void Wait_CREG(void);

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
      	RI = 0;                           //���RIλ
		Uart1_Buf[First_Int] = SBUF;  	  //�����յ����ַ����浽������
		First_Int++;                			//����ָ������ƶ�
		if(First_Int > Buf1_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
		{
			First_Int = 0;
		}
    }
    if (TI)
    {
        TI = 0;                          //���TIλ
    }
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
	TL0 = 0x00;		//���趨ʱ����ֵ
	TH0 = 0xB8;		//���趨ʱ����ֵ
	Time_count++;
	if(Time_count>=50)//1������ָʾ����˸
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
    First_Int = 0;             //�����ַ�������ʼ�洢λ��
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
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
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
	Second_AT_Command("ATE1","OK",3);									//ȡ������	
	Second_AT_Command("AT+CMGF=1","OK",3);								//TEXTģʽ	
	Second_AT_Command("AT+CSCS=\"GSM\"","OK",3);								
	Second_AT_Command("AT+CSMP=17,167,0,241","OK",3);		
	Second_AT_Command("AT+CSCA?","OK",3);//���ö������ĺ�
}

void Send_Text_Sms(u8 *temp_data)
{
	unsigned char temp[50]="AT+CMGS=";
	
	strcat(temp,phone); //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(temp,">",3); //�ȴ����յ���>���ŷ��Ͷ�������
	UART1_SendString(temp_data);     //���Ͷ�������
	UART1_SendData(0X1A);          //���ͽ�����	
}
u8 GPS_Datt_handle(void)
{	
	content[0]=0;//����ַ���������
	strcat(content,"GPS data:");
	Second_AT_Command("AT+CGNSINF\r\n","OK",3);
	if((p2=(char*)strstr(Uart1_Buf,"OK")),(p2!=NULL))//Ѱ�ҽ�����
	{
		*p2=0;//��ӽ�����
		p2=strtok(Uart1_Buf,",");
		if(p2==NULL) return 1;
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 2;
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 3;
		//UART1_SendString("ʱ��:");UART1_SendString((char *)p2);UART1_SendLR();
		//strcat(content,"TIME:");
		//strcat(content,(char *)p2);
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 4;
		//UART1_SendString("γ��:");UART1_SendString((char *)p2);UART1_SendLR();
		strcat(content,"latitude:N");
		strcat(content,(char *)p2);
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 5;
		//UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
		strcat(content,"longitude:E");
		strcat(content,(char *)p2);
		p2=(char*)strtok(NULL,",");
		if(p2==NULL) return 6;
		//UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
		strcat(content,"elevation:");
		strcat(content,(char *)p2);//UART1_SendString((char *)content);							
		//delay_ms(1000); 
		CLR_Buf1();	
		return 0;
	}
	return 7;
}
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
	EA=1;	//�����ж�
	Uart1Init();    //��ʼ������9600

	Wait_CREG();    //��ѯ�ȴ�ģ��ע��ɹ�
	Set_Text_Mode();//���ö���ΪTEXTģʽ		 
	
	Second_AT_Command("AT+CGNSPWR=1\r\n","OK",2);   //��GPS��Դ
//	delay_ms(100); 
//	Second_AT_Command("AT+CGNSSEQ=\"RMC\"\r\n","OK",2);//����NMEA����
//	Second_AT_Command("AT+CGNSINF\r\n","+CGNSINF: 1,1,",3);//�ȴ���λ�ɹ�
//	Second_AT_Command("AT+CGNSURC=0\r\n","OK",1); //����GPS�������
//	CLR_Buf1();
	Send_Text_Sms("UNV-SIM808-TEST");//�����ʺ���Ϣ
	while(1)
	{
		if(!KEY)delay_ms(10);
		{
			if((KEY==0))//��������
			{ 								
			 	if(!GPS_Datt_handle())
					Send_Text_Sms(content);//���Ͷ�λ��Ϣ				
			}
		}		
	}
}


