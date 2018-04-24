
#include "display.h"
#include "12864.h"
#include "model.h"
#include "ds1302.h"
#include "lunar.h"
#include "keyinput.h"

#define uchar unsigned char
#define uint unsigned int
#define NoUpLine 	1
#define UpLine   	0
#define NoUnderLine 1
#define UnderLine	0
#define FALSE	0
#define TRUE    1

uchar dispBuf[7];
uchar T0_Count=0,Tmp_Count=0;
bit T0_Flag,Tmp_Flag,Flash_Flag;
void DecToBCD();
void BCDToDEC();
SYSTIME sys;		   //ϵͳ����
Mydate SpDat;		   //ũ������

bit Hour_Flag=TRUE,Min_Flag=TRUE,Sec_Flag=TRUE;	   //����ʱ���־
bit Year_Flag=TRUE,Mon_Flag=TRUE,Day_Flag=TRUE;

uchar State_Set=0;     //����ʱ���֡��롢�ա��¡����״̬

bit State_Flag=FALSE,Inc_Flag=FALSE,Dec_Flag=FALSE;	//���������Ƿ��µı�־

uchar code Mon2[2][13]={0,31,28,31,30,31,30,31,31,30,31,30,31,
				  0,31,29,31,30,31,30,31,31,30,31,30,31};

/*****************************LCD��ʾ����******************************
��ڲ�����
		cDat:		Ҫ��ʾ����
		X:			���� 0~7
		Y:          ���� 0~127
		show_flag:  �Ƿ񷴰���ʾ��0���ף�1������ 
		upline: 	�ϻ���, 0��ʾ���ϻ���
		underline:  �»���, 0��ʾ���»���
***********************************************************************/
void LCD_ShowTime(char cDat,uchar X,uchar Y,bit show_flag,bit up,bit under){
	uchar s[2];
   	s[0]=cDat/10+'0';
	s[1]=cDat%10+'0';
	en_disp(X,Y,2,Asc,s,show_flag,up,under);
}
																  
void Show_YMD(){	  //�ꡢ�¡��ա����ڡ���ʾ����
	uchar uiTempDat;
   	uiTempDat=RDS1302(0x88|0x01);
	sys.cMon=((uiTempDat&0x1f)>>4)*10+(uiTempDat&0x0f);	
	LCD_ShowTime(sys.cMon,2,5,Mon_Flag,NoUpLine,NoUnderLine);	
	hz_disp(4,5,1,uMod[1],1,NoUpLine,NoUnderLine);				//��
	Show16X32(2,27,ucNum3216[sys.cDay/10],Day_Flag);			//��
	Show16X32(2,43,ucNum3216[sys.cDay%10],Day_Flag);  			
	hz_disp(6,8,2,ucLunar[13],1,UpLine,UnderLine);					    
	if(sys.cWeek==7)
	hz_disp(6,40,1,uMod[2],1,UpLine,UnderLine);					//���� 
	else
    hz_disp(6,40,1,ucLunar[sys.cWeek],1,UpLine,UnderLine);		//����
    LCD_ShowTime(20,0,9,1,UpLine,UnderLine);
  	LCD_ShowTime(sys.cYear,0,25,Year_Flag,UpLine,UnderLine);
    hz_disp(0,41,1,uMod[0],1,UpLine,UnderLine);					//��

	//SpDat=GetSpringDay(sys.cYear,sys.cMon,sys.cDay);			//���ũ��
		SpDat=toLunar(sys.cYear+2000,sys.cMon,sys.cDay);
//		SpDat.cMon = 9;
//	SpDat.cDay= 26;
//	SpDat.cYear = 2017;
    if(SpDat.cMon==1)										   	//��ʾũ����
	  hz_disp(4,64,1,ucLunar[15],1,UpLine,NoUnderLine);			//"��"
	else if(SpDat.cMon==11)
	  hz_disp(4,64,1,ucLunar[16],1,UpLine,NoUnderLine);			//"��"
	else if(SpDat.cMon==12)
	  hz_disp(4,64,1,ucLunar[17],1,UpLine,NoUnderLine);			//"��"
	else
	  hz_disp(4,63,1,ucLunar[SpDat.cMon],1,UpLine,NoUnderLine); //"��"~"ʮ"
	 if(SpDat.cDay/10==1 && SpDat.cDay%10>0)					//��ʾ"ʮ" ����"ʮ��"������"һ��"
	 hz_disp(4,95,1,ucLunar[10],1,UpLine,NoUnderLine);
	 else if(SpDat.cDay/10==2 && SpDat.cDay%10>0)               //��ʾ"إ" ����"إ��"������"����"
	 hz_disp(4,95,1,ucLunar[19],1,UpLine,NoUnderLine);
	 else
	 hz_disp(4,95,1,ucLunar[SpDat.cDay/10],1,UpLine,NoUnderLine);  //��������
	if(!(SpDat.cDay%10))	    							    //"ʮ"
	 hz_disp(4,111,1,ucLunar[10],1,UpLine,NoUnderLine);
	else														//��������
	 hz_disp(4,111,1,ucLunar[SpDat.cDay%10],1,UpLine,NoUnderLine);

	 hz_disp(0,104,1,SX[(uint)(SpDat.cYear)%12],1,UpLine,UnderLine);   //��Ф

	 hz_disp(2,95,1,TianGan[(uint)(SpDat.cYear)%10],1,NoUpLine,NoUnderLine); //���
	 hz_disp(2,111,1,DiZhi[(uint)(SpDat.cYear)%12],1,NoUpLine,NoUnderLine);  //��֧
}

void LCD_ShowWNL(){	  //��������ʾ����
	LCD_ShowTime(sys.cSec,6,111,Sec_Flag,UpLine,UnderLine);		 //�룬ÿ����ˢ��
	if(!sys.cSec || State_Set)								     //�֣���ͨģʽÿ����ˢ��
	LCD_ShowTime(sys.cMin,6,87,Min_Flag,UpLine,UnderLine);		 //    ����ģʽÿ��ˢ��
	 
	if(!sys.cSec && !sys.cMin || State_Set)						 //ʱ����ͨģʽÿСʱˢ��
    LCD_ShowTime(sys.cHour,6,63,Hour_Flag,UpLine,UnderLine);     //    ����ģʽÿ��ˢ��
	
	if(!sys.cSec && !sys.cMin && !sys.cHour || State_Set ){      //����ũ�����ꡢ�¡��ա�����
		Show_YMD();												 //��ͨģʽÿ��ˢ��
		if(State_Set==7) State_Set=0;							 //����ģʽÿ��ˢ��
	}	
}

//void CAL_Init(){	//���ڳ�ʼ������
////	sys.cYear=0x17;	//BCD���ʾ������ʱ��ֵ
////	sys.cMon=0x11;
////	sys.cDay=0x14;
////	sys.cHour=0x20;
////	sys.cMin=0x12;
////	sys.cSec=0x11;
////	sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);
//	
//	GetTime(&sys);				   //���ʱ��
//	 SetTime(sys);				   //����ʱ��
//	sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);
//}

void SFR_Init(){		 //��ʱ��1��ʼ������
	Flash_Flag=FALSE;
	TMOD=0x11;
	ET1=1;
	TH1= (-10000)/256;
	TL1= (-10000)%256;
	EA=1;
}

void GUI_Init(){	 //LCDͼ�γ�ʼ������
	 LCD12864_init();
	 ClearLCD();
	 Rect(0,0,127,63,1);   //�����
	 Line(62,0,62,62,1);
	 Line(0,48,127,48,1);
	 Line(0,15,127,15,1);
	 Line(24,15,24,48,1);
	 Line(63,32,128,32,1);
	
		GetTime(&sys);				   //���ʱ��
		DecToBCD();
	 //SetTime(sys);				   //����ʱ��
		sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);
		SetTime(sys);				   //����ʱ��
	//GetTime(&sys);				   //���ʱ��
		BCDToDEC();
		Show_YMD();
	 LCD_ShowTime(sys.cSec,6,111,Sec_Flag,UpLine,UnderLine);
	 en_disp(6,103,1,Asc,":",1,UpLine,UnderLine);
	 LCD_ShowTime(sys.cMin,6,87,Min_Flag,UpLine,UnderLine); 
	 en_disp(6,79,1,Asc,":",1,UpLine,UnderLine);	 
     LCD_ShowTime(sys.cHour,6,63,Hour_Flag,UpLine,UnderLine);

	 hz_disp(2,64,1,ucLunar[11],1,NoUpLine,NoUnderLine);	   //"ũ"
	 hz_disp(2,80,1,ucLunar[12],1,NoUpLine,NoUnderLine);	   //"��"
	 hz_disp(4,79,1,uMod[1],1,UpLine,NoUnderLine);			   //"��"
}

void DecToBCD(){   //ʮ����ת��������
	sys.cHour=(((sys.cHour)/10)<<4)+((sys.cHour)%10);
	sys.cMin=(((sys.cMin)/10)<<4)+((sys.cMin)%10);
	sys.cSec=((sys.cSec/10)<<4)+((sys.cSec)%10);
  	sys.cYear=((sys.cYear/10)<<4)+((sys.cYear)%10);
	//sys.cMon=((sys.cMon/10)<<4)+((sys.cMon)%10);//gyz 20171120ע��
	sys.cDay=((sys.cDay/10)<<4)+((sys.cDay)%10);
}
void BCDToDEC(){ //������ת��ʮ���ƺ���
	sys.cYear=(((sys.cYear)>>4)&0X0f)*10+(sys.cYear&0x0f);
	sys.cDay=(((sys.cDay)>>4)&0X0f)*10+(sys.cDay&0x0f);
	sys.cHour=(((sys.cHour)>>4)&0X0f)*10+(sys.cHour&0x0f);
	sys.cMin=(((sys.cMin)>>4)&0X0f)*10+(sys.cMin&0x0f);
	sys.cSec=(((sys.cSec)>>4)&0X0f)*10+(sys.cSec&0x0f);

}
void Time_Set(){   //ʱ�����ú���
	if(State_Flag){					//���ü�����
		State_Flag=FALSE;
		State_Set++;
		if(State_Set==8) State_Set=0;
	}

	Hour_Flag=TRUE;Min_Flag=TRUE;Sec_Flag=TRUE;
    Year_Flag=TRUE;Mon_Flag=TRUE;Day_Flag=TRUE;

	switch(State_Set){              //��������
			case 0:                 //������
				break;
			case 1:					//����ʱ
				Hour_Flag=FALSE;
				break;
			case 2:	   				//���÷�
				Min_Flag=FALSE;
				break;
			case 3:				 	//������
				Sec_Flag=FALSE;
				break;
			case 4:					//������
			 	Day_Flag=FALSE;
				break;
			case 5:
			 	Mon_Flag=FALSE;     //������
				break;
			case 6:
			 	Year_Flag=FALSE;	//������
		    	break;
			case 7:					//�޶��������ô�ֵΪ��"��"�ķ�����ʧ��
				break;
	  }	   
	  
	if(Inc_Flag){					//�Ӽ�������
		Inc_Flag=FALSE;
		switch(State_Set)		{
			case 0:
				break;
			case 1:	   				 //Сʱ��
				sys.cHour++;
				(sys.cHour)%=24;
				break;
			case 2:		   			 //�ּ�1
				sys.cMin++;
				sys.cMin%=60;
				break;
			case 3:					 //���1
			    sys.cSec++;
				sys.cSec%=60;
				break;
			case 4:					 //���1
				(sys.cDay)=(sys.cDay%Mon2[YearFlag(sys.cYear)][sys.cMon])+1;		   
				break;
			case 5:					 //�¼�1
				sys.cMon=(sys.cMon%12)+1;
				break;
			case 6:
				sys.cYear++;		 //���1
				sys.cYear=sys.cYear%100;
			break;
		  }	
		DecToBCD();					//תΪBCD��
		sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);    //�������
		SetTime(sys);	    		//����DS1302
	}

	if(Dec_Flag){				 	//��������
		Dec_Flag=FALSE;
		switch(State_Set){
			case 0:
				break;
			case 1:	   
				sys.cHour=(sys.cHour+23)%24;  //ʱ��1
				break;
			case 2:		  					  //�ּ�1
				sys.cMin=(sys.cMin+59)%60;
				break;
			case 3:							  //���1
				sys.cSec=(sys.cSec+59)%60;
				break;
			case 4:							  //���1
				sys.cDay=((sys.cDay+Mon2[YearFlag(sys.cYear)][sys.cMon]-1)%Mon2[YearFlag(sys.cYear)][sys.cMon]);		   
				if(sys.cDay==0) sys.cDay=Mon2[YearFlag(sys.cYear)][sys.cMon];
				break;
			case 5:							  //�¼�1
				sys.cMon=(sys.cMon+11)%12;
				if(sys.cMon==0) sys.cMon=12;
				break;
			case 6:							  //���1
				sys.cYear=(sys.cYear+99)%100;
				break;
		  }	
		DecToBCD();
		sys.cWeek=GetWeekDay(sys.cYear,sys.cMon,sys.cDay);     	 
		SetTime(sys);	
	}
}



void timer1() interrupt  3 {  //��ʱ��1�жϷ�����
	TH1= (-10000)/256;
	TL1= (-10000)%256;
	keyinput();					//��ȡ����
	if (keyvalue&0x10){	
		State_Flag=TRUE;
		keyvalue &= 0xef;		//���ֵ����֤һֱ����ִֻ��һ�ΰ���������
	}	
	if (keyvalue&0x20 ){		//��
		Inc_Flag=TRUE;
		keyvalue &= 0xdf;		//���ֵ����֤һֱ����ִֻ��һ�ΰ���������
	}	
	if (keyvalue&0x40){			//��
		Dec_Flag=TRUE;
		keyvalue &= 0xbf;		//���ֵ����֤һֱ����ִֻ��һ�ΰ���������
	}
	
}