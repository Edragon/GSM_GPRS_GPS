#ifndef __LCD_DATA_FONT_H__
#define __LCD_DATA_FONT_H__

#include "eat_interface.h"

typedef struct 
{
  char charCode;              /* �ַ� */
  u8 width;                   /* �ַ��Ŀ�� */
  u8 height;                  /* �ַ��ĸ߶� */
  u16 len;                    /* �ַ���ʾ���ݵĳ��� */
  u32 dataOffset;             /* �ַ���ʾ�������ֿ��е�ƫ���� */
} FontInfo_Struct;            /* �ֿ���Ϣ */

typedef struct 
{
  u16 charNum;                 /* �ֿ�������ַ����� */
  const FontInfo_Struct *info; /* �ֿ���Ϣ */
  const u8 *data;              /* �ֿ����ʾ����, ȡģѡ��: ����, ����(��λ��ǰ), ���� */
} FontInfoTable_Struct;        

typedef enum 
{
  FONT_TYPE_MAIN,
  FONT_TYPE_BIG,
  FONT_TYPE_NUM
} FontType_Enum; /* ���� */

extern const u8 * FontFind(char c, FontType_Enum font, FontInfo_Struct *FontInfo);

#endif

