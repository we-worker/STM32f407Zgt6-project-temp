#ifndef _TIMER_H
#define _TIMER_H
#include "common.h"
	

void TIM2_Init(u16 auto_data,u16 fractional);
//TIM2_Init2 不开启tim2中断，但开启了t2_trgo触发，可以控制adc-dma
void TIM2_Init2(u16 auto_data, u16 fractional);//ȡ��tim2�ж�


#endif
