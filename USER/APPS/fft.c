#include "fft.h"
#include "arm_math.h"
#include "stdio.h"
#include "math.h"
#include "adc.h"
#include "stdlib.h"

uint32_t Fs = 10; // ���������Ҫ�Զ��޸�Fs

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

//     ADC_Value[i] = fx + 1024;
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

void FFT(uint16_t *ADC_Value)
{
  // InitBufInArray();
  for (int i = 0; i < FFT_LENGTH; i++)
  {
    lBufInArray[i * 2] = ADC_Value[i]; // ʵ����ֵ//�������ADC_Value
    lBufInArray[i * 2 + 1] = 0;        // �鲿��ֵ
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

//�����ܱ�ƽ�͸��ŵķ�ʽ����������й¶���⣬���׼ȷ��fft��ѹ��ֵ��
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
}

//���fft�����±��Ӧ��Ƶ��
float fft_freq(int index)
{
  float freqq=(Fs * 1.0f / FFT_LENGTH) * index;

  return freqq+freqq*0.023f;
}
//���Ƶ�ʶ�Ӧ��fft�����±�
int fft_index2freq(int freq)
{
  return (int)(freq/(Fs * 1.0f / FFT_LENGTH));
}

//���fft����λ�����ֻ�����Ҳ�����λ����Ҫ�������Ҳ���λ����ſ��Եõ���
float fft_phase(index){
  return atan2(lBufInArray[2 * index + 1], lBufInArray[2 * index]) * 180.0f / 3.1415926f + 90 + 10.39;
}

int fft_max_index(void){
  uint32_t fft_index_ = 0; // ���� FFT ����е����ֵ
  float32_t fft_maxValue = 0;

  // int16_t peaks[FFT_LENGTH / 2]; // fft��ֵ����
  // uint16_t peaks_num = 0;        // fft��ֵ����
  arm_max_f32(&lBufOutArray[1], FFT_LENGTH / 2-1, &fft_maxValue, &fft_index_); // ʹ�� arm_max_f32 ���������ҵ� FFT ����е����ֵ��������
  //                                                                            //	    // Ѱ�ҷ�ֵг��
  // AMPD(lBufOutArray, FFT_LENGTH / 2, peaks, &peaks_num);
  //========================================
  return fft_index_;
}


// ����һ��������ʵ��AMPD�㷨���ҵ���ͬ�ķ�ֵ�㡣
// ������data��һ��һά���飬count������ĳ���
// ����ֵ��һ��ָ�򲨷���������ֵ�����飬��-1����
//  // int16_t peaks[FFT_LENGTH / 2]; // fft��ֵ����
  // uint16_t peaks_num = 0;        // fft��ֵ����
void AMPD(int16_t *peaks, uint16_t *peaks_num)
{
  int count=FFT_LENGTH/2;
  // ����һ����dataͬ����С���������飬���ڴ洢����ĸ���
  int p_data[FFT_LENGTH / 2] = {0};

  // ����һ�����飬���ڴ洢ÿһ�еĺ�
  int arr_rowsum[FFT_LENGTH / 4] = {0};

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
  // ��󴰿ڳ��ȵ�����Сֵ������
  int max_window_length = min_index;
  // ����ÿһ�У�ͳ�Ʋ���ĸ���
  for (int k = 1; k < max_window_length + 1; k++)
  {
    for (int i = k; i < count - k; i++)
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
    if (p_data[i] == max_window_length)
    {
      peaks[j] = i;
      j++;
    }
  }
  *peaks_num = j;
}
