#ifndef __DAC_H
#define __DAC_H	 
#include "common.h"	     			    
 	
extern u16 dac1val;
extern u8 dac1StopFlag;
extern float dac1_Vref;

void DAC1_Init(void);		//DAC通道1初始化	 	 
void Dac1_Set_Vol(u16 vol);	//设置通道1输出电压

void DAC1_Init2(void);

extern uint16_t dac1_sinval[128];
extern uint8_t dac_sin_index;
#endif

















