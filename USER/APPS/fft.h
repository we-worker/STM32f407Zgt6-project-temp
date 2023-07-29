#ifndef _FFT_H
#define _FFT_H
#include "common.h"
#include "arm_const_structs.h"

// FFT长度（16/64/256/1024/4096）,即采样点的个数
extern uint32_t Fs; // 虚拟是100000  正常使用定时器是20000    //目前最快模式是700000
#define FFT_LENGTH 1024  //记得修改  arm_cfft_f32(&arm_cfft_sR_f32_len4096, lBufInArray, 0, 1); // fft变化

void FFT(uint16_t *ADC_Value);

extern float32_t lBufOutArray[FFT_LENGTH];
extern float32_t lBufInArray[FFT_LENGTH * 2];

extern int16_t fft_show_idx; // 用于显示的下标

// 采用周边平和根号的方式，补偿能量泄露问题，获得准确的fft电压幅值。
float fft_value(int index);
// 求解fft数组下标对应的频率
float fft_freq(int index,int need_fix);
// 求解频率对应的fft数组下标
int fft_index2freq(int freq);
// 获得fft的相位，如果只是正弦波的相位，需要两个正弦波相位做差才可以得到。
float fft_phase(int index);

// 定义一个函数，实现AMPD算法,找fft峰值,以lBufOutArray为输入数组
// 参数：data是一个一维数组，count是数组的长度
// 返回值：一个指向波峰所在索引值的数组，以-1结束
void AMPD(int *peaks, int *peaks_num);
int fft_max_index(void);
void find_max_indices(uint16_t* max_index, uint16_t max_num);


#endif
