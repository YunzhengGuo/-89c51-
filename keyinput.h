#define int8 unsigned char
#define  KEY1     4
#define  KEY2     5
#define  KEY3     6
#define  KEY4     7
//��ֵ����
#define   KEY_NONE       	0x00
#define   KEY_ONOFF       	0x10
#define   KEY_ACCU       	0x20
#define   KEY_BUZZER      	0x40     
#define   KEY_BACKLED      	0x80     

#define   KEYCOUNT_DEF 		1             	//10ms*1=10ms  ȥ��ʱ��

int8  keyvalue=KEY_NONE;
int8  key1count_dn =0,key1count_up =0;		//����ȥ��������
int8  key2count_dn =0,key2count_up =0;
int8  key3count_dn =0,key3count_up =0;
int8  key4count_dn =0,key4count_up =0;

void keyinput(void){  
	int8  portb_temp;
    portb_temp=P3;
    if(!(portb_temp&0x10)){  //SET�����´���
    	if (key1count_up<KEYCOUNT_DEF) key1count_up= 0;	//�嵯�����������֤���������������2*KEYCOUNT_DEF
    	key1count_dn++;
     	if (key1count_dn==KEYCOUNT_DEF) 	{ keyvalue|=0x10;key1count_up= 0; }  
     	if (key1count_dn> KEYCOUNT_DEF) 	key1count_dn = KEYCOUNT_DEF;	//���ְ��¼�����
	}
	else{	 //SET��������
		if (key1count_dn<KEYCOUNT_DEF) key1count_dn=0; //�尴�¼�����,��֤���µ�����������2*KEYCOUNT_DEF
		key1count_up++;
	 	if (key1count_up==KEYCOUNT_DEF) 	{ keyvalue&=0xef;key1count_dn=0;}
     	if (key1count_up>KEYCOUNT_DEF)		key1count_up = KEYCOUNT_DEF;	
	}   
    if(!(portb_temp&0x20)){  //INC�����´���
    	if (key2count_up<KEYCOUNT_DEF) key2count_up= 0;	//�嵯�����������֤���������������2*KEYCOUNT_DEF
    	key2count_dn++;
     	if (key2count_dn==KEYCOUNT_DEF) 	{ keyvalue|=0x20;key2count_up= 0; }  
     	if (key2count_dn> KEYCOUNT_DEF) 	key2count_dn = KEYCOUNT_DEF;	//���ְ��¼�����
	}
	else{	 //INC��������
		if (key2count_dn<KEYCOUNT_DEF) key2count_dn=0; //�尴�¼�����,��֤���µ�����������2*KEYCOUNT_DEF
		key2count_up++;
	 	if (key2count_up==KEYCOUNT_DEF) 	{ keyvalue&=0xdf;key2count_dn=0;}
     	if (key2count_up>KEYCOUNT_DEF)		key2count_up = KEYCOUNT_DEF;	
	}      
     
    if(!(portb_temp&0x40)){  //DEC�����´���
    	if (key3count_up<KEYCOUNT_DEF) key3count_up= 0;	//�嵯�����������֤���������������2*KEYCOUNT_DEF
    	key3count_dn++;
     	if (key3count_dn==KEYCOUNT_DEF) 	{ keyvalue|=0x40; key3count_up= 0;}  
     	if (key3count_dn> KEYCOUNT_DEF) 	key3count_dn = KEYCOUNT_DEF;	//���ְ��¼�����
	}
	else{	//DEC��������
	
		if (key3count_dn<KEYCOUNT_DEF) key3count_dn=0; //�尴�¼�����,��֤���µ�����������2*KEYCOUNT_DEF
		key3count_up++;
	 	if (key3count_up==KEYCOUNT_DEF) 	{ keyvalue&=0xbf;key3count_dn=0;}
     	if (key3count_up>KEYCOUNT_DEF)		key3count_up = KEYCOUNT_DEF;	
	}      
}

