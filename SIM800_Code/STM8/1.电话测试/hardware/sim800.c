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
#include "uart.h"

extern u8 UART_RX_BUF[300];
extern u16 UART_RX_NUM;
extern u8 UART_RX_STA;

          // 18127870064            //unicode�룺18127870064
u8 *NUM[2]={"18127870064","00310038003100320037003800370030003000360034"}; 
                                                  //unicode�룺SIM800���Զ���
u8 *Msg[2]={"sim800h receive your message","00530049004D0038003000306D4B8BD577ED4FE1"};  

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
  
  if(SIM800_Send_CMD("ATE1","OK")) flag|=1<<0;          //�ػ���

  if(SIM800_Send_CMD("AT+CMGF=1","OK")) flag|=1<<1;     //���� textģʽ  

  if(SIM800_Send_CMD("AT+CLIP=1","OK")) flag|=1<<2;     //������ʾ
  
  if(SIM800_Send_CMD("AT+COLP=1","OK")) flag|=1<<3;     //���к�����ʾ
  
  if(SIM800_Send_CMD("AT+CMIC=0,15","OK")) flag|=1<<4;  //change the microphone gain level
  
  if(SIM800_Send_CMD("AT+SIDET=0,0","OK")) flag|=1<<5;  //change the side tone gain level
  
  if(SIM800_Send_CMD("AT+ECHO=0,1024,1024,1024,1024,1","OK")) flag|=1<<6; //Echo cancellation control

  OLED_ShowNum(110,2,flag,2,16); 
  
}

void SIM800_Call(void)
{
  u8 num[70]="";

  sprintf((char*)num,"ATD%s;",NUM[0]);   
  
  SIM800_Send_CMD( num ,"");
}

