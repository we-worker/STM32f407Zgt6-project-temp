#include "fft.h"
#include "arm_math.h"
#include "stdio.h"
#include "math.h"
#include "adc.h"
#include "stdlib.h"

uint32_t Fs = 10; // 方便后续需要自动修改Fs

float32_t lBufInArray[FFT_LENGTH * 2];
float32_t lBufOutArray[FFT_LENGTH];



int16_t fft_show_idx = 0; // 用于显示的下标

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

// 汉宁窗函数
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
    lBufInArray[i * 2] = ADC_Value[i]; // 实部赋值//虚拟测试ADC_Value
    lBufInArray[i * 2 + 1] = 0;        // 虚部赋值
  }

  // 减去直流量
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
  arm_cfft_f32(&arm_cfft_sR_f32_len1024, lBufInArray, 0, 1); // fft变化
  arm_cmplx_mag_f32(lBufInArray, lBufOutArray, FFT_LENGTH);  // 取模

  for (int i = 1; i < FFT_LENGTH; i++)
  {
    lBufOutArray[i] /= 512; // 归一到幅值
  }

}

//采用周边平和根号的方式，补偿能量泄露问题，获得准确的fft电压幅值。
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

//求解fft数组下标对应的频率
float fft_freq(int index)
{
  float freqq=(Fs * 1.0f / FFT_LENGTH) * index;

  return freqq+freqq*0.023f;
}
//求解频率对应的fft数组下标
int fft_index2freq(int freq)
{
  return (int)(freq/(Fs * 1.0f / FFT_LENGTH));
}

//获得fft的相位，如果只是正弦波的相位，需要两个正弦波相位做差才可以得到。
float fft_phase(index){
  return atan2(lBufInArray[2 * index + 1], lBufInArray[2 * index]) * 180.0f / 3.1415926f + 90 + 10.39;
}

int fft_max_index(void){
  uint32_t fft_index_ = 0; // 查找 FFT 输出中的最大值
  float32_t fft_maxValue = 0;

  // int16_t peaks[FFT_LENGTH / 2]; // fft峰值数组
  // uint16_t peaks_num = 0;        // fft峰值数量
  arm_max_f32(&lBufOutArray[1], FFT_LENGTH / 2-1, &fft_maxValue, &fft_index_); // 使用 arm_max_f32 函数快速找到 FFT 输出中的最大值及其索引
  //                                                                            //	    // 寻找峰值谐波
  // AMPD(lBufOutArray, FFT_LENGTH / 2, peaks, &peaks_num);
  //========================================
  return fft_index_;
}


// 定义一个函数，实现AMPD算法，找到不同的峰值点。
// 参数：data是一个一维数组，count是数组的长度
// 返回值：一个指向波峰所在索引值的数组，以-1结束
//  // int16_t peaks[FFT_LENGTH / 2]; // fft峰值数组
  // uint16_t peaks_num = 0;        // fft峰值数量
void AMPD(int16_t *peaks, uint16_t *peaks_num)
{
  int count=FFT_LENGTH/2;
  // 创建一个和data同样大小的整型数组，用于存储波峰的个数
  int p_data[FFT_LENGTH / 2] = {0};

  // 创建一个数组，用于存储每一行的和
  int arr_rowsum[FFT_LENGTH / 4] = {0};

  // 遍历每一行，计算每一行的和
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
  // 找到arr_rowsum中最小值的索引
  int min_index = 0;
  for (int i = 1; i < count / 2 + 1; i++)
  {
    if (arr_rowsum[i] < arr_rowsum[min_index])
    {
      min_index = i;
    }
  }
  // 最大窗口长度等于最小值的索引
  int max_window_length = min_index;
  // 遍历每一行，统计波峰的个数
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
  // 初始化peaks为-1    // 创建一个数组，用于存储波峰所在的索引值
  for (int i = 0; i < count; i++)
  {
    peaks[i] = -1;
  }
  // 遍历p_data，找到波峰所在的索引值，存入peaks中
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
