#ifndef _FFT_H
#define _FFT_H
#include "common.h"
#include "arm_const_structs.h"


//FFT长度（16/64/256/1024/4096）,即采样点的个数
#define Fs  700000   //虚拟是100000  正常使用定时器是20000    //目前最快模式是700000
#define FFT_LENGTH 1024
void FFT(uint16_t * ADC_Value);

void all_result_x4(float * pSrc,float * Amp,float * rate,float * Phase);//计算各点幅值、频率、相位

extern float32_t lBufOutArray[FFT_LENGTH];
extern float32_t lBufInArray[FFT_LENGTH * 2];

extern int16_t fft_show_idx;  //用于显示的下标
#endif
