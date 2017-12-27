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

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)        
{ 
  
  u8 err=0;
  u8 bl_status=0;
  u8 *p1,*p2;
//  u8 sendbuf[20];

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
  OLED_ShowNum(112,0,err,2,16);  
  SIM800_Send_CMD("AT+BTPOWER=1",""); //��������Դ
  SIM800_Send_CMD("AT+BTUNPAIR=0",""); //ɾ������������豸
  SIM800_Send_CMD("AT+BTSCAN=1,50","OK");  //��ʼ����50S  
  OLED_ShowString(0,0,"Scaning..");  
  while(1)
  {
    delay(500);
    if(UART_RX_STA==1)
    {
      if(bl_status==0)   //δ���ӳɹ�
      {
        if(strstr((char*)UART_RX_BUF,"+BTSCAN: 0,1"))  //�����������豸
        {
          SIM800_Send_CMD("AT+BTPAIR=0,1","OK");    //�����������
        }
        else if(strstr((char*)UART_RX_BUF,"+BTPAIRING: "))   //�յ��������
        {
          SIM800_Send_CMD("AT+BTPAIR=1,1","OK"); //Ӧ�����
        }
        else if(strstr((char*)UART_RX_BUF,"+BTPAIR: 1"))     //��Գɹ�
        {
          OLED_ShowString(0,0,"Paired   "); 
        }
        else if(strstr((char*)UART_RX_BUF,"+BTCONNECT: 1"))  //���ӳɹ�
        {
          bl_status=1;
          OLED_ShowString(0,0,"Connected"); 
        }
        else if(strstr((char*)UART_RX_BUF,"+BTDISCONN: "))   //�Ͽ����� 
        {
          bl_status=0;
          OLED_ShowString(0,0,"DisConect");
        }	
      }
      else 
      {
        if(strstr((char*)UART_RX_BUF,"+BTCONNECTING: "))  //�յ��ֻ���SPP��������
        {
          if(SIM800_Send_CMD("AT+BTACPT=1","+BTCONNECT: "))//Ӧ���ֻ���spp�������� 7S
          {
            OLED_ShowString(0,0,"SPP Connect Fail"); 
          }else
          {
            OLED_ShowString(0,0,"SPP Connected   "); 
            SIM800_Send_CMD("AT+BTSPPSEND",">");
            SIM800_Send_CMD("SIM800 Bluetooth SPP test\r\n\x1A","OK");//��������
          }
        }
        else if(p1=(u8*)strstr((char*)UART_RX_BUF,"+BTSPPDATA: ")) //���յ�����
        {
          p2 = (u8*)strstr((const char *)p1,"\x0d\x0a");  //+BTSPPDATA: 2,6,ryjjgf
          p2[0]=0;//��ӽ�����
          p2 =(u8*)strstr((const char *)p1,",");
          p1 =(u8*)strstr((const char *)p2+1,",");
          OLED_ShowString(0,2,"                ");
          OLED_ShowString(0,4,"                ");
          OLED_ShowString(0,6,"                ");
          OLED_ShowString(0,2,(u8*)(p1+1)); 
        }
        else if(strstr((char*)UART_RX_BUF,"+BTDISCONN:"))   //�Ͽ����� 
        {
          bl_status=0;
          OLED_Clear();
          OLED_ShowString(0,0,"DisConect");
        }
      }

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
