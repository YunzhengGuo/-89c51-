
#include "display.h"

extern void LCD_ShowWNL();
extern void GUI_Init();
extern void SFR_Init();
extern void Time_Set();
extern SYSTIME sys;		   //ϵͳ����
extern  Mydate SpDat;		   //ũ������
void  main(){	//������
	 SFR_Init();
	
	 GUI_Init();
	 TR1=1;
	 while(1){
		GetTime(&sys);		//���ʱ��
		LCD_ShowWNL();		//��ʾ������
		Time_Set();         //ʱ������
	 }	
}