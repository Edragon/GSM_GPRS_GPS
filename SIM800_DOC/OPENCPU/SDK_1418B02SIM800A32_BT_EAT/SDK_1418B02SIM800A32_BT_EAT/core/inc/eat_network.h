#if !defined (__EAT_NETWORK_H__)
#define __EAT_NETWORK_H__

#include "eat_type.h"

typedef enum
{
   EAT_REG_STATE_NOT_REGISTERED=0, //δע��
   EAT_REG_STATE_REGISTERED,          //��ע��
   EAT_REG_STATE_SEARCHING,           //������
   EAT_REG_STATE_REG_DENIED,          //ע��ʧ��
   EAT_REG_STATE_UNKNOWN,             //δ֪
   EAT_REG_STATE_REGISTERED_ROAMING //ע�ᵽ��������
} EatCregState_enum;

/*****************************************************************************
* Function   : eat_network_get_creg
* Description: get creg's value
* Parameters:  -
*  param1   
* Returns:     EatCregState_enum
* NOTE
*****************************************************************************/
extern EatCregState_enum (* const eat_network_get_creg)(void);

/*****************************************************************************
* Function   : eat_network_get_ber
* Description: get ber's value
* Parameters:  -
*  param1   
* Returns:     ber's value
* NOTE
*****************************************************************************/
extern s32 (* const eat_network_get_ber)(void);

/*****************************************************************************
* Function   : eat_network_get_csq
* Description: get csq's value
* Parameters:  -
*  param1   
* Returns:     csq's value
* NOTE
*****************************************************************************/
extern s32 (* const eat_network_get_csq)(void);

/*****************************************************************************
* Function   : eat_network_get_cgatt
* Description:
*   get the cgatt status 
* Parameters:
*  void
* Returns:
*  1 �� gprs attach success
*  0�� gprs attach fail
* NOTE
*****************************************************************************/
extern u8 (* const eat_network_get_cgatt)(void);


#endif
