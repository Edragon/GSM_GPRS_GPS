#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "oled.h"
#include "gps.h"
#include "string.h"    
#include "usart2.h" 
#include "sim800c.h"

u8 USART1_TX_BUF[USART2_MAX_RECV_LEN]; 					//����1,���ͻ�����
nmea_msg gpsx; 											//GPS��Ϣ
__align(4) u8 dtbuf[50];   								//��ӡ������

void Gps_Msg_Show(void);

int main(void)
{
	u16 i,rxlen;
	u8 err=0;
	
	NVIC_Configuration(); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	USART2_Init(115200);	//��GSMģ��ͨ��
	OLED_Init();
	OLED_Clear();

	USART2_RX_STA=0;	
	if(sim800c_send_cmd("AT","OK",200)) err|=1<<0;    //	
	if(sim800c_send_cmd("ATE0","OK",200)) err|=1<<1;  //�ػ���
	if(sim800c_send_cmd("AT+CGPSPWR=1","OK",200)) err|=1<<2; //��GPS��Դ
	if(sim800c_send_cmd("AT+CGPSOUT=255","OK",200)) err|=1<<3; //��NMEA�������
	OLED_ShowNum(112,0,err,2,16);
	
  while(1)
  {
		delay_ms(1);
		if(USART2_RX_STA&0X8000)		//���յ�һ��������
		{
			rxlen=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART2_RX_BUF[i];	   
 			USART2_RX_STA=0;		   	//������һ�ν���
			USART1_TX_BUF[i]=0;			//�Զ���ӽ�����
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//�����ַ���
			Gps_Msg_Show();				//��ʾ��Ϣ	
			#if 0
			printf("\r\n%s\r\n",USART1_TX_BUF);//���ͽ��յ������ݵ�����1
			#endif
 		}
  }
}

//��ʾGPS��λ��Ϣ 
void Gps_Msg_Show(void)
{
 	float tp;		   
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"%.5f %1c",tp/=100000,gpsx.ewhemi);	//�õ������ַ���
	OLED_ShowString(0,0,dtbuf);	 	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"%.5f %1c",tp/=100000,gpsx.nshemi);	//�õ�γ���ַ���
 	OLED_ShowString(0,2,dtbuf);
   
	sprintf((char *)dtbuf,"%02d/%02d",gpsx.posslnum,gpsx.svnum); //���ڶ�λ��GPS������/�ɼ�������
 	OLED_ShowString(88,6,dtbuf);    

	sprintf((char *)dtbuf,"%04d/%02d/%02d",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//��ʾUTC����
	OLED_ShowString(0,4,dtbuf);	
	sprintf((char *)dtbuf,"%02d:%02d:%02d",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//��ʾUTCʱ��
  OLED_ShowString(0,6,dtbuf);			  
}
