//ADS1118.H�ļ�
#ifndef __ADS9833_H_
#define __ADS9833_H_

#include "common.h"

#define s_wave 1
#define t_wave 2
#define sin_wave 3

#define F_mclk 25000000                 //9833���Ƶ��Ϊ25Mhz
#define M_mclk 268435456                //2��28�η�


void AD9833_WaveOut(unsigned int mode,unsigned long Freq);
void AD9833_init(void);
#endif