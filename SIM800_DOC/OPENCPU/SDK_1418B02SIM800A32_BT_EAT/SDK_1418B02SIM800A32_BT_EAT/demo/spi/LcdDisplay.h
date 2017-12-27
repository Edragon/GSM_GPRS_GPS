#if !defined(__LCD_DISPLAY_H__)
#define __LCD_DISPLAY_H__

#include "eat_interface.h"
#include "lcd.h"
#include "LcdDataFont.h"

#define LCD_ALIGN_V       0x03
#define LCD_ALIGN_V_UP    0x01
#define LCD_ALIGN_V_DOWN  0x02
#define LCD_ALIGN_H       0x30
#define LCD_ALIGN_H_LEFT  0x10
#define LCD_ALIGN_H_RIGHT 0x20

extern void LcdDisplayStr(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, FontType_Enum font, u8 align, const char *str, eat_bool update);
/************************
   ����: ��ʾһ��ASCII�ַ�
   xStart: ���Ͻ�x����
   yStart: ���Ͻ�y����
   xEnd: ���½�x����
   yEnd: ���½�y����
   font: ʹ�õ�����
   align: ����ѡ��
   str: ��ʾ�ַ���
   update: �Ƿ���ʾ���������LCD; EAT_TRUE-���, EAT_FALSE-�����
 */
extern void LcdDisplayClear(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, eat_bool update); 
/************************
   ����: ���һ����ʾ����
   xStart: ���Ͻ�x����
   yStart: ���Ͻ�x����
   xEnd: ���½�x����
   yEnd: ���½�x����
   update: �Ƿ���ʾ���������LCD; EAT_TRUE-���, EAT_FALSE-�����
 */
extern void LcdDisplaySet(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, eat_bool update); 
/************************
   ����: ����һ����ʾ����
   xStart: ���Ͻ�x����
   yStart: ���Ͻ�x����
   xEnd: ���½�x����
   yEnd: ���½�x����
   update: �Ƿ���ʾ���������LCD; EAT_TRUE-���, EAT_FALSE-�����
 */
extern void LcdUpdate(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd);
/************************
   ����: ��һ����ʾ��������������LCD
   xStart: ���Ͻ�x����
   yStart: ���Ͻ�x����
   xEnd: ���½�x����
   yEnd: ���½�x����
 */
extern void LcdUpdateAll(void);
/************************
   ����: �����������������LCD
 */

extern void LcdDisplayStrEn(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, u8 align, const unsigned short *str, eat_bool update);
extern void LcdDisplayStrCn(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, u8 align, const unsigned short *str, eat_bool update);

#endif

