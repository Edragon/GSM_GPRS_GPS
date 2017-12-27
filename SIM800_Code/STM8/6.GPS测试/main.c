//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stdio.h"
#include "string.h"

#include "oled.h"
#include "oledbmp.h"
#include "timer.h"
#include "uart.h"
#include "sim800.h"
#include "gps.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

nmea_msg gpsx; 	        //GPS��Ϣ
u8 dtbuf[50];   	//��ӡ����

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

void main(void)        
{ 
  
  u8 err=0;

  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
  
//  disableInterrupts();
  enableInterrupts();  //�����ж�  

  UART2_Config(115200);   //9600bps 
  TIM1_Config();        //������ɶ�ʱ��

  OLED_Init();	       //��ʼ��OLED  
  OLED_Clear();

  if(SIM800_Send_CMD("AT","OK")) err|=1<<0;    //	
  if(SIM800_Send_CMD("ATE0","OK")) err|=1<<1;  //�ػ���
  if(SIM800_Send_CMD("AT+CGPSPWR=1","OK")) err|=1<<2; //��GPS��Դ
  if(SIM800_Send_CMD("AT+CGPSOUT=255","OK")) err|=1<<3; //��NMEA�������
  OLED_ShowNum(112,0,err,2,16);
  
  while(1)
  {
    delay(1000);
    if(UART_RX_STA==1)
    {
      OLED_ShowNum(96,2,UART_RX_NUM,4,16);  //��ʾ�ַ�������	
      GPS_Analysis(&gpsx,(u8*)UART_RX_BUF); //�����ַ���
      Gps_Msg_Show();		            //��ʾ��Ϣ	

      UART_RX_STA=0;
      UART_RX_NUM=0;  
      memset(UART_RX_BUF,'\0',sizeof(UART_RX_BUF)); //���
    }  
  }
}
  
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
