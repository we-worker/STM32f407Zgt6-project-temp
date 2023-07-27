#ifndef __ADC_H
#define __ADC_H	
#include "common.h" 
 #include "fft.h"
//ADC_BUFFER_SIZE 最好等于FFT——LENTH
#define  ADC_BUFFER_SIZE FFT_LENGTH
// extern  u16 ADC_Value[ADC_BUFFER_SIZE];
extern uint16_t ADC_Value[ADC_BUFFER_SIZE];
extern uint16_t ADC2_Value[ADC_BUFFER_SIZE];
 							   
void  ADC1_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
 void ADC1_Init2(void) ;     //init2配置了tim2定时器触发adc采集，dma数据搬运，速度更快
 void ADC2_Init2(void); //PA7 的DMA
#endif 















