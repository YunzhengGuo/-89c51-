#ifndef _COLINLUAN_LUNAR_H_
#define _COLINLUAN_LUNAR_H_
#define uchar unsigned char
#define uint unsigned int
 
typedef struct {
	int cYear;
	uchar cMon;
	uchar cDay;
	int reserved;
} Mydate;
//typedef struct spdate
//{
//	char 	cYear;
//	char	cMon;
//	char	cDay;
//}SPDATE;


//extern SPDATE GetSpringDay(uchar GreYear,uchar GreMon,uchar GreDay); 		 //���ũ������
extern Mydate toLunar(int year,int month,int day) ;
extern bit YearFlag(uchar cYear);											 //�ж�ƽ�꣬����
extern uchar GetWeekDay(uchar cYear,uchar cMon,uchar cDay);					 //�жϵ�ǰ�������ڼ�

#endif

