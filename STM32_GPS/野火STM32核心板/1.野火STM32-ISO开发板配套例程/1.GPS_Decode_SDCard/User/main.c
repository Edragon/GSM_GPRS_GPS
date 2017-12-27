/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-08-xx
  * @brief   gps������ʾ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "gps_config.h"
#include "bsp_sdio_sdcard.h"
#include "diskio.h"
#include "ff.h"
#include <string.h>


		

extern void nmea_decode_test(void);

/*
 * ���������ڶ�SD���ڵ�GPS�����ļ����н���
 * 
 */
int main(void)
{
    /* ����USART1 ���������printf������Ϣ*/
    USART1_Config();    
  
    /* ��ʼ��GPSģ��ʹ�õĽӿ� */
    GPS_Config();
    
    printf("\r\nҰ��WF-NEO-6M GPSģ���������\r\n"); 
  
    /* GPS������� */
    nmea_decode_test();

    while(1);
  
  
}



