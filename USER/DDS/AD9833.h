#ifndef __AD9833_H
#define __AD9833_H	 
#include "common.h"


#define TRI_WAVE 	0  		//������ǲ�
#define SIN_WAVE 	1		//������Ҳ�
#define SQU_WAVE 	2		//�������

void AD9833_Init(void);
void AD9833_WaveSeting(double frequence,unsigned int frequence_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_Init_GPIO(void);
void AD9833_AmpSet(unsigned char amp);		 				    
#endif
