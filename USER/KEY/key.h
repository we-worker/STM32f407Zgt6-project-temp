#ifndef __KEY_H
#define __KEY_H	 
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

//����IO�˿ڶ���
#define KEY0 		PFin(9)   
#define KEY1 		PFin(8)		
#define KEY2 		PFin(7)		
#define KEY3 	  PFin(6)		


//����ֵ����
#define KEY0_DATA	  1
#define KEY1_DATA	  2
#define KEY2_DATA	  3
#define KEY3_DATA   4

//��������
extern u8   keydown_data;    //�������º�ͷ��ص�ֵ
extern u8   keyup_data;      //����̧�𷵻�ֵ
extern u16  key_time;
extern u8   key_tem; 

//��������
void KEY_Init(void);	      //IO��ʼ��
void key_scan(u8 mode);  		//����ɨ�躯��	

#endif
