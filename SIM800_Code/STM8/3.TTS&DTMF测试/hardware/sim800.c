//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#include "stm8s.h"
#include "string.h"
#include "stdio.h"
#include "sim800.h"
#include "oled.h"

extern u8 UART_RX_BUF[300];
extern u16 UART_RX_NUM;
extern u8 UART_RX_STA;


void delay(u16 t)
{
  u8 i=0;
  for(;t>0;t--)
  {
    for(i=10;i>0;i--)
      asm("nop"); 
  }
}

u8 SIM800_Send_CMD(u8 *cmd,u8 *ack)
{
  u8 res=1; 
  
  UART_RX_STA=0;
  UART_RX_NUM=0;  
  memset(UART_RX_BUF,'\0',sizeof(UART_RX_BUF)); //�������

  printf("%s\r",cmd); //��������

  while(UART_RX_STA==0);  //�ȴ��������     
  if(ack)	      //Ӧ��У��
  { 
    if(strstr((char*)UART_RX_BUF,(char*)ack))
      res=0;                //У��ɹ�
    else
      res=1;
  }
  return res;
}

void SIM800_Init(void)
{ 
  u8 flag=0;   //�����־
  
  if(SIM800_Send_CMD("ATE0","OK")) flag|=1<<0;          //�ػ���

  if(SIM800_Send_CMD("AT+CMGF=1","OK")) flag|=1<<1;     //���� textģʽ  

  if(SIM800_Send_CMD("AT+CLIP=1","OK")) flag|=1<<2;     //������ʾ
  
  if(SIM800_Send_CMD("AT+COLP=1","OK")) flag|=1<<3;     //������ʾ
  
  if(SIM800_Send_CMD("AT+DDET=1,0,0","OK")) flag|=1<<4; //����DTMF  

  if(SIM800_Send_CMD("AT+CTTSPARAM=50,0,50,55,1","OK")) flag|=1<<5; //TTS����
                         // ���� ģʽ ���� �ٶ� ͨ��

  OLED_ShowNum(110,2,flag,2,16);  
  
  SIM800_Send_CMD( "ATD18127870064;" ,""); //����

}

void test(void)
{
  char *res,i=0;
  
  if(UART_RX_STA==1)
  {
    if((res=strstr((char*)UART_RX_BUF,"+CLIP:")))   //�ӵ�����
    {
      for(i=0;i<11;i++)   //+CLIP: "18127870064",161,"",0,"",0
      {
        OLED_ShowChar(40+8*i,6,res[8+i]);
      }
      SIM800_Send_CMD("ATA","OK");
      delay(6000);
      delay(6000);  
      SIM800_Send_CMD("AT+CTTS=2,\"��ӭʹ��Զ�̿��ƣ���LED�밴1���ر�LED�밴2\"","OK");
    }
    else if((res=strstr((char*)UART_RX_BUF,"+COLP:")))  //���н�ͨ
    {
      for(i=0;i<11;i++)     //+COLP: "18127870064",129,"",0,""
      {
        OLED_ShowChar(40+8*i,6,res[8+i]);
      }
      delay(6000);  
      SIM800_Send_CMD("AT+CTTS=2,\"��ӭʹ��Զ�̿��ƣ���LED�밴1���ر�LED�밴2\"","OK");
    }
    else if((res=strstr((char*)UART_RX_BUF,"+DTMF:")))
    {
      OLED_ShowChar(48,4,res[7]);        
      if(res[7]=='1')
      {
        SIM800_Send_CMD("AT+CTTS=0","OK");  //ֹͣ����
        SIM800_Send_CMD("AT+CTTS=2,\"LED�Ѵ�\"","OK");
        GPIO_WriteHigh(LED_PORT,LED_PIN );     
        OLED_ShowString(40,2,"ON ");
      }
      else if(res[7]=='2')
      {
        SIM800_Send_CMD("AT+CTTS=0","OK");  //ֹͣ����
        SIM800_Send_CMD("AT+CTTS=2,\"LED�ѹر�\"","OK");
        GPIO_WriteLow(LED_PORT,LED_PIN );  
        OLED_ShowString(40,2,"OFF");
      }
      else
      {
        SIM800_Send_CMD("AT+CTTS=0","OK");  //ֹͣ����
        SIM800_Send_CMD("AT+CTTS=2,\"��Ч����\"","OK"); 
      }
    }
    else if(strstr((char*)UART_RX_BUF,"NO CARRIER"))   //�ҵ绰
    {
      OLED_ShowChar(48,4,' ');  
      OLED_ShowString(40,6,"            "); 
    }
    
    UART_RX_STA=0;
    UART_RX_NUM=0;  
    memset(UART_RX_BUF,'\0',sizeof(UART_RX_BUF)); //
  }
}
