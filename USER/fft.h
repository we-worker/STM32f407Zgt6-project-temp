#ifndef _FFT_H
#define _FFT_H
#include "common.h"
#include "arm_const_structs.h"

//===================================================
//========================DSP库使用======================
//#define length 1024 
//void FFTx4_init(u8 ifftFlag,u8 bitReverseFlag);//FFT初始化函数（基4）
//void cFFTx4(float * pSrc);//FFT计算函数（基4）
//void cmplxFFTx4(float * pSrc,float * pDst);//对FFT结果求模值（基4）

//==================================================

//FFT长度（16/64/256/1024/4096）,即采样点的个数
#define Fs  700000   //虚拟是100000  正常使用定时器是20000    //目前最快模式是700000
#define FFT_LENGTH 1024
void FFT(void);

void all_result_x4(float * pSrc,float * Amp,float * rate,float * Phase);//计算各点幅值、频率、相位

//extern float32_t  lBufInArray[FFT_LENGTH*2];
extern float32_t lBufOutArray[FFT_LENGTH / 2];
// extern  int numHarmonics ; // 谐波数量
// extern  float harmonics[FFT_LENGTH/2]; // 谐波频率数组
extern int16_t fft_show_idx;  //用于显示的下标
#endif
