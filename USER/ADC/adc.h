#ifndef __ADC_H
#define __ADC_H	
#include "common.h" 
 

#define  ADC_BUFFER_SIZE 1024
extern  u16 adc1_datas[ADC_BUFFER_SIZE];
 							   
void  ADC1_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
 void ADC1_Init2(void) ;     //init2配置了tim2定时器触发adc采集，dma数据搬运，速度更快
#endif 















