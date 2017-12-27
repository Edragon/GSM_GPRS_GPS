/********************************************************************
 *                Copyright Simcom(shanghai)co. Ltd.                               *
 *---------------------------------------------------------------------
 * FileName      :   eat_at.h
 * version       :   0.10
 * Description   :   eat AT command parser interface
 * Authors       :   Maobin  
 * Notes         :
 *---------------------------------------------------------------------
 *
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2013-01-11, maobin, create originally.
 *
 ********************************************************************/
#ifndef _EAT_AT_H_
#define _EAT_AT_H_
/********************************************************************
 * Include Files
 ********************************************************************/
#include "eat_type.h"
/********************************************************************
 * Macros
 ********************************************************************/
#define RMMI_SHORT_RSP_LEN          80
/********************************************************************
 * Types
 ********************************************************************/
/* Define the equipment to return the string mapping. */
typedef enum 
{
    EAT_EQ_ME_MANUFACTURE_ID      = 0 ,
    EAT_EQ_ME_MODEL_ID                ,
    EAT_EQ_ME_REVISION_ID             ,
    EAT_EQ_ME_SERIAL_NUMBER_ID        ,
    EAT_EQ_ME_SERIAL_NUMBER_SIM2_ID   ,
    EAT_EQ_ME_GLOBAL_OBJECT_ID        ,
    EAT_EQ_ME_MANUFACTURE_INFO_ID     ,
    EAT_EQ_ME_COMP_CAP_LIST_ID        ,
#if defined(__SIMCOM_INFO__) /*SIMCOM YangMing 2012-11-22 CR_MKBug00014357_yang.ming*/
    EAT_EQ_ME_BUILD_ID                , 
#endif
    EAT_MAX_ME_ID_NUM                    
} eat_equip_id_enum;

/* GSM0707  9.2 +CME ERROR*/
typedef enum
{
    /*because the rang 0-150 have been used by mtk, these value must same as mtk except the enum name, 
      * or it cmee string will not right*/
   
    /*SIMCOM: in the range 0 - 100 are reserved for  general errors, same as sim300 and sim900. begin */
    
      
    /* GSM0707 Sec 9.2.1 */
    EAT_CME_PHONE_FAILURE = 0,

    EAT_CME_NO_MS_CONNECTION = 1,
    EAT_CME_PHONE_LINK_RESERVED=2,

    EAT_CME_OPERATION_NOT_ALLOWED_ERR = 3,
    EAT_CME_OPERATION_NOT_SUPPORTED = 4,
    EAT_CME_PH_SIM_PIN_REQUIRED = 5,
    EAT_CME_PH_FSIM_PIN_REQUIRED = 6, //Kinki: SIM-ME lock [MAUI_01264994]
    EAT_CME_PH_FSIM_PUK_REQUIRED = 7, //Kinki: SIM-ME lock [MAUI_01264994]
    EAT_CME_SIM_NOT_INSERTED = 10,
    EAT_CME_SIM_PIN_REQUIRED = 11,
    EAT_CME_SIM_PUK_REQUIRED = 12,
    EAT_CME_SIM_FAILURE = 13,
    EAT_CME_SIM_BUSY = 14,
    EAT_CME_SIM_WRONG = 15,
    EAT_CME_INCORRECT_PASSWD = 16,
    EAT_CME_SIM_PIN2_REQUIRED = 17,
    EAT_CME_SIM_PUK2_REQUIRED = 18,
    EAT_CME_MEM_FULL = 20,
    EAT_CME_INVALID_INDEX = 21,
    EAT_CME_NO_FOUND = 22,
    EAT_CME_MEMORY_FAILURE =23,

    EAT_CME_TEXT_ERRSTRING_TOO_LONG_ERR = 24,
    EAT_CME_INVALID_CHARACTERS_IN_TEXT_ERRSTRING_ERR = 25,
    EAT_CME_DIAL_ERRSTRING_TOO_LONG_ERR = 26, 
    EAT_CME_INVALID_CHARACTERS_IN_DIAL_ERRSTRING_ERR = 27,
    EAT_CME_NO_NW_SERVICE = 30,
    EAT_CME_NETWORK_TIMEOUT = 31,
    EAT_CME_NETWORK_NOT_ALLOWED = 32,
    EAT_CME_NW_PERSON_PIN_REQUIRED = 40,
    EAT_CME_NW_PERSON_PUK_REQUIRED = 41,
    EAT_CME_NW_SUB_PERSON_PIN_REQUIRED = 42,
    EAT_CME_NW_SUB_PERSON_PUK_REQUIRED = 43,
    EAT_CME_SP_PERSON_PIN_REQUIRED = 44,
    EAT_CME_SP_PERSON_PUK_REQUIRED = 45,
    EAT_CME_CORP_PERSON_PIN_REQUIRED = 46,
    EAT_CME_CORP_PERSON_PUK_REQUIRED = 47,
    EAT_CME_UNKNOWN = 100,
    /*SIMCOM: in the range 0 - 100 are reserved for  general errors, same as sim300 and sim900.  end*/

    /*SIMCOM: Other values in the range 101 - 150 are reserved for use by GPRS,same as sim300 and sim900. begin*/
    EAT_CME_ILLEGAL_MS = 103,
    EAT_CME_ILLEGAL_ME = 106,
    EAT_CME_GPRS_NOT_ALLOWED = 107,
    EAT_CME_PLMN_NOT_ALLOWED = 111,
    EAT_CME_LA_NOT_ALLOWED = 112,
    EAT_CME_ROAMING_AREA_NOT_ALLOWED = 113,
    EAT_CME_SERV_OPTION_NOT_SUPPORTED = 132,
    EAT_CME_REQ_SERV_OPTION_NOT_SUBSCRIBED = 133,
    EAT_CME_SERV_OPTION_TEMP_OUT_OF_ORDER = 134,
    EAT_CME_GPRS_UNSPECIFIED_ERROR = 148,
    EAT_CME_PDP_AUTH_FAIL = 149,
    EAT_CME_INVALID_MOBILE_CLASS = 150,
    /*SIMCOM: Other values in the range 101 - 150 are reserved for use by GPRS,same as sim300 and sim900. end*/

#if defined (__EAT_PING__)
      EAT_CME_DNS_RESOLVE_FAIL = 160,
      EAT_CME_SOCKET_OPEN_FAIL,
#endif /* __EAT_PING__ */

    /*SIMCOM: Other values in the range 171 - 193 are reserved for MMS error codes,same as sim300 and sim900. begin*/
#if defined(__EAT_MMS__)
    EAT_CME_MMS_BUSY = 171,
    EAT_CME_MMS_OVERSIZE,
    EAT_CME_MMS_OVERTIME,
    EAT_CME_MMS_NO_RECEIVER,
    EAT_CME_MMS_ADDRESS_FULL,
    EAT_CME_MMS_ADDRESS_NOT_FIND,
    EAT_CME_MMS_NETWORK_ERROR,
    EAT_CME_MMS_READ_SMS_FAILED,
    EAT_CME_MMS_NOT_PUSH_MESSAGE,
    EAT_CME_MMS_GPRS_DETACHED,
    EAT_CME_MMS_TCPIP_STACK_BUSY,
    EAT_CME_MMS_STORAGE_FULL,
    EAT_CME_MMS_EMPTY_BOX,
    EAT_CME_MMS_SAVE_FAILED,
    EAT_CME_MMS_EDIT_ENABLED,
    EAT_CME_MMS_EDIT_DISABLED,
    EAT_CME_MMS_EMPTY_CONTENT,
    EAT_CME_MMS_NOT_FIND_FILE,
    EAT_CME_MMS_RECV_FAILED,
    EAT_CME_MMS_READ_FAILED, 
    EAT_CME_MMS_NOT_NOTIFICATION_IND,
    EAT_CME_MMS_INCLOUSURE_OVERNUM,
    EAT_CME_MMS_UNKNOWN,
#endif
    /*SIMCOM: Other values in the range 171 - 193 are reserved for MMS error codes,same as sim300 and sim900. begin*/


    /*SIMCOM: Other values in the range 577 - 672 are nominally reserved for proprietary PPP error codes,same as sim300 and sim900 not defined. begin*/
    EAT_CME_GPRS_ACTIVATION_REJECTED_BY_GGSN = 577,
    EAT_CME_GPRS_UNSPECIFIED_ACTIVATION_REJECTION = 578,
    EAT_CME_GPRS_BAD_CODE_OR_PROTOCOL_REJECTION = 579,
    EAT_CME_GPRS_CANT_MODIFY_ADDRESS = 580,
    EAT_CME_GPRS_CHAP_CLOSE = 581,
    EAT_CME_GPRS_PROFILE_CID_CURRENTLY_UNAVAILABLE = 582,
    EAT_CME_GPRS_A_PROFILE_CID_IS_CURRENTLY_ACTIVE = 583,
    EAT_CME_GPRS_COMBINED_SERVICES_NOT_ALLOWED = 584,
    EAT_CME_GPRS_CONDITIONAL_IE_ERROR = 585,
    EAT_CME_GPRS_CONTEXT_ACTIVATION_REJECTED = 586,
    EAT_CME_GPRS_DUPLICATE_TI_RECEIVED = 587,
    EAT_CME_GPRS_FEATURE_NOT_SUPPORTED = 588,
    EAT_CME_GPRS_SERVICE_NOT_AVAILABLE = 589,
    EAT_CME_GPRS_UNKNOWN_IE_FROM_NETWORK = 590,
    EAT_CME_GPRS_IMPLICITLY_DETACHED = 591,
    EAT_CME_GPRS_INSUFFICIENT_RESOURCES = 592,
    EAT_CME_GPRS_INVALID_ACTIVATION_STATE_0_1 = 593,
    EAT_CME_GPRS_INVALID_ADDRESS_LENGTH = 594,
    EAT_CME_GPRS_INVALID_CHARACTER_IN_ADDRESS_STRING = 595,
    EAT_CME_GPRS_INVALID_CID_VALUE = 596,
    EAT_CME_GPRS_INVALID_DIAL_STRING_LENGTH = 597,
    EAT_CME_GPRS_MODE_VALUE_NOT_IN_RANGE = 598,
    EAT_CME_GPRS_INVALID_MAND_INFORMATION = 599,
    EAT_CME_GPRS_SMS_SERVICE_PREFERENCE_OUT_OF_RANGE = 600,
    EAT_CME_GPRS_INVALID_TI_VALUE = 601,
    EAT_CME_GPRS_IPCP_NEGOTIATION_TIMEOUT = 602,
    EAT_CME_GPRS_LCP_NEGOTIATION_TIMEOUT = 603,
    EAT_CME_GPRS_LLC_ERROR = 604,
    EAT_CME_GPRS_LLC_OR_SNDCP_FAILURE = 605,
    EAT_CME_GPRS_LOWER_LAYER_FAILURE = 606,
    EAT_CME_GPRS_MISSING_OR_UNKNOWN_APN = 607,
    EAT_CME_GPRS_MOBILE_NOT_READY = 608,
    EAT_CME_GPRS_MS_IDENTITY_NOT_IN_NETWORK = 609,
    EAT_CME_GPRS_MSC_TEMPORARILY_NOT_REACHABLE = 610,
    EAT_CME_GPRS_MESSAGE_INCOMPATIBLE_WITH_STATE = 611,
    EAT_CME_GPRS_MESSAGE_TYPE_INCOMPATIBLE_WITH_STATE = 612,
    EAT_CME_GPRS_UNKNOWN_MESSAGE_FROM_NETWORK = 613,
    EAT_CME_GPRS_NCP_CLOSE = 614,
    EAT_CME_GPRS_NETWORK_FAILURE = 615,
    EAT_CME_GPRS_NO_ECHO_REPLY = 616,
    EAT_CME_GPRS_NO_FREE_NSAPIS = 617,
    EAT_CME_GPRS_PROCESSING_OF_MULTIPLE_CIDS_NOT_SUPPORTED = 618,
    EAT_CME_GPRS_NO_PDP_CONTEXT_ACTIVATED = 619,
    EAT_CME_GPRS_NORMAL_TERMINATION = 620,
    EAT_CME_GPRS_NSAPI_ALREADY_USED = 621,
    EAT_CME_GPRS_ADDRESS_ELEMENT_OUT_OF_RANGE = 622,
    EAT_CME_GPRS_PAP_CLOSE = 623,
    EAT_CME_GPRS_PDP_CONTEXT_WO_TFT_ALREADY_ACTIVATED = 624,
    EAT_CME_GPRS_PDP_TYPE_NOT_SUPPORTED = 625,
    EAT_CME_GPRS_PEER_REFUSES_OUR_ACCM = 626,
    EAT_CME_GPRS_PEER_REFUSES_OUR_IP_ADDRESS = 627,
    EAT_CME_GPRS_PEER_REFUSES_OUR_MRU = 628,
    EAT_CME_GPRS_PEER_REQUESTED_CHAP = 629,
    EAT_CME_GPRS_PROFILE_CID_NOT_DEFINED = 630,
    EAT_CME_GPRS_UNSPECIFIED_PROTOCOL_ERROR = 631,
    EAT_CME_GPRS_QOS_NOT_ACCEPTED = 632,
    EAT_CME_GPRS_QOS_VALIDATION_FAIL = 633,
    EAT_CME_GPRS_REACTIVATION_REQUIRED = 634,
    EAT_CME_GPRS_REGULAR_DEACTIVATION = 635,
    EAT_CME_GPRS_SEMANTIC_ERROR_IN_TFT_OPERATION = 636,
    EAT_CME_GPRS_SEMANTIC_ERRORS_IN_PACKET_FILTER = 637,
    EAT_CME_GPRS_SEMANTICALLY_INCORRECT_MESSAGE = 638,
    EAT_CME_GPRS_SERVICE_TYPE_NOT_YET_AVAILABLE = 639,
    EAT_CME_GPRS_SYNTACTICAL_ERROR_IN_TFT_OPERATION = 640,
    EAT_CME_GPRS_SYNTACTICAL_ERRORS_IN_PACKET_FILTER = 641,
    EAT_CME_GPRS_TOO_MANY_RXJS = 642,
    EAT_CME_GPRS_UNKNOWN_PDP_ADDRESS_OR_TYPE = 643,
    EAT_CME_GPRS_UNKNOWN_PDP_CONTEXT = 644,
    EAT_CME_GPRS_USER_AUTHORIZATION_FAILED = 645,
    EAT_CME_GPRS_QOS_INVALID_PARAMETER = 646,
    /*SIMCOM: Other values in the range 577 - 672 are nominally reserved for proprietary PPP error codes,same as sim300 and sim900 not defined. end*/

    /*SIMCOM: Other values in the range 673 - 704 are nominally reserved for proprietary audio/speech error codes,same as sim300 and sim900 not defined. begin*/
    EAT_CME_AUDIO_MANAGER_NOT_READY = 673,
    EAT_CME_AUDIO_FORMAT_CANNOT_BE_CONFIGURED = 674,
    /*SIMCOM: Other values in the range 673 - 704 are nominally reserved for proprietary audio/speech error codes,same as sim300 and sim900 not defined. end*/


    /*SIMCOM:Other values in the range  705 - 720 are nominally reserved for proprietary STK error codes,same as sim300 and sim900 not defined. begin*/
    EAT_CME_SIM_TOOLKIT_MENU_HAS_NOT_BEEN_CONFIGURED = 705,
    EAT_CME_SIM_TOOLKIT_ALREADY_IN_USE = 706,
    EAT_CME_SIM_TOOLKIT_NOT_ENABLED = 707,
    /*SIMCOM: Other values in the range 705 - 720 are nominally reserved for proprietary STK error codes,same as sim300 and sim900 not defined. begin*/

    /*SIMCOM:Other values in the range 721 - 736 are nominally reserved for proprietary cursor error codes,sim300 and sim900 not defined. begin*/
    /*SIMCOM:Other values in the range 721 - 736 are nominally reserved for proprietary cursor error codes,sim300 and sim900 not defined. begin*/

    /*SIMCOM:Other values in the range 737 - 1024 are nominally reserved for proprietary general error codes,same as sim300 . begin*/

    /*SIMCOM: the range 737- 752 sim900 not defined. 753- 773 sim 900 is defined and same as sim300*/

    /* codes for AT+CSCS command */
    EAT_CME_CSCS_TYPE_NOT_SUPPORTED = 737,
    EAT_CME_CSCS_TYPE_NOT_FOUND = 738,

    /* codes for AT+CPOL command */
    EAT_CME_MUST_INCLUDE_FORMAT_WITH_OPER = 741,
    EAT_CME_INCORRECT_OPER_FORMAT = 742,
    EAT_CME_OPER_LENGTH_TOO_LONG = 743,
    EAT_CME_SIM_FULL = 744,
    EAT_CME_UNABLE_TO_CHANGE_PLMN_LIST = 745,
    EAT_CME_NETWORK_OPERATOR_NOT_RECOGNIZED = 746,

    /* codes for AT+CSIM command */
    EAT_CME_INVALID_COMMAND_LENGTH = 749,
    EAT_CME_INVALID_INPUT_STRING = 750,

    /* codes for AT+CRSM command */
    EAT_CME_MISSING_REQUIRED_CMD_PARAMETER = 753,
    EAT_CME_INVALID_SIM_COMMAND = 754,
    EAT_CME_INVALID_FILE_ID = 755,
    EAT_CME_MISSING_REQUIRED_P1_2_3_PARAMETER = 756,
    EAT_CME_INVALID_P1_2_3_PARAMETER = 757,
    EAT_CME_MISSING_REQUIRED_COMMAND_DATA = 758,
    EAT_CME_INVALID_CHARACTERS_IN_COMMAND_DATA = 759,

    /* miscellaneous codes */	
    EAT_CME_INVALID_INPUT_VALUE = 765,
    EAT_CME_UNSUPPORTED_VALUE_OR_MODE = 766,
    EAT_CME_OPERATION_FAILED = 767,
    EAT_CME_MULTIPLEXER_ALREADY_ACTIVE = 768,
    EAT_CME_UNABLE_TO_GET_CONTROL_OF_REQUIRED_MODULE = 769,
    EAT_CME_SIM_INVALID_NETWORK_REJECT = 770,
    EAT_CME_CALL_SETUP_IN_PROGRESS = 771,
    EAT_CME_SIM_POWERED_DOWN = 772,
    EAT_CME_SIM_FILE_NOT_PRESENT = 773,
    /*SIMCOM: the range 737- 752 sim900 not defined. 753- 773 sim 900 is defined and same as sim300*/

    EAT_CME_REFRESH_NET_TIME_FALIURE=774,  


    /*SIMCOM:Other values in the range 737 - 1024 are nominally reserved for proprietary general error codes,same as sim300. end*/
    EAT_PARAM_COUNT_NOT_ENOUGH = 791,
    EAT_PARAM_COUNT_BEYOND = 792,
    EAT_PARAM_VALUE_RANGE_BEYOND = 793,
    EAT_PARAM_TYPE_NOT_MATCH = 794,
    EAT_PARAM_FORMAT_INVALID = 795,
    /*SIMCOM DongBingQuan 2013-02-22 MKBug00015234 BEGIN*/
    EAT_PARAM_GET_NULL_PARAM = 796,
    /*SIMCOM DongBingQuan 2013-02-22 MKBug00015234 END*/

    /*SIMCOM chenhaibing 2013-02-01 MKBug00015064 BEGIN*/
    /*SIMCOM: the range 809 -830 IS used by MTK RMMI_ERR_COMMAND_CONFLICT - RMMI_ERR_AT_NOT_ALLOWED_BY_CUSTOMER*/
    EAT_CME_NO_ERR = 810,
    EAT_CME_UNRECOGNIZED_CMD, 
    EAT_CME_RETURN_ERROR,
    EAT_CME_SYNTEX_ERROR,
    EAT_CME_UNSPECIFIED , //unspecified parsing error
    EAT_CME_DATA_TRANSFER_ALREADY,
    EAT_CME_ACTION_ALREADY,
    EAT_CME_NOT_AT_CMD,
    EAT_CME_MULTI_CMD_TOO_LONG,
    EAT_CME_ABORT_COPS,
    EAT_CME_NO_CALL_DISC,
    EAT_CME_BT_SAP_UNDEFINED,
    EAT_CME_BT_SAP_NOT_ACCESSIBL,
    EAT_CME_BT_SAP_CARD_REMOVED,
    EAT_CME_AT_NOT_ALLOWED_BY_CUSTOMER, 
    /*SIMCOM: the range 800 -820 IS used by MTK RMMI_ERR_UNRECOGNIZED_CMD - RMMI_ERR_AT_NOT_ALLOWED_BY_CUSTOMER*/
    /*SIMCOM chenhaibing 2013-02-01 MKBug00015064s END*/

    /*SIMCOM:Other values in the range 1024 - 65535 are nominally reserved,same as sim300. begin*/

    /*SIMCOM:Other values in the range 1024 - 65535 are nominally reserved,same as sim300. end*/


    EAT_CME_ERROR_TOTAL_NUM = 0xFFFF

    /*SIMCOM:Other values in the range 1024 - 65535 are nominally reserved,same as sim300. end*/

}eat_cmee_err_id_enum;

/*SIMCOM chenhaibing 2012-12-05 MKBug00014441 BEGIN*/

/*GSM0705 +cms error*/
typedef enum
{

    /*0-127 define by GSM04.11 Annex E-2 */
    
// Class(000): 0-15 -> normal event             CC+SS    SMS
   EAT_CMS_UNASSIGNED_NUM                     = 1,  // X       X
   EAT_CMS_NO_ROUTE_TO_DESTINATION            = 3,  // X
   EAT_CMS_CHANNEL_UN_ACCP                    = 6,  // X
   EAT_CMS_OPR_DTR_BARRING                    = 8,  // X       X
   EAT_CMS_CALL_BARRED                        = 10, //         X
   EAT_CMS_RESERVED                           = 11, //         X
// Class(001): 16-31 -> normal event
   EAT_CMS_NORMAL_CALL_CLR                    = 16, // X
   EAT_CMS_USER_BUSY                          = 17, // X       X! (SMS: nwk failure)
   EAT_CMS_NO_USER_RESPONDING                 = 18, // X
   EAT_CMS_NO_ANSWER_ON_ALERT                 = 19, // X
   EAT_CMS_CALL_REJECTED                      = 21, // X       X
   EAT_CMS_NUMBER_CHANGED                     = 22, // X       X! (SMS: congestion)
   EAT_CMS_PRE_EMPTION                        = 25, // X
   EAT_CMS_NON_SEL_USER_CLEAR                 = 26, // X
   EAT_CMS_DEST_OUT_OF_ORDER                  = 27, // X       X
   EAT_CMS_INVALID_NUMBER_FORMAT              = 28, // X       X
   EAT_CMS_FACILITY_REJECT                    = 29, // X       X
   EAT_CMS_RES_STATUS_ENQ                     = 30, // X       X! (SMS:Unknown_Sub)
   EAT_CMS_NORMAL_UNSPECIFIED                 = 31, // X
// Class(010): 32-48 -> resource unavailable
   EAT_CMS_NO_CIRCUIT_CHANNEL_AVAIL           = 34, // X
   EAT_CMS_NETWORK_OUT_OF_ORDER               = 38, // X       X
   EAT_CMS_TEMPORARY_FAILURE                  = 41, // X       X
   EAT_CMS_SWITCH_EQUIPMENT_CONGESTION        = 42, // X       X
   EAT_CMS_ACCESS_INFO_DISCARDED              = 43, // X
   EAT_CMS_REQUESTED_CKT_CHANEL_NOT_AVIL      = 44, // X
   EAT_CMS_RESOURCE_UNAVAIL_UNSPECIFIED       = 47, // X       X
// Class(011): 49-64 -> service or option not available
   EAT_CMS_QOS_UNAVAIL                        = 49, // X
   EAT_CMS_REQ_FAC_NOT_SUBS                   = 50, // X       X
   EAT_CMS_IC_BAR_CUG                         = 55, // X
   EAT_CMS_BEARER_CAP_NOT_AUTHORISED          = 57, // X
   EAT_CMS_BEARER_CAP_NOT_AVAIL               = 58, // X
   EAT_CMS_SER_UNAVAILABLE                    = 63, // X
// Class(100): 65-80 -> service or option not implementated
   EAT_CMS_BEARER_SER_UNIMPL                  = 65, // X
   EAT_CMS_AEAT_CMS_EXCEEDED                       = 68, // X
   EAT_CMS_REQ_FACILITY_UNAVAIL               = 69, // X       X
   EAT_CMS_RESTR_DIGITAL_INFO                 = 70, // X
   EAT_CMS_SER_OPT_UNIMPL                     = 79, // X
// Class(101): 81-95 -> invalid message
   EAT_CMS_INVALID_TI_VALUE                   = 81, // X       X
   EAT_CMS_USER_NOT_IN_CUG                    = 87, // X
   EAT_CMS_INCOMPATIBLE_DEST                  = 88, // X
   EAT_CMS_INVALID_TRANSIT_NW_SEL             = 91, // X
   EAT_CMS_SEMANTIC_ERR                       = 95, // X       X
// Class(110): 96-112 -> protocol error
   EAT_CMS_INVALID_MANDATORY_INF              = 96, // X       X
   EAT_CMS_MSG_TYPE_UNIMPL                    = 97, // X       X
   EAT_CMS_MSG_TYPE_NOT_COMPATIBLE            = 98, // X       X
   EAT_CMS_IE_NON_EX                          = 99, // X       X
   EAT_CMS_COND_IE_ERR                       = 100, // X
   EAT_CMS_INCOMP_MESG_WITH_STATE            = 101, // X
   EAT_CMS_RECOVERY_ON_TIMER_EXPIRY          = 102, // X
   EAT_CMS_PROTOCOL_ERR_UNSPECIFIED          = 111, // X       X
// Class(111): 113-127 -> interworking
   EAT_CMS_INTER_WRK_UNSPECIFIED             = 127, // X       X   


    /*128-255 define by GSM0340 subclause 9.2.3.22 */
// 3GPP 23.040 clause 9.2.3.22 SMS TP cause values
// 128-255 -> SMS cause value
      
   /* PID error */   
   EAT_CMS_TELEMATIC_INT_WRK_NOT_SUPPORT = 0x80, /* telematic interworking not support */
   EAT_CMS_SMS_TYPE0_NOT_SUPPORT         = 0x81, /* short message type 0 not support */
   EAT_CMS_CANNOT_REPLACE_MSG            = 0x82, 
   EAT_CMS_UNSPECIFIED_PID_ERROR         = 0x8F, 
   
   /* DCS error */
   EAT_CMS_ALPHABET_NOT_SUPPORT          = 0x90, /* data coding scheme (alphabet) not support */
   EAT_CMS_MSG_CLASS_NOT_SUPPORT         = 0x91, /* message class not support */
   EAT_CMS_UNSPECIFIED_TP_DCS            = 0x9f,

   /* Command error */
   EAT_CMS_CMD_CANNOT_ACTION             = 0xa0, /* command cannot be actioned */
   EAT_CMS_CMD_NOT_SUPPORT               = 0xa1, /* command unsupported */
   EAT_CMS_UNSPECIFIED_CMD_ERROR         = 0xaf, 
   
   EAT_CMS_TPDU_NOT_SUPPORT              = 0xb0,   
   EAT_CMS_SC_BUSY                       = 0xc0,
   EAT_CMS_NO_SC_SUBSCRIPTION            = 0xc1,
   EAT_CMS_SC_SYS_FAILURE                = 0xc2,  /* SC system failure */
   EAT_CMS_INVALID_SME_ADDR              = 0xc3,  
   EAT_CMS_DEST_SME_BARRED               = 0xc4,  /* destination SME barred */
   EAT_CMS_SM_REJ_DUP_SM                 = 0xc5,  /* SM rejected-duplicate SM */
   EAT_CMS_TP_VPF_NOT_SUPPORT            = 0xc6,
   EAT_CMS_TP_VP_NOT_SUPPORT             = 0xc7,
      
   EAT_CMS_SIM_STORAGE_FULL              = 0xd0,
   EAT_CMS_NO_SMS_STO_IN_SIM             = 0xd1,  /* No SMS storage capability in SIM */
   EAT_CMS_ERROR_IN_MS                   = 0xd2, 
   EAT_CMS_MEM_CAP_EXCEEDED              = 0xd3,  /* Memory Capacity Exceeded */
   
   /* SIM Application Toolkit */
   EAT_CMS_SMS_SAT_BUSY                  = 0xd4,  /* SIM Application Toolkit Busy */
   EAT_CMS_SMS_SAT_DL_ERROR              = 0xd5,  /* SIM data download error */
   
   /* 0XE0 to 0xFE are specific to application */
   EAT_CMS_SMS_CP_RETRY_EXCEED           = 0xe0,
   EAT_CMS_SMS_RP_TR1M_TIMEOUT           = 0xe1,
   EAT_CMS_SMS_CONNECTION_BROKEN         = 0xe2,
   
   EAT_CMS_UNSPECIFIED_TP_FCS            = 0xff,



    /*300-500 defined by GSM0705*/
    EAT_CMS_ME_FAILURE = 300,	/* ME failure  */
    EAT_CMS_SMS_ME_RESERVED = 301,  /* SMS ME reserved  */
    EAT_CMS_OPERATION_NOT_ALLOWED = 302,  /* Operation not allowed  */
    EAT_CMS_OPERATION_NOT_SUPPORTED = 303,  /* Operation not supported  */
    EAT_CMS_INVALID_PDU_MODE_PARA  = 304,   /* invalid pdu mode parameter */
    EAT_CMS_INVALID_TEXT_MODE_PARA = 305,   /* invalid text mode parameter */

    EAT_CMS_SIM_NOT_INSERTED = 310,  /* SIM not inserted  */
    EAT_CMS_SIM_PIN_NECESSARY = 311,  /* SIM pin necessary  */
    EAT_CMS_PH_SIM_PIN_NECESSARY = 312,  /* PH SIM pin necessary  */
    EAT_CMS_SMS_SIM_FAILURE        = 313,   /* sim failure */
    EAT_CMS_SIM_BUSY = 314,  /* SIM busy  */
    EAT_CMS_SIM_WRONG = 315,  /* SIM wrong  */
    EAT_CMS_SIM_PUK_REQUIRED = 316,  /* SIM PUK required  */
    EAT_CMS_SIM_PIN2_REQUIRED = 317,  /* SIM PIN2 required  */
    EAT_CMS_SIM_PUK2_REQUIRED = 318,  /* SIM PUK2 required  */

    EAT_CMS_MEM_FAILURE	          = 320,   /* memory failure */
    EAT_CMS_INVALID_MEM_INDEX	  = 321,   /* invalid memory index */
    EAT_CMS_MEM_FULL	          = 322,   /* memory full */
    EAT_CMS_INVALID_PARAMETER = 323,  /* Invalid parameter  */
    EAT_CMS_INVALID_INPUT_FORMAT = 324,  /* Invalid input format  */
    EAT_CMS_INVALID_INPUT_VALUE =325, /*invalid input value*/

    EAT_CMS_SCA_UNKNOWN	          = 330,   /* SMSC address unknown */
    EAT_CMS_NO_NETWORK = 331,  /* No network  */
    EAT_CMS_NETWORK_TIMEOUT = 332,  /* Network timeout  */

    EAT_CMS_NO_CNMA_EXPECTED       = 340,   /* no +CNMA acknowledgement expected */
    EAT_CMS_ERROR_UNKNOWN = 500,

    /*SIMCOM: Other values in the range 300 - 511 are reserved for use by CMS error codes,same as sim300 and sim900. end*/


    /*SIMCOM: Other values in the range 512 - 576 are nominally reserved for proprietary SMS error codes,same as sim300 and sim900. begin*/
    EAT_CMS_SIM_NOT_READY = 512,  /* SIM not ready  */
    EAT_CMS_UNREAD_RECORDS_ON_SIM = 513,  /* Unread records on SIM  */
    EAT_CMS_CB_ERROR_UNKNOWN = 514,  /* CB error unknown  */
    EAT_CMS_PS_BUSY = 515,  /* PS busy  */

    EAT_CMS_ERROR_SMREADY_COULDNT_READ_SMSP = 516,

    EAT_CMS_SM_NOT_READY = 517,  /* SM not ready  */
    EAT_CMS_INVALID_NON_HEX_CHARS_INPDU = 528,  /* Invalid (non-hex) chars inPDU  */
    EAT_CMS_INCORRECT_PDU_LENGTH = 529,  /* Incorrect PDU length  */
    EAT_CMS_INVALID_MTI = 530,  /* Invalid MTI  */
    EAT_CMS_INVALID_NON_HEX_CHARS_IN_ADDRESS = 531,	/* Invalid (non-hex) chars in address  */
    EAT_CMS_INVALID_ADDRESS_NO_DIGITS_READ = 532,  /* Invalid address (no digits read)  */
    EAT_CMS_INCORRECT_PDU_LENGTH_UDL = 533,	/* Incorrect PDU length (UDL)  */
    EAT_CMS_INCORRECT_SCA_LENGTH = 534,  /* Incorrect SCA length  */
    EAT_CMS_INVALID_FIRST_OCTET_SHOULD_BE_2_OR_34 = 536,  /* Invalid First Octet (should be 2 or 34)  */
    EAT_CMS_INVALID_COMMAND_TYPE = 537,  /* Invalid Command type  */
    EAT_CMS_SRR_BIT_NOT_SET = 538,  /* SRR bit not set  */
    EAT_CMS_SRR_BIT_SET = 539,  /* SRR bit set  */
    EAT_CMS_INVALID_USER_DATA_HEADER_IE = 540,  /* Invalid User Data Header IE  */

    /*512... defined by manufacturer*/
    EAT_CMS_SMSAL_NO_ERROR             = 512,
    EAT_CMS_MSG_LEN_EXCEEDED           = 513,   /* message length exceeds maximum length */
    EAT_CMS_INVALID_REQ_PARAMETER      = 514,   /* invalid request parameters */
    EAT_CMS_SMS_ME_STORAGE_FAILURE     = 515,   /* ME storage failure */
    EAT_CMS_SMSAL_INVALID_BEARER       = 516,   /* Invalid bearer service */
    EAT_CMS_SMSAL_INVALID_SERVICE_MODE = 517,   /* Invalid service mode */
    EAT_CMS_SMSAL_INVALID_STORAGE_TYPE = 518,   /* Invalid storage type */
    EAT_CMS_SMSAL_INVALID_MSG_FORMAT   = 519,   /* Invalid message format */
    EAT_CMS_TOO_MANY_MO_CONCAT_MSG     = 520,   /* Too many MO concatenated messages */
    EAT_CMS_SMSAL_NOT_READY            = 521,
    EAT_CMS_SMSAL_NO_MO_SERVICE        = 522,
    EAT_CMS_NOT_SUPP_SR_CMD_IN_STORAGE = 523,   /* not support TP-Status-Report & TP-Command in storage */   
    EAT_CMS_SMSAL_NOT_SUPPORT_MSG_TYPE = 524,   /* Reserved MTI */
    EAT_CMS_SMS_RL_NO_FREE_ENTITY      = 525,   /* no free entity in RL layer */
    EAT_CMS_SMS_PORT_NUM_ALREADY_REG   = 526,   /* The port number is already registerred */
    EAT_CMS_SMS_PORT_NUM_NO_FREE_ENTITY= 527,   /* There is no free entity for port number */
    EAT_CMS_SMS_RL_MMS_STATE_ERR       = 528,   /* More Message to Send state error */
    EAT_CMS_SMS_MO_SMS_NOT_ALLOW       = 529,   /* MO SMS is not allow */
    EAT_CMS_SMS_PS_SUSPENDED           = 530,   /* GPRS is suspended */
    EAT_CMS_SMS_ME_STORAGE_FULL        = 531,   /* ME storage full */
    EAT_CMS_SMS_SIM_REFRESHING         = 532,   /* doing SIM refresh */    
}eat_cms_err_id_enum;

/*SIMCOM chenhaibing 2012-12-05 MKBug00014441 END*/
/*附件一：M2M模组规范（评审稿）.pdf 附录C  WMMP 的错误代码 */
#ifdef  __SIMCOM_WMMP__
typedef enum
{    
	EAT_WMMP_PARAMETER_WRONG   = 240,
	EAT_WMMP_DNS_PARSE_WRONG	 = 241,
	EAT_WMMP_PDP_NONACTIVATED	 = 242,
	EAT_WMMP_UNLINK_SOCKET	 = 243,
	EAT_WMMP_NET_ERR	 = 244,
	EAT_WMMP_POWER_VOLTAGE_ERR  = 245,
	EAT_WMMP_DATE_TIME_ERR	 = 246,

	EAT_WMMP_UNLINK_SVR   =908,
	EAT_WMMP_UNOPEN_SVR	 = 909,
	EAT_WMMP_SYS_BUSY	 = 910,
	EAT_WMMP_UNREG	 = 911,
	EAT_WMMP_UNLOGIN	 = 912,
	EAT_WMMP_LOGIN  = 913,
	EAT_WMMP_LAST_TASK_OT= 914,
	
	EAT_WMMP_UNINIT_WMMP	 =950,
	EAT_WMMP_REPEAT	 = 951,
	EAT_WMMP_FORBID_EXECUTE = 952,
	EAT_WMMP_WAIT_CHANGE_SIM= 953,
	EAT_WMMP_PIN1_REQUIRE  = 954,
	EAT_WMMP_PUK1_REQUIRE  = 955,
	EAT_WMMP_CRC_ERR= 956,

	EAT_WMMP_PASSWORD_ERR	 =957,
	EAT_WMMP_DATA_OVERLENGTH	 = 958,
	EAT_WMMP_SIM_BIND_FAIL = 959,
	EAT_WMMP_SIM_BINDED= 960,
	
	/*980-983 TT-transparent transmission*/
	EAT_WMMP_TT_DESTINATION_OFFLINE  = 980,
	EAT_WMMP_TT_INVALID_DESTINATION  = 981,
	EAT_WMMP_TT_REJECT= 982,
	EAT_WMMP_TT_CRC_ERR= 983,
	
	/*985-987 FILE REQUEST*/
	EAT_WMMP_INVALID_ID	= 985,
	EAT_WMMP_INVALID_READ_POINT  = 986,
	EAT_WMMP_INVALID_FILE_SIZE= 987,
	
	/*990-993 security configuration*/
	EAT_WMMP_SEC_DECRYPTION_FAILERE=990,
	EAT_WMMP_SEC_INVALID_TLV=991,
	EAT_WMMP_SEC_REJECT=992,
	EAT_WMMP_SEC_ABSTRACT_ERR=993,
	
	/*995-999 command transparent transmission*/
	EAT_WMMP_CTT_DESTINATION_OFFLINE  = 995,
	EAT_WMMP_CTT_INVALID_DESTINATION  = 996,
	EAT_WMMP_CTT_REJECT= 997,
	EAT_WMMP_CTT_INVALID_DATA= 998,
	EAT_WMMP_CTT_INVALID_INPUT= 999	
	
}EAT_WMMP_ERR_ID_ENUM;
#endif/*#ifdef __SIMCOM_WMMP__*/

typedef enum _EAT_AT_PARSER_RET
{
	EAT_APR_OK = 1 << 0,
	EAT_APR_NOT_DQ_FORMAT = 1 << 1,  //非双引号格式的字符
	EAT_APR_NULL_PARAM_ERR = 1 << 2,  //解析到一个空的参数错误。例:p1, ,p2，p1和p2之间的参数
	EAT_APR_FORMAT_ERR = 1 << 3,  //参数格式错误
	EAT_APR_TYPE_ERR = 1 << 4,  //参数类型错误
	EAT_APR_NO_PARAM_ERR = 1 << 5,  //未解析到参数错误
	EAT_APR_VALUE_RANGE_ERR    = 1 << 6//输入的参数取值范围不满足命令要求
}EAT_AT_PARSER_RET;

typedef enum _EAT_AT_EXE_RESULT
{
	EAT_AER_OK,
	EAT_AER_CONNECT,
	EAT_AER_RING,
	EAT_AER_NO_CARRIER,
	EAT_AER_ERROR,
	EAT_AER_BUSY,
	EAT_AER_CONNECT_PS
}EAT_AT_EXE_RESULT;


typedef void (*EAT_CUSTOMER_AT_HDLR)(void*);

typedef struct _EAT_AT_REG_INFO
{
	const char* cpszAt;
	const EAT_CUSTOMER_AT_HDLR pfAtTestHdlr;
	const EAT_CUSTOMER_AT_HDLR pfAtReadHdlr;
	const EAT_CUSTOMER_AT_HDLR pfAtSetHdlr;
	const EAT_CUSTOMER_AT_HDLR pfAtExeHdlr;
}EAT_AT_REG_INFO;
extern void (* const eat_ds_enter_data_mode)(void);
extern void (* const eat_ds_escape_data_mode)(void);

//设置AT命令执行的结果，和错误码
extern void (* const eat_at_output_set_result)(EAT_AT_EXE_RESULT enmAER, u32 u16ErrCode);

//向串口输出字符串
extern u32 (* const eat_at_output)(const char* cpszFormat,...);
extern void (* const eat_at_output_async)(u16 channel_id, u8 *buffer, u16 length, eat_bool stuff);

//向串口输出CR和LF(回车与换行)
extern void (* const eat_at_output_cr_lf)(void);

//向串口输出默认的结束字符串
extern void (* const eat_at_output_default_end)(void);

//获取回车字符串:"x"，x代表回车字符
extern const char* (* const eat_at_output_get_cr)(void);

//获取换行字符串:"x"，x代表换行字符
extern const char* (* const eat_at_output_get_lf)(void);

//增加向串口输出的缓存区的长度
extern void (* const eat_at_output_increase_buffer)(u32 uLen);


//如果at命令附加的 s32输入数值 在给定的闭区间内(如[nMin,nMax]为: nMin <= n && n <= nMax)，获取该值到pnValue
extern EAT_AT_PARSER_RET (* const eat_at_parser_get_s32)(s32 nMin, s32 nMax, s32* pnValue);

//如果at命令附加的 u32输入数值 在给定的闭区间内(如[uMin,uMax]为: uMin <= n && n <= uMax)，获取该值到puValue
extern EAT_AT_PARSER_RET (* const eat_at_parser_get_u32)(u32 uMin, u32 uMax, u32* puValue);

//如果at命令附加的 下一个参数 长度在给定闭区间[u8LenMin, u8LenMax]内，获取该值到pszString
//不带双引号的参数也可用此函数读取
extern EAT_AT_PARSER_RET (* const eat_at_parser_get_str)(u32 uLenMin, u32 uLenMax, char* pszString);

//如果at命令附加的 下一个参数 长度在给定闭区间[u8LenMin, u8LenMax]内，获取该值到pszString
//dq = double_quotes，该函数要求参数必须在双引号之间
extern EAT_AT_PARSER_RET (* const eat_at_parser_get_str_between_dq)(u32 uLenMin, u32 uLenMax, char* pszString);

//当前是否已经解析到最后一个参数
extern eat_bool (* const eat_at_parser_is_end)(void);

//获取该条AT命令的source id
extern u8 (* const eat_at_parser_get_src_id)(void);

//注册自定义的AT命令
extern u32 (* const eat_customer_at_reg)(EAT_AT_REG_INFO* pAtSet, u32 uAtCount);
#endif //#ifndef _EAT_AT_H_
