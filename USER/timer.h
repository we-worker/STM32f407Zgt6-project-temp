#ifndef _TIMER_H
#define _TIMER_H
#include "common.h"
	

void TIM2_Init(u16 auto_data,u16 fractional);
void TIM2_Init2(u16 auto_data, u16 fractional);//ȡ��tim2�ж�
extern 	float adc_data1;
extern	float adc_data2;

#endif
