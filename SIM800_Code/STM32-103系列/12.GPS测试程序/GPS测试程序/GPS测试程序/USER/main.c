/**********************************************************************************
 * ������  :GPS��λ���ܲ��Գ���
 * ����    :ͨ��STM32�����崮��2����ģ���GPS��������λ��Ϣ�����󷢵�����1
 * ʵ��ƽ̨:STM32F10X
 * ��汾  :


 * Ӳ������˵��
	 ʹ�õ�Ƭ����2��GPRSģ��ͨ��  ע��ʹ�ô���2���Ա������غ�ͨ�Ų����ͻ
	 STM32      GPRSģ��
	 PA3 (RXD2)->TXD
	 PA2 (TXD2)->RXD
	 GND	   ->GND

 * �������˵��
   �����ϵ������ָʾ��RUNING_LED����һ���Ƶ����˸
**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"

#define Buf2_Max 200 					  //����2���泤��
/*************	���س�������	**************/

/*************  ���ر�������	**************/

char Uart2_Buf[Buf2_Max]; //����2���ջ���

char *p1,*p2;             
vu8 Times=0,First_Int = 0,shijian=0;

vu8 Timer0_start;	//��ʱ��0��ʱ����������

/*************	���غ�������	**************/
void System_Initialization(void); //STM32ϵͳ��ʼ��
void Ram_Initialization(void);    //�ڴ������ʼ��
void NVIC_Configuration(void);    //�ж��������ʼ��
void CLR_Buf2(void);              //�崮��2���ջ���
u8 Find(char *a);                 //�����ַ���
void Second_AT_Command(char *b,char *a,u8 wait_time);  //����ATָ��
/*************  �ⲿ�����ͱ�������*****************/
/*******************************************************************************
* ������ : main 
* ����   : ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : ����2������ģ��ͨ�ţ�����1���ڴ��ڵ��ԣ����Ա��������س���ʱ����
					 �����͵�ģ��
*******************************************************************************/
int main(void)
{
	System_Initialization(); //ϵͳ��ʼ��
	Ram_Initialization();    //������ʼ��

	UART1_SendString("SIM808ģ��GPS���Գ���\r\n");
	UART1_SendString("SIM808ģ��������\r\n");
	Second_AT_Command("AT+CGNSPWR=1\r\n","AT",20);   //��GPS��Դ
	UART1_SendString("SIM808ģ�����ڻ�ȡ��λ��Ϣ\r\n");
	Delay_nMs(100); 	
	CLR_Buf2();	
	while(1)
	{
		Second_AT_Command("AT+CGNSINF\r\n","OK",30);   //�������
		if((p1=(char*)strstr((const char*)Uart2_Buf,"CGNSINF:")),(p1!=NULL))//Ѱ�ҿ�ʼ��
		{		
				if((p2=(char*)strstr((const char*)p1,"OK")),(p2!=NULL))//Ѱ�ҽ�����
				{
					*p2=0;//��ӽ�����
					p2=strtok((p1),",");
					p2=(char*)strtok(NULL,",");
					p2=(char*)strtok(NULL,",");
					UART1_SendString("ʱ��:");UART1_SendString((char *)p2);UART1_SendLR();
					p2=(char*)strtok(NULL,",");
					UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
//					p2=(char*)strtok(NULL,",");
//					UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
					p2=(char*)strtok(NULL,",");
					UART1_SendString("γ��:");UART1_SendString((char *)p2);UART1_SendLR();
//					p2=(char*)strtok(NULL,",");
//					UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
//					p2=(char*)strtok(NULL,",");
//					p2=(char*)strtok(NULL,",");
//					UART1_SendString("������:");UART1_SendString((char *)p2);UART1_SendLR();
//					p2=(char*)strtok(NULL,",");
//					UART1_SendString("����:");UART1_SendString((char *)p2);UART1_SendLR();
					UART1_SendLR();		
			    CLR_Buf2();
				}
		}
	}
	
}

/*******************************************************************************
* ������  : System_Initialization
* ����    : STM32ϵͳ��ʼ������(��ʼ��STM32ʱ�Ӽ�����)
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void System_Initialization(void)
{
	RCC_Configuration();		//����ϵͳʱ��Ϊ72MHZ(������Ը�����Ҫ��)
  SysTick_Init_Config();  //��ʼ��ϵͳ�δ�ʱ��SysTick
  NVIC_Configuration();		//STM32�ж�������������
	Timer2_Init_Config();		//��ʱ��2��ʼ������
	USART1_Init_Config(115200);	//����1��ʼ������
	USART2_Init_Config(115200);	//����2��ʼ������	
	GPIO_Config();          //��ʼ��GPIO
}
/*******************************************************************************
* ������  : Ram_Initialization
* ����    : ������ʼ������
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void Ram_Initialization(void)
{
	Times=0;
	shijian=0;
	Timer0_start=0;
}

/*******************************************************************************
* ������  : NVIC_Configuration
* ����    : STM32�ж�������������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ����KEY1(PC11)���ж�������
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;						//����NVIC��ʼ���ṹ��

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				    //�����ж����ȼ���Ϊ2����ռ���ȼ��ʹ����ȼ�����λ(����0��3)
	
	/*��ʱ��2�ж���������*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				    //�����ж�������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//�����������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
	NVIC_Init(&NVIC_InitStructure);
	/*����1�ж���������*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				  //�����ж�������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�����������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
	NVIC_Init(&NVIC_InitStructure);
  /*����2�ж���������*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;				  //�����ж�������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�����������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
	NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* ������  : USART2_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
			u8 Res=0;
			Res =USART_ReceiveData(USART2);
			Uart2_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
			First_Int++;                			//����ָ������ƶ�
			if(First_Int > Buf2_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
			}    		
} 	

/*******************************************************************************
* ������  : TIM2_IRQHandler
* ����    : ��ʱ��2�ж϶Ϸ�����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
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
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
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

void Second_AT_Command(char *b,char *a,u8 wait_time)         
{
	while(*b)//����ַ���������
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*b++);//���͵�ǰ�ַ�
	}
	Delay_nMs(wait_time*100);	
}

