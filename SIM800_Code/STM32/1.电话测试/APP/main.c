#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "sim800c.h"
#include "string.h"    
#include "usart2.h" 

void sim800c_init(void);

int main(void)
{
	u8 callbuf[]="18127870064"; //���еĺ���
	u8 p[20],*p1,*p2;

	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	USART2_Init(115200);	//��GSMģ��ͨ��
	OLED_Init();
	OLED_Clear();

	sim800c_init();

	sim800c_send_cmd("AT+CLIP=1","OK",200);	//����������ʾ  
	sim800c_send_cmd("AT+COLP=1","OK",200);	//���ñ��к�����ʾ
	u2_printf("ATD%s;\r\n",callbuf);//����	
	
  while(1)
  {
		if(USART2_RX_STA&0X8000)		//���յ�����
		{
			if(sim800c_check_cmd("+CLIP:"))//���յ�����
			{
				p1=sim800c_check_cmd("+CLIP:");  //+CLIP: "18127870064",161,"",0,"",0
				p1+=8;
				p2=(u8*)strstr((const char *)p1,"\"");
				p2[0]=0;//��ӽ����� 
				strcpy((char*)p,(char*)p1);
				OLED_ShowString(0,6,p);
				sim800c_send_cmd("ATA","OK",200);//����Ӧ��ָ��
			}
			else if(sim800c_check_cmd("+COLP:"))//��ͨ�绰
			{
				p1=sim800c_check_cmd("+COLP:");  //+COLP: "18127870064",129,"",0,""
				p1+=8;
				p2=(u8*)strstr((const char *)p1,"\"");
				p2[0]=0;//��ӽ����� 
				strcpy((char*)p,(char*)p1);
				OLED_ShowString(0,6,p);
			}
			else if(sim800c_check_cmd("NO CARRIER"))   //�ҵ绰
			{
				OLED_ShowString(0,6,"            ");     //��պ�����ʾ����
			}
			USART2_RX_STA=0;
		}
    LED0=!LED0;
    delay_ms(1000);
  }
}

void sim800c_init(void)
{
	u8 p[50],*p1,*p2;
	u8 err=0;
	USART2_RX_STA=0;	
	if(sim800c_send_cmd("AT","OK",100))err|=1<<0;//����Ƿ�Ӧ��ATָ�� 
	USART2_RX_STA=0;		
	if(sim800c_send_cmd("ATE0","OK",200))err|=1<<1;//������	
	USART2_RX_STA=0;
	if(sim800c_send_cmd("AT+CGMM","OK",200)==0)//��ѯģ������
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n"); 
		p1[0]=0;//���������
		sprintf((char*)p,"%s",USART2_RX_BUF+2);
		OLED_ShowString(0,0,p); 
		USART2_RX_STA=0;		
	}else err|=1<<2;
	if(sim800c_send_cmd("AT+CPIN?","OK",200))err|=1<<3;	//��ѯSIM���Ƿ���λ 
	USART2_RX_STA=0;  
	if(sim800c_send_cmd("AT+COPS?","OK",200)==0)		//��ѯ��Ӫ������
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),"\""); 
		if(p1)//����Ч����
		{
			p2=(u8*)strstr((const char*)(p1+1),"\"");
			p2[0]=0;//���������			
			sprintf((char*)p,"%s",p1+1);
			OLED_ShowString(0,2,p);
		}else err|=1<<4; 
		USART2_RX_STA=0;		
	}else err|=1<<4;
	
	OLED_ShowString(0,4,"err=");
	OLED_ShowNum(32,4,err,2,16);
} 
