#include "fft.h"
#include "arm_math.h"
#include "stdio.h"
#include "math.h"
#include "adc.h"
#include "stdlib.h"

uint32_t Fs = 700e3; // ���������Ҫ�Զ��޸�Fs

float32_t lBufInArray[FFT_LENGTH * 2];
float32_t lBufOutArray[FFT_LENGTH];

int16_t fft_show_idx = 0; // ������ʾ���±�

// void InitBufInArray(void)
// {
//   unsigned short i;
//   float fx;
//   for (i = 0; i < FFT_LENGTH; i++)
//   {
//     fx =  512 * arm_sin_f32(2 * PI * i * 600.0f / Fs+PI/4*5)+23 * arm_sin_f32(2 * PI * i * 3000.0f / Fs+PI);

//     ADC1_Value[i] = fx + 1024;
//   }
// }

// ����������
float Han_Win(u16 i, float value)
{
	float Wn;
	Wn = 0.5 - 0.5 * arm_cos_f32((6.2831852f * (i - 1)) / (512 - 1)); // w = .5*(1 - cos(2*pi*(1:m)'/(n+1)));
	Wn = Wn * value;
	return Wn;
}

void FFT(uint16_t *ADC1_Value)
{
	// InitBufInArray();
	for (int i = 0; i < FFT_LENGTH; i++)
	{
		lBufInArray[i * 2] = ADC1_Value[i]; // ʵ����ֵ//�������ADC_Value
		lBufInArray[i * 2 + 1] = 0;		   // �鲿��ֵ
	}

	// ��ȥֱ����
	//  	uint32_t sum=0;
	//  	for(int i = 0; i<FFT_LENGTH; i++)
	//  		sum = sum + lBufInArray[i * 2];
	//  	uint32_t average = sum/FFT_LENGTH;
	//  	for(int i = 0; i<FFT_LENGTH; i++)
	//  		lBufInArray[2*i] = lBufInArray[2*i] - average;

	for (int i = 0; i < FFT_LENGTH; i++)
	{
		Han_Win(i, lBufInArray[i * 2]);
	}
	arm_cfft_f32(&arm_cfft_sR_f32_len1024, lBufInArray, 0, 1); // fft�仯
	arm_cmplx_mag_f32(lBufInArray, lBufOutArray, FFT_LENGTH);  // ȡģ

	for (int i = 1; i < FFT_LENGTH; i++)
	{
		lBufOutArray[i] /= 512; // ��һ����ֵ
	}
}

// �����ܱ�ƽ�͸��ŵķ�ʽ����������й¶���⣬���׼ȷ��fft��ѹ��ֵ��
float fft_value(int index)
{
	double uo_sum = 0;
	for (int i = -3; i < 3; i++)
	{
		if (index + i < 0)
			continue;
		uo_sum += lBufOutArray[index + i] * lBufOutArray[index + i];
	}
	return sqrtf(uo_sum);
	//return lBufOutArray[index];
}

// ���fft�����±��Ӧ��Ƶ��
float fft_freq(int index, int need_fix)
{
	float freqq = (Fs * 1.0f / FFT_LENGTH) * index;
	if (freqq <= 1100)
		freqq = 4.41393191648220e-05 * freqq * freqq + 0.965728595439243 * freqq + 26.8939478861443;
	else if (freqq <= 5200)
		freqq = 2.58219191914616e-06 * freqq * freqq + 0.988911502252750 * freqq + 48.2850854470916;
	else if (freqq <= 70000)
		freqq = -3.51278059697917e-07 * freqq * freqq + 1.05651004449479 * freqq - 783.269371576457;
	else if (freqq <= 9e5)
		freqq = -6.50181508803556e-06 * freqq * freqq + 2.06492461566215 * freqq - 40836.6316886145;
	else
		freqq = freqq;
	if (need_fix == 0)
		freqq = (Fs * 1.0f / FFT_LENGTH) * index;
	return freqq;
}
// ���Ƶ�ʶ�Ӧ��fft�����±�
int fft_index2freq(int freq)
{
	return (int)(freq / (Fs * 1.0f / FFT_LENGTH));
}

// ���fft����λ�����ֻ�����Ҳ�����λ����Ҫ�������Ҳ���λ����ſ��Եõ���
float fft_phase(int index)
{
	return atan2(lBufInArray[2 * index + 1], lBufInArray[2 * index]) * 180.0f / 3.1415926f + 90 + 10.39;
}

int fft_max_index(void)
{
	uint32_t fft_index_ = 0; // ���� FFT ����е����ֵ
	float32_t fft_maxValue = 0;

	// int16_t peaks[FFT_LENGTH / 2]; // fft��ֵ����
	// uint16_t peaks_num = 0;        // fft��ֵ����
	arm_max_f32(&lBufOutArray[1], FFT_LENGTH / 2 - 1, &fft_maxValue, &fft_index_); // ʹ�� arm_max_f32 ���������ҵ� FFT ����е����ֵ��������
	//                                                                            //	    // Ѱ�ҷ�ֵг��
	// AMPD(lBufOutArray, FFT_LENGTH / 2, peaks, &peaks_num);
	//========================================
	return fft_index_;
}

int comp(const void *p1, const void *p2)
{
	return lBufOutArray[*((int *)p2)] > lBufOutArray[*((int *)p1)] ? 1 : -1;
}
// ����һ��������ʵ��AMPD�㷨���ҵ���ͬ�ķ�ֵ�㡣
// ������data��һ��һά���飬count������ĳ���
// ����ֵ��һ��ָ�򲨷���������ֵ�����飬��-1����
//  // int16_t peaks[FFT_LENGTH / 2]; // fft��ֵ����
// uint16_t peaks_num = 0;        // fft��ֵ����
void AMPD(int *peaks, int *peaks_num)
{
	int count = FFT_LENGTH / 2;
	// ����һ����dataͬ����С���������飬���ڴ洢����ĸ���
	int p_data[FFT_LENGTH / 2] = {0};

	// ����һ�����飬���ڴ洢ÿһ�еĺ�
	int arr_rowsum[FFT_LENGTH / 4];

	// ����ÿһ�У�����ÿһ�еĺ�
	for (int k = 1; k < count / 2 + 1; k++)
	{
		int row_sum = 0;
		for (int i = k; i < count - k; i++)
		{
			if (lBufOutArray[i] > lBufOutArray[i - k] && lBufOutArray[i] > lBufOutArray[i + k])
			{
				row_sum -= 1;
			}
		}
		arr_rowsum[k] = row_sum;
	}
	// �ҵ�arr_rowsum����Сֵ������
	int min_index = 0;
	for (int i = 1; i < count / 2 + 1; i++)
	{
		if (arr_rowsum[i] < arr_rowsum[min_index])
		{
			min_index = i;
		}
	}

	//======================================
	// ��󴰿ڳ��ȵ�����Сֵ������
	int max_window_length = 5; // ����Ϊmin_index ��������Զ����� �������ֶ�����

	int min_level = (lBufOutArray[2] + lBufOutArray[FFT_LENGTH / 2 - 2]) / 2;
	min_level = 50; // �ж�Ϊ��ֵ����Сֵ��ֵ��

	//=======================
	// ����ÿһ�У�ͳ�Ʋ���ĸ���

	for (int i = 1; i < count - max_window_length; i++)
	{
		for (int k = 1; k < max_window_length + 1; k++)
		{
			if (lBufOutArray[i] > lBufOutArray[i - k] && lBufOutArray[i] > lBufOutArray[i + k])
			{
				p_data[i] += 1;
			}
		}
	}
	// ��ʼ��peaksΪ-1    // ����һ�����飬���ڴ洢�������ڵ�����ֵ
	for (int i = 0; i < count; i++)
	{
		peaks[i] = -1;
	}
	// ����p_data���ҵ��������ڵ�����ֵ������peaks��
	int j = 0;
	for (int i = 0; i < count; i++)
	{
		if (p_data[i] >= max_window_length) // ԭ����==
		{
			if (lBufOutArray[i] < min_level)
				continue;
			peaks[j] = i;
			j++;
		}
	}
	*peaks_num = j;
  qsort(peaks, *peaks_num, sizeof(int), comp); // ���ú���qsort
}
