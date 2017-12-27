/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-08-xx
  * @brief   gps解码演示程序
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-STM32 开发板
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
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
 * 本工程用于对SD卡内的GPS数据文件进行解码
 * 
 */
int main(void)
{
    /* 配置USART1 用于向电脑printf调试信息*/
    USART1_Config();    
  
    /* 初始化GPS模块使用的接口 */
    GPS_Config();
    
    printf("\r\n野火WF-NEO-6M GPS模块测试例程\r\n"); 
  
    /* GPS解码测试 */
    nmea_decode_test();

    while(1);
  
  
}



