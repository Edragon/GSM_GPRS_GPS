/********************************************************************
 *                Copyright Simcom(shanghai)co. Ltd.                               *
 *---------------------------------------------------------------------
 * FileName      :   eat_wmmp.h
 * version       :   0.10
 * Description   :   eat wmmp interface define for customer
 * Authors       :   Maobin  
 * Notes         :
 *---------------------------------------------------------------------
 *
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2013-01-11, maobin, create originally.
 *
 ********************************************************************/
/********************************************************************
 * Include Files
 ********************************************************************/
#ifndef _EAT_WMMP_H_
#define _EAT_WMMP_H_
#include "eat_type.h"
/********************************************************************
 * Macros
 ********************************************************************/

/********************************************************************
 * Types
 ********************************************************************/
typedef enum SecuritySimStateTag
{
	SECURITY_SIM_STATE_OK = 0,
	SECURITY_SIM_STATE_PIN,
	SECURITY_SIM_STATE_PUK,
	SECURITY_SIM_STATE_OTHER
}SecuritySimState_e;
typedef struct
{
	u8  BearRef;
	u8 UpResult;
	u32 LocalIP;
	u32 DNS1;
	u32 DNS2;
}EatTcpipStartCnf_st;

typedef struct
{
    u8  BearRef;
    u8 DownResult;
}EatTcpipShoutdownCnf_st;


typedef struct
{
    u8  BearRef;
}EatTcpipShutdownInd_st;


typedef struct
{
     u8  BearRef;
     u8  ConnRef;
}EatTcpipCloseCnf_st;

typedef struct
{
    u8  BearRef;
    u8  ConnRef;
    u8  OpenResult;
}EatTcpipActiveopenCnf_st;

typedef struct
{
    u8  BearRef;
    u8  ConnRef;
    u32 sizeProcessed;
}EatTcpipTxDataCnf_st;

typedef struct
{
    u8  BearRef;
    u8  ConnRef;
    u32 sizeSended;
}EatTcpipTxDataEnabledInd_st;

typedef struct
{
    u8  BearRef;
    u8  ConnRef;
    u32 Size;
    u8* Data;
    u16 RemotePort;
    u32 RemoteAddr;
}EatTcpipRxDataReq_st;

typedef enum WPIRtcStatusTag
{
  WPI_RTC_STATUS_OK,
  WPI_RTC_STATUS_FAILURE      /* RTC is not functioning correctly */
}
WPIRtcStatus_e;

typedef struct
{
 u8 gprs_state;
}EatNwAttachInd_st;

typedef struct WPIRtcDateAndTimeTag
{
  u16 year;
  u8  month;
  u8  day;
  u8 hours;
  u8 minutes;
  u8 seconds;
}WPIRtcDateAndTime;

typedef enum IpUpResultTag
{
    IP_UP_OK = 0,
    IP_UP_OK_BY_OTHER  = 1,
    IP_UP_ERROR_NO_BEARER = 2,
    IP_UP_ERROR_INVALIDPARA = 3,
    IP_UP_ERROR_OTHER = 4,
    INVALID_IP_UP
} IpUpResult_e;

typedef enum IpDownResultTag
{
    IP_DOWN_OK   = 0,
    IP_DOWN_FAIL_NO_RIGHT = 1,
    IP_DOWN_ERROR = 2,
    INVALID_IP_DOWN
} IpDownResult_e;

typedef enum SocketOpenResultTag
{
    SOCKET_OPEN_OK_CONNECT   = 0,
    SOCKET_OPEN_ERROR_BY_SOCKET = 1,
    SOCKET_OPEN_ERROR_BY_BIND = 2,
    SOCKET_OPEN_ERROR_BY_CONNECT = 3,   //connect -1，被网络端拒绝，连接超时
    SOCKET_OPEN_ERROR_BY_LISTEN = 4,   //已经监听过再监听等
    SOCKET_OPEN_ERROR_BY_NETWORK = 5,
    SOCKET_OPEN_ERROR =6,
    SOCKET_OPEN_OK_LISTEN = 7,
    SOCKET_OPEN_OK_ACCEPT = 8,
    SOCKET_OPEN_ACCEPT_NO_CONREF = 9,
    INVALID_SOCKET_OPEN
} SocketOpenResult_e;

typedef enum
{
   REG_STATE_NOT_REGISTERED=0,
   REG_STATE_REGISTERED,
   REG_STATE_SEARCHING,
   REG_STATE_REG_DENIED,
   REG_STATE_UNKNOWN,
   REG_STATE_NOT_ACTIVE
} reg_state_enum;

/********************************************************************
 * Extern Variables (Extern /Global)
 ********************************************************************/

/********************************************************************
 * Local Variables:  STATIC
 ********************************************************************/

/********************************************************************
 * External Functions declaration
 ********************************************************************/

/********************************************************************
 * Local Function declaration
 ********************************************************************/
extern u32 (* const eat_WPI_GetCBC)(void);
extern eat_bool (* const eat_WPI_GetICCID)(u8*iccid_str);
extern eat_bool (* const eat_WPI_GetIMEI)(char* pImei, u16 len);
extern eat_bool (* const eat_WPI_GetIMSI)(char* IMSI_p, u16 len);
extern unsigned short (* const eat_WPI_GetCellID)(void);
extern unsigned short (* const eat_WPI_GetLAC)(void);
extern u8 (* const eat_WPI_GetCSQLevel)(u8 *ber);
extern u32 (* const eat_WPI_GetCurrentTime)(WPIRtcDateAndTime *dateAndTime_p, WPIRtcStatus_e *status_p);
extern u32 (* const eat_WPI_rand)(void ) ;
extern void (* const eat_WPI_hex_printf)(char *input,u16 len);
extern void (* const eat_WPI_WriteUartData)(char *data,u16 datalength);

extern void (* const eat_WPI_send_urc)(const char *fmt,...);
extern void (* const eat_WPI_SystemReboot)(void);

extern u8 (* const eat_WPI_GetSimState)(void);
extern void(* const eat_wmmp_get_imsi_init)(void) ;
extern eat_bool(* const eat_WPI_MODIFY_IMEI_IMSI)(char* newImei,char* newImsi);
extern eat_bool(* const eat_WPI_SecurityAutoCpin)(char* cpin);
extern eat_bool(* const eat_WPI_SecurityChangeCpin)(char* cpin1,char *cpin2);
extern eat_bool(* const eat_WPI_SecurityEnableCpin)(char* cpin);
extern eat_bool(* const eat_WPI_SecurityDisableCPIN)(char * cpin);
extern void (* const eat_WPI_sendRxDataCnf)(u8 bearRef,u8 connRef,u32 sizeProcessed);

extern void (*const eat_WPI_CreateConnect)(u8 apn_type,u8 type,u8 connRef,u32 ipaddress, u16 sockPort);
extern void (*const eat_WPI_send_net_data)( u8 apn_type,u8 connRef, s32 bytesSent,u8 *pData);
extern u32 (*const eat_WPI_Send) (u8 apn_type,u8 connRef, void *buf_p, u16 len);
extern void (*const  eat_WPI_CloseSocket) (u8 apn_type,u8 connRef);
extern void (*const eat_WPI_ActiveGPRS)(u8 apn_type,u8 *server,u8 *pwd,u8*user);
extern void (*const eat_WPI_DeactiveGPRS)(u8 apn_type);
extern void (*const eat_WPI_sendRxDataCnf)(u8 bearRef,u8 connRef,u32 sizeProcessed);

#endif // _EAT_WMMP_H_
