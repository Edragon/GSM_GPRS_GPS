#ifndef _EAT_INPUTMETHOD_H_
#define _EAT_INPUTMETHOD_H_

/***************************************************************************
* Include Files
***************************************************************************/

#include "eat_type.h"

/***************************************************************************
* Macros
***************************************************************************/

/***************************** ime key value define begin******************************/
/* digital : 0~9 */
#define WDK_NORMAL_0		0x10
#define WDK_NORMAL_1		0x11
#define WDK_NORMAL_2		0x12
#define WDK_NORMAL_3		0x13
#define WDK_NORMAL_4		0x14
#define WDK_NORMAL_5		0x15
#define WDK_NORMAL_6		0x16
#define WDK_NORMAL_7		0x17
#define WDK_NORMAL_8		0x18
#define WDK_NORMAL_9		0x19

/* letter : A~Z */

#define WDK_NORMAL_LETTER   0xB0					    
#define WDK_NORMAL_A		(WDK_NORMAL_LETTER+0x01)	//  1
#define WDK_NORMAL_B		(WDK_NORMAL_LETTER+0x02)    //  2
#define WDK_NORMAL_C		(WDK_NORMAL_LETTER+0x03)    //  3
#define WDK_NORMAL_D		(WDK_NORMAL_LETTER+0x04)	//  4
#define WDK_NORMAL_E		(WDK_NORMAL_LETTER+0x05)    //  5
#define WDK_NORMAL_F		(WDK_NORMAL_LETTER+0x06)	//  6
#define WDK_NORMAL_G		(WDK_NORMAL_LETTER+0x07)	//  7
#define WDK_NORMAL_H		(WDK_NORMAL_LETTER+0x08)	//  8
#define WDK_NORMAL_I		(WDK_NORMAL_LETTER+0x09)	//  9
#define WDK_NORMAL_J		(WDK_NORMAL_LETTER+0x0A)	//  10
#define WDK_NORMAL_K		(WDK_NORMAL_LETTER+0x0B)	//  11
#define WDK_NORMAL_L		(WDK_NORMAL_LETTER+0x0C)	//  12
#define WDK_NORMAL_M		(WDK_NORMAL_LETTER+0x0D)	//  13
#define WDK_NORMAL_N		(WDK_NORMAL_LETTER+0x0E)	//  14
#define WDK_NORMAL_O		(WDK_NORMAL_LETTER+0x0F)	//  15
#define WDK_NORMAL_P		(WDK_NORMAL_LETTER+0x10)	//  16
#define WDK_NORMAL_Q		(WDK_NORMAL_LETTER+0x11)	//  17
#define WDK_NORMAL_R		(WDK_NORMAL_LETTER+0x12)	//  18
#define WDK_NORMAL_S		(WDK_NORMAL_LETTER+0x13)	//  19
#define WDK_NORMAL_T		(WDK_NORMAL_LETTER+0x14)	//  20
#define WDK_NORMAL_U		(WDK_NORMAL_LETTER+0x15)	//  21
#define WDK_NORMAL_V		(WDK_NORMAL_LETTER+0x16)	//  22
#define WDK_NORMAL_W		(WDK_NORMAL_LETTER+0x17)	//  23
#define WDK_NORMAL_X		(WDK_NORMAL_LETTER+0x18)	//  24
#define WDK_NORMAL_Y		(WDK_NORMAL_LETTER+0x19)	//  25
#define WDK_NORMAL_Z		(WDK_NORMAL_LETTER+0x1A)    //  26

/* function key */
#define WDK_STAR			0x1A	/* "*" */
#define WDK_SHARP			0x1B	/* "#" */
#define WDK_UP				0x1C	/* up */
#define WDK_DOWN			0x1D	/* down */
#define WDK_LEFT				0x1E	/* left */
#define WDK_RIGHT			0x1F	/* right */
#define WDK_C				0x20	/* back */
#define WDK_OK				0x21	/* ok */
#define WDK_MENU			0x22	/* FN --full-keybroad */
#define WDK_SOFTLEFT		0x23	/* left softkey */
#define WDK_SOFTRIGHT		0x24	/* right softkey */
#define WDK_SHIFT			0x3A	/* Shift */
#define WDK_CAPLOCK		0x3B	/* caplock */
#define WDK_SEPERATER		0x3C	/* seperater */
#define WDK_FUNC			0x3D	/* FN */
#define WDK_ENTER			0x3E	/* enter */
#define WDK_ALT 				0x3F	/* Alt */
#define WDK_CTRL			0x40	/* Ctrl */
#define WDK_SPACE			0x41	/* space */
#define WDK_SYMBOL			0x42	/* symbol */
#define WDK_SWITCHIME       	0x43	/*switch inputmethod type*/
/***************************** ime key value define end******************************/



/***************************************************************************
* Types
***************************************************************************/

typedef enum{
	EAT_IME_DIGIT,                 /*digital*/
	EAT_IME_ENGLISH_ABC,    /*english abc*/
	EAT_IME_CN_FULLPINYIN,  /*chinese full pinyin*/
	EAT_IME_CN_CPINYIN,       /*chinese pure pinyin*/
	EAT_IME_ENGLISH,            /*english*/
	EAT_IME_CN_BIHUA,         /*chinese bihua*/
	EAT_IME_MAX
}EAT_IME_TYPE;

typedef struct _IME_KEY_MAP
{
	u32 uEatValue;	/*EatKey_enum��ֵ*/
	u32 uIMEValue;	/*���뷨��Ӧ��ֵ*/
}IME_KEY_MAP;


#define MAX_KEYS_LEN				24		/* ����ļ����������ֵ	*/
#define MAX_CHINESE_WORD_LEN		8		/* ���ĵ��ʵ���󳤶� */
#define MAX_KEY_LETTERCOUNT		26		/* ���ļ�λ�ϵ���ĸ�� */
#define MAX_PINYIN_LENGTH			28		/* ÿ��ƴ������󳤶� */
#define MAX_ONEPAGE_RECORD		20		/* ÿҳ��ѡ���������� */
#define MAX_DISPLAY_LINE			3       	/* ���뷨�����ʾ���� */
#define MAX_MESSAGE_LEN			60		/* ��ʾ��Ϣ�ĳ��� */

typedef struct WD_imeResult_t
{
	/* original key code */
	/* ԭʼ�������� */
	u16 inputKeys[MAX_KEYS_LEN+3];

	/* output key code: all "inputKeys" are converted into output keys' code*/
	/* ����������У���������ʾ������ļ�������Ӧ��ƴ����ע�������ģ�
	�ü������ѱ��ָ��ģ��������Ĵ����룩�������滻��ƴ����ע����(����ȷ�ϵ���) */
	u16 outputKeys[MAX_KEYS_LEN*MAX_CHINESE_WORD_LEN+1];

	/* current edit word`s focus */
	/* ��ǰ�Ľ��㡣
	���ģ���ǰ�༭�ļ����(�ڼ�������)�������ǰ�༭���ǵڼ�����ĸ */
	u8 idxFocus;

	/* current page of selecting words */
	/*��ǰ��ҳ��(��0��ʼ)���������ж��Ƿ��Ѿ��������һҳ���Ƿ���ʾ�·�ҳ��ͷ */	
	u8 curPage;

	/* Chinese inputmethod types: all candidate pinyin
	    English inputmethod types: all candidate characters */
	/* ���ģ�����ƴ����ע�����뷨��ǰ�����ֵļ�������Ӧ�����е�ƴ����ע����ϣ�
		  ������ƴ���뷨���ڵ�һ��ʱ��ʾ���б�ѡ��ĸ��
	    ���ģ���ǰ�����������Ӧ��������ĸ��*/
	u16 letterSelecting[MAX_KEY_LETTERCOUNT][MAX_PINYIN_LENGTH+1];

	/* Current index of "letterSelecting" */
	/* ��ǰ��ѡ������ĸ�����,��0��ʼ */
	u8 idxLetterSelecting;
	
	/* All candidate words */
	/* ��ѡ������MAX_ONEPAGE_RECORD�
	���ģ�����Ѳ���Ĵʻ��֣�ÿ���ִ�һ����
	���ģ��������������ĵ��ʣ����Unicode��*/
	u16 selecting[MAX_ONEPAGE_RECORD][MAX_KEYS_LEN+3];	
	
	/* Current index of all candidate words */
	/* ��ǰ��ѡ����ţ�,��0��ʼ */
	u8 idxSelecting;
	
	/* Selected word(s) of this time,it will be clear before next response of input key */
	/* ��ȷ���ִʣ����ظ����ú��� */
	u16 output[MAX_KEYS_LEN*MAX_CHINESE_WORD_LEN+1];
	
	/* The value of return when input key respond failed */
	/* ���ؼ��룺���뷨������İ�����ֱ�Ӵ��أ�����ڷǷ������������󱨾� */
	u16 returnKey;					   
	
	/* Inputmethod type`s value */
	/* ��ǰ���뷨������: �������뷨���� */
	u8 imeType; 				   
	
	/* Inputmethod type`s name */
	/* ���뷨������ʾ�ַ�����������GB�����unicode�룬������Unicode�롣*/
	u16 imeString[4];				   
	
	/* Invalid data at this version */
	/* ��ʾ��Ϣ������ʾ����ʾ��Ϣ��������GB�룬������Unicode��
	�˰汾���ṩ��ʾ��Ϣ���� */
	u16 message[MAX_DISPLAY_LINE][MAX_MESSAGE_LEN];  
	
	/* Invalid data at this version */
	/*�˰汾�����ã�*/
	u8 resultEmpty;
	
	/* Current status of input method */
	/*���뷨��ǰ��״̬ */
	u8 imeStatus;
	
	/* Previous status of input method */
	/*���뷨ǰһ��״̬ */
	u8 preStatus;

	/* 0: Lowercase state;
	    1: Capitalize the first letterl;
	    2: Capital state*/
	/*��ʾ��Сд״̬0ΪСд״̬��1Ϊ����ĸ��д��2Ϊȫ��д״̬ */
	u8 capLock;

	/* Amount of "selecting" */
	/*ÿ���������Ľ����*/
	u8 selectingCount;

	/* 0:chinese mix rank,otherwise 1 */
	/*�����Ƿ���ţ�����һ����־Ϊָʾ���������ı�ѡ���Ƿ���ţ�����Ϊ0������Ϊ1 */
	u8 cnMix;

	/*  */
	/*�������Ľ��� */
	u8 screenFocus;

	/*  */
	/*������ʱ�˱�ʶ��1 */
	u8 numFlag;

	/*  */
	/*����DIY״̬Ϊ1������Ϊ0 */
	u8 statusDIY;

	/* This value equal 1 when switch to English with "#" key */
	/*��#���޷��л���Ӣ��ʱΪ1*/
	u8 CnEnglishFlag;

	/* Previous input method type */
	/*ǰһ���뷨*/
	u8 preIme;

	/*  */
	/*ȷ�����Ĵ���������*/
	u8 confirmCnWordNum;	
} WD_imeResult;

typedef enum{
	EAT_IME_SUCCESS,
	EAT_IME_TYPE_ERR,    //���뷨���Ͳ����ڡ�Failed type of inputmethod
	EAT_IME_ERR_AMOUNT_BEYOND,    //���뷨����ʱ�ĸ����������ֵ��Overmuch inputmethod types (Overmuch) when start ime	
	EAT_UNKNOWN_ERR,    //��ӿڵ���ʧ�ܡ�Interface called failed
	EAT_INIT_ERR		//���뷨ע��key mapʧ�ܡ�Failed to register inputmethod key map.
	
}EAT_IME_ERROR;


/***************************************************************************
* External Functions Prototypes
***************************************************************************/

/*****************************************************************************
* Function :  eat_ime_register_keymap
 * Description
 *    You must config a key map array before calling eat_ime_entry. Call this interface to register it.
 * Parameters
 *   pKeyMapTable[in] : The array of key map.
 *   u8TableCount[in] : Length of pKeyMapTable.
 * Returns
 *         EAT_TRUE: Mean register successful,otherwise return EAT_FALSE.
 * attention
 *     1. This interface must called before interface of eat_ime_entry called at the first time.
*****************************************************************************/
extern eat_bool (* const eat_ime_register_keymap)(IME_KEY_MAP * pKeyMapTable, u8 u8TableCount);

/*****************************************************************************
* Function :  eat_ime_entry
 * Description
 *    Start ime.
 * Parameters
 *   ime_types[in] : Input method type`s array.
 *   ime_types_count[in] : ime_types length.
 *   default_ime_type[in] : The first input method type when inputmethod starts.
 *   displayed_words_perline[in] :  Max number of words displayed when a input key responsed(to response a input key) 
 * Returns
 *         EAT_IME_ERROR
 * attention
 *     1. You must call eat_ime_register_keymap at least one time before this interface used.
*****************************************************************************/
extern s32 (* const eat_ime_entry)(EAT_IME_TYPE ime_types[],u8 ime_types_count,EAT_IME_TYPE default_ime_type,u8 displayed_words_perline);

/*****************************************************************************
* Function :  eat_ime_exit
 * Description
 *    Exit ime.
 * Parameters
 * Returns
*****************************************************************************/

extern void (* const eat_ime_exit)();

#endif	/*_EAT_INPUTMETHOD_H_*/



