#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int
#define SECOND 0x80		//��
#define MINUTE 0x82		//��
#define HOUR   0x84		//�r
#define DAY	   0x86		//��
#define MONTH  0x88		//��
#define WEEK   0x8a		//����
#define YEAR   0x8c		//��

sbit DS1302_RST=P1^5; 	
sbit DS1302_SCLK=P1^6; 	
sbit DS1302_IO=P1^7; 	
	
typedef struct systime{
	uchar cYear;
	uchar	cMon;
	uchar	cDay;
	uchar	cHour;
	uchar	cMin;
	uchar	cSec;
	uchar	cWeek;
}SYSTIME;
		 
void DS1302_Write(uchar D){		//DS1302д�뺯��
	uchar i;	
	for(i=0;i<8;i++){	
		DS1302_IO=D&0x01;
		DS1302_SCLK=1;
		DS1302_SCLK=0;
		D=D>>1;				
	}	
}

uchar DS1302_Read(){		   //DS1302��������
	uchar TempDat=0,i;
	for(i=0;i<8;i++){	
		TempDat>>=1; 	
		if(DS1302_IO) TempDat=TempDat|0x80;
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}	
    return TempDat;
}

void WDS1302(uchar ucAddr, uchar ucDat){  //DS1302���ֽ�д�뺯��
    DS1302_RST = 0;
    DS1302_SCLK = 0;
    DS1302_RST = 1;
    DS1302_Write(ucAddr);       // ��ַ������ 
    DS1302_Write(ucDat);       	// д1Byte����
    DS1302_SCLK = 1;
    DS1302_RST = 0;
} 

uchar RDS1302(uchar ucAddr){			  //DS1302���ֽڶ�������
    uchar ucDat;
    DS1302_RST = 0;
    DS1302_SCLK = 0;
    DS1302_RST = 1;
    DS1302_Write(ucAddr);       	// ��ַ������ 
    ucDat=DS1302_Read();       
    DS1302_SCLK = 1;
    DS1302_RST = 0;
	return ucDat;
}

void SetTime(SYSTIME sys){		   //ʱ�����ú���
//    WDS1302(YEAR,sys.cYear&0x7f); 
//	WDS1302(MONTH,sys.cMon&0x0f);//&0x1f);
//	WDS1302(DAY,sys.cDay&0x1f);//&0x3f);
//	WDS1302(HOUR,sys.cHour&0x1f);//&0xbf);
//	WDS1302(MINUTE,sys.cMin&0x3f);//&0x7f);
//	WDS1302(SECOND,sys.cSec&0x3f);//&0x7f);	
//	WDS1302(WEEK,sys.cWeek&0x07);//&0x07);
	
	  WDS1302(YEAR,sys.cYear); 
	WDS1302(MONTH,sys.cMon&0x1f);
	WDS1302(DAY,sys.cDay&0x3f);
	WDS1302(HOUR,sys.cHour&0x3f);//ԭΪ&0xbf ��2017 11 20 17:26 �޸Ĺ�����
	WDS1302(MINUTE,sys.cMin&0x7f);
	WDS1302(SECOND,sys.cSec&0x7f);	
	WDS1302(WEEK,sys.cWeek&0x07);
}

void GetTime(SYSTIME *sys){		  //ʱ���ȡ����
	uchar uiTempDat;
	
	uiTempDat=RDS1302(YEAR|0x01);			  
	(*sys).cYear=(uiTempDat>>4)*10+(uiTempDat&0x0f);

	uiTempDat=RDS1302(0x88|0x01);
	(*sys).cMon=((uiTempDat&0x1f)>>4)*10+(uiTempDat&0x0f);	
	
	uiTempDat=RDS1302(DAY|0x01);
	(*sys).cDay=((uiTempDat&0x3f)>>4)*10+(uiTempDat&0x0f);
	uiTempDat=RDS1302(HOUR|0x01);
	(*sys).cHour=((uiTempDat&0x3f)>>4)*10+(uiTempDat&0x0f);
	uiTempDat=RDS1302(MINUTE|0x01);
	sys->cMin=((uiTempDat&0x7f)>>4)*10+(uiTempDat&0x0f);
	uiTempDat=RDS1302(SECOND|0x01);
	sys->cSec=((uiTempDat&0x7f)>>4)*10+(uiTempDat&0x0f);
	uiTempDat=RDS1302(MONTH|0x01);
	(*sys).cMon=uiTempDat&0x17;
	uiTempDat=RDS1302(WEEK|0x01);
	sys->cWeek=uiTempDat&0x07;
}