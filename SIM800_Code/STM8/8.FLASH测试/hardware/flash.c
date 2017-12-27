//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#include "stm8s.h"
#include "flash.h"

u8 WriteBuf[FLASH_BLOCK_SIZE] = "this is content:2015/08/22";
u8 ReadBuf[FLASH_BLOCK_SIZE] = "";

void Flash_Init(void)
{
  /* Define flash programming Time*/
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  /* Unlock Data memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA); 
}

void Flash_Write(void)
{
  u8 block ;

  block = 0; /* block 0 is first block of Data memory: address is 0x4000 */
    
  FLASH_ProgramBlock(block, FLASH_MEMTYPE_DATA, FLASH_PROGRAMMODE_STANDARD, WriteBuf);  

  FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
}

void Flash_Read(void)
{
  u16 start_add, stop_add, i = 0 ;   

  start_add = FLASH_DATA_START_PHYSICAL_ADDRESS;  /* block 0 start address */
  stop_add = FLASH_DATA_START_PHYSICAL_ADDRESS + FLASH_BLOCK_SIZE;

  for ( ; start_add < stop_add; start_add++)
  {
    ReadBuf[i++] = FLASH_ReadByte( start_add );
  }
}

