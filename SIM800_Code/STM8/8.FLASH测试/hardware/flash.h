//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __FLASH_H
#define __FLASH_H

extern u8 ReadBuf[FLASH_BLOCK_SIZE] ;

void Flash_Init(void);
void Flash_Write(void);
void Flash_Read(void);

#endif