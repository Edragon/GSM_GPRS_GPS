//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//diy Audio
//����֧��:aemails@163.com
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __GPS_H
#define __GPS_H	 

#include "stm8s.h"

//GPS NMEA-0183Э����Ҫ�����ṹ�嶨�� 
//������Ϣ
typedef struct  
{										    
  u8 num;	//���Ǳ��
  u8 eledeg;	//��������
  u16 azideg;	//���Ƿ�λ��
  u8 sn;	//�����		   
}nmea_slmsg; 
//#pragma pack(push) //�������״̬  
//#pragma   pack(1)
//typedef struct
//{
//  u8 num;	
//}aasd;
//#pragma   pack() 
//#pragma pack(pop)//�ָ�����״̬  

//UTCʱ����Ϣ
typedef struct  
{										    
  u16 year;	//���
  u8 month;	//�·�
  u8 date;	//����
  u8 hour; 	//Сʱ
  u8 min; 	//����
  u8 sec; 	//����
}nmea_utc_time;   	   
//NMEA 0183 Э����������ݴ�Žṹ��
typedef struct  
{										    
  u8 svnum;					//�ɼ�GPS������
  nmea_slmsg slmsg[12];		//���12��GPS����
  nmea_utc_time utc;			//UTCʱ��
  u32 latitude;				//γ�� ������100000��,ʵ��Ҫ����100000
  u8 nshemi;					//��γ/��γ,N:��γ;S:��γ				  
  u32 longitude;			    //���� ������100000��,ʵ��Ҫ����100000
  u8 ewhemi;					//����/����,E:����;W:����
  u8 gpssta;					//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
  u8 posslnum;				//���ڶ�λ��GPS������,0~12.
  u8 possl[12];				//���ڶ�λ�����Ǳ��
  u8 fixmode;					//��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ
  u16 pdop;					//λ�þ������� 0~500,��Ӧʵ��ֵ0~50.0
  u16 hdop;					//ˮƽ�������� 0~500,��Ӧʵ��ֵ0~50.0
  u16 vdop;					//��ֱ�������� 0~500,��Ӧʵ��ֵ0~50.0 
  int altitude;			 	//���θ߶�,�Ŵ���10��,ʵ�ʳ���10.��λ:0.1m	 
  u16 speed;					//��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ	 
}nmea_msg;


long NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf);
void UTC_TimeZone (nmea_msg *gpsx,u8 tz);

#endif

 



