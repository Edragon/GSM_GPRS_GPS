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
	u8 *p1,*p2;
	u8 bl_status=0;
	u8 sendbuf[20];
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	USART2_Init(115200);	//��GSMģ��ͨ��
	OLED_Init();
	OLED_Clear();

	sim800c_init();

	if(sim800c_send_cmd("AT+BTPOWER=1","",200));  //��������Դ
	delay_ms(5);
	sim800c_send_cmd("AT+BTUNPAIR=0","",200);     //ɾ������������豸
	delay_ms(5);
	sim800c_send_cmd("AT+BTSCAN=1,50","OK",200);  //��ʼ����50S  
	OLED_ShowString(0,6,"Scaning.."); 
  while(1)
  {
		if(USART2_RX_STA&0X8000)		//���յ�����
		{
			if(bl_status==0)   //δ���ӳɹ�
			{
				if(sim800c_check_cmd("+BTSCAN: 0,1"))   //�����������豸
				{
					sim800c_send_cmd("AT+BTPAIR=0,1","OK",200); //�����������
				}
				else if(sim800c_check_cmd("+BTPAIRING: "))   //�յ��������
				{
					sim800c_send_cmd("AT+BTPAIR=1,1","OK",200); //Ӧ�����
				}
				else if(sim800c_check_cmd("+BTPAIR: 1"))     //��Գɹ�
				{
					OLED_ShowString(0,6,"Paired   "); 
				}
				else if(sim800c_check_cmd("+BTCONNECT: 1"))  //���ӳɹ�
				{
					bl_status=1;
					OLED_ShowString(0,6,"Connected"); 
				}
				else if(sim800c_check_cmd("+BTDISCONN: "))   //�Ͽ����� 
				{
					bl_status=0;
					OLED_ShowString(0,6,"DisConect");
				}	
			}else 
			{
				if(sim800c_check_cmd("+BTCONNECTING: "))  //�յ��ֻ���SPP��������
				{
					if(sim800c_send_cmd("AT+BTACPT=1","+BTCONNECT: ",700))//Ӧ���ֻ���spp�������� 7S
					{
						OLED_Clear();
						OLED_ShowString(0,0,"SPP Connect Fail"); 
					}else
					{
						OLED_Clear();
						OLED_ShowString(0,0,"SPP Connected"); 
						sim800c_send_cmd("AT+BTSPPSEND",">",100);//��������
						sprintf((char*)sendbuf,"%s \r\n\x1A","SIM800C Bluetooth SPP test"); //
						sim800c_send_cmd((u8*)sendbuf,"OK",100);//��������
					}
				}
				else if(sim800c_check_cmd("+BTSPPDATA: ")) //���յ�����
				{
					p1=sim800c_check_cmd("+BTSPPDATA: ");   //+BTSPPDATA: 2,6,ryjjgf
					p2 = (u8*)strstr((const char *)p1,"\x0d\x0a"); 
					p2[0]=0;//��ӽ�����
					p2 =(u8*)strstr((const char *)p1,",");
					p1 =(u8*)strstr((const char *)p2+1,",");
					//printf("%s",p1+1);//��ӡ������
					OLED_ShowString(0,2,"                ");
					OLED_ShowString(0,4,"                ");
					OLED_ShowString(0,6,"                ");
					OLED_ShowString(0,2,(u8*)(p1+1)); 
				}
				else if(sim800c_check_cmd("+BTDISCONN:"))   //�Ͽ����� 
				{
					bl_status=0;
					OLED_Clear();
					OLED_ShowString(0,6,"DisConect");
				}
			}
			USART2_RX_STA=0;			
		}
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
