#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "sim800c.h"
#include "string.h"    
#include "usart2.h" 


//��������
typedef enum {EN = 0, CN = 1} MsgType;
          // 18127870064            //unicode��:18127870064
u8 *NUM[2]={"18127870064","00310038003100320037003800370030003000360034"}; 
                                                  //unicode��:SIM800���յ�����
u8 *Msg[2]={"sim800 received your message","00530049004D00380030003063A56536523077ED4FE1"};  

void sim800c_init(void);
void sim800c_sendmsg( u8 *Msg ,MsgType type);

int main(void)
{

	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	USART2_Init(115200);	//��GSMģ��ͨ��
	OLED_Init();
	OLED_Clear();

	sim800c_init();

  while(1)
  {
		if(USART2_RX_STA&0X8000)		
		{
			if(sim800c_check_cmd("+CMT:"))  //���յ�����
			{                               //+CMT: "18127870064","","16/11/25,10:27:31+32"
				OLED_ShowString(0,6,"receive message");
				sim800c_sendmsg(Msg[CN] ,CN);  
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

//Msg[]:��������
//type:  0->Ӣ��  1->����
void sim800c_sendmsg( u8 *Msg ,MsgType type)
{
  u8 p[70]="";
	
	sim800c_send_cmd("AT+CMGF=1","OK",200);			//�����ı�ģʽ 
	sim800c_send_cmd("AT+CNMI=2,2","OK",200); 	//��������Ϣ��ʾ
  if( type == EN )
  {
    sim800c_send_cmd("AT+CSCS=\"GSM\"","OK",200);     //����GSM�ַ���
    sim800c_send_cmd("AT+CSMP=17,167,0,0","OK",200);	//���ö���Ϣ�ı�ģʽ���� 
  }
  else if( type == CN ) 
  {
    sim800c_send_cmd("AT+CSCS=\"UCS2\"","OK",200);    //����UCS2�ַ���
    sim800c_send_cmd("AT+CSMP=17,167,0,8","OK",200);  //���ö���Ϣ�ı�ģʽ���� 
  }
	sprintf((char*)p,"AT+CMGS=\"%s\"",NUM[type]); 
	if(sim800c_send_cmd(p,">",200)==0)					//���Ͷ�������+�绰����
	{ 		 				 													 
		u2_printf("%s",Msg);		 						//���Ͷ������ݵ�GSMģ��
		delay_ms(90);                     //������ʱ�������ܷ��Ͷ���						
		sim800c_send_cmd((u8*)0X1A,"+CMGS:",1000); //���ͽ�����,�ȴ��������
	} 
}
