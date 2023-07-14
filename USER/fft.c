
#include "fft.h"

#include "arm_math.h"
#include "stdio.h"

#include "adc.h"

#define FFT_LENGTH 1024

float32_t lBufInArray[FFT_LENGTH * 2];
float32_t lBufOutArray[FFT_LENGTH / 2];
uint16_t ADC_Value[FFT_LENGTH];

// int numHarmonics ; // г������
// float harmonics[FFT_LENGTH/2]; // г��Ƶ������
int16_t fft_show_idx = 0; // ������ʾ���±�

// void InitBufInArray(void)
// {
//   unsigned short i;
//   float fx;
//   for (i = 0; i < FFT_LENGTH; i++)
//   {
//     fx = 1024 * sin(2 * PI * i * 13500.0f / Fs) + 512 * sin(2 * PI * i * 8500.0f / Fs) + 512 * sin(2 * PI * i * 3500.0f / Fs);

//     ADC_Value[i] = fx + 2048;
//   }
// }

float Han_Win(u16 i, float value)
{
  float Wn;
  Wn = 0.5 - 0.5 * cos((6.2831852f * (i - 1)) / (512 - 1)); // w = .5*(1 - cos(2*pi*(1:m)'/(n+1)));
  Wn = Wn * value;
  return Wn;
}

void FFT(void)
{
  //InitBufInArray();
  for (int i = 0; i < FFT_LENGTH; i++)
  {
    lBufInArray[i * 2] = adc1_datas[i]; // ʵ����ֵ//�������ADC_Value
    lBufInArray[i * 2 + 1] = 0;         // �鲿��ֵ
  }

  // ��ȥֱ����
  //	uint32_t sum=0;
  //	for(int i = 0; i<FFT_LENGTH; i++)
  //		sum = sum + lBufInArray[i * 2];
  //	uint32_t average = sum/FFT_LENGTH;
  //	for(int i = 0; i<FFT_LENGTH; i++)
  //		lBufInArray[2*i] = lBufInArray[2*i] - average;

  for (int i = 0; i < FFT_LENGTH / 2; i++)
  {
    Han_Win(i, lBufInArray[i]);
  }

  arm_cfft_f32(&arm_cfft_sR_f32_len1024, lBufInArray, 0, 1); // fft�仯
  arm_cmplx_mag_f32(lBufInArray, lBufOutArray, FFT_LENGTH);  // ȡģ

  for (int i = 1; i < FFT_LENGTH / 2; i++)
  {
    lBufOutArray[i] /= 512; // ��һ����ֵ
  }

  int16_t max_i = 1;
  for (int16_t i = 1; i < FFT_LENGTH / 2; i++)
  {
    if (lBufOutArray[max_i] < lBufOutArray[i])
      max_i = i;
  }

  //========================================
  //	    // Ѱ�ҷ�ֵг��
  //		numHarmonics=0;

  //		int segmentStart = 0; // �ֶ���ʼ����
  //    float maxAmplitude = lBufOutArray[0]; // ��ǰ�ε�����ֵ
  //		int segment_maxi=0;
  //
  //    for (int i = 1; i < FFT_LENGTH / 2; i++) {
  //        if (lBufOutArray[i] > maxAmplitude) {
  //            maxAmplitude = lBufOutArray[i];
  //					segment_maxi=i;
  //        }
  //        else if (lBufOutArray[i] < maxAmplitude * 0.1) {
  //            // ����ֵ�½�������ֵ��һ������ʱ����Ϊ��ǰ�ν�������¼г��Ƶ��
  //            harmonics[numHarmonics] = (Fs*1.0 / FFT_LENGTH) * segment_maxi;
  //            numHarmonics++;
  //
  //            // ����ﵽԤ������г���������˳�ѭ��
  //            if (numHarmonics >= FFT_LENGTH/2) {
  //                break;
  //            }

  //            // ���·ֶ���ʼ����������ֵ
  //            segmentStart = i;
  //            maxAmplitude = lBufOutArray[i];
  //        }
  //    }

  //==================================
  //		 for (int i = ; i < FFT_LENGTH / 2; i++) {
  //			if (lBufOutArray[i] > maxAmplitude) {
  //            maxAmplitude = lBufOutArray[i];
  //					numHarmonics=0;
  //					harmonics[numHarmonics]=(Fs*1.0 / FFT_LENGTH) * i;
  //				numHarmonics=1;
  //        }
  //		 }

  // ��ʾг��Ƶ��
  // printf("г��Ƶ��:\n");
  //    for (int i = 0; i < numHarmonics; i++) {
  //        printf("%.2f\n", harmonics[i]);
  //    }
}

	//��������ֵ��Ƶ�ʡ���λ
//void all_result_x4(float * pSrc,float * Amp,float * rate,float * Phase)
//{
//	for(int i=0;i<FFT_LENGTH;i++)
//	{
//		if(i==0)	Amp[0]=pDst[0]/length;	//��һ����ֱ�ӳ���size(zֱ������)
//		else Amp[i]=pDst[i]*2/length;	//���������size/2
//		rate[i]=Fs/length*i;//	����Ƶ��
//		Phase[i]=atan2(pSrc[2*i+1], pSrc[2*i]); /* atan2���Ľ����Χ��(-pi, pi], ������ */
//	}

//}




//=================�����ǻ���arm����dspд��==================�����ǻ���dsp��д��===========================
// #include "stm32_dsp.h"
// #define Fs 100000
// #define FFT_LENGTH 1024
// int32_t lBufInArray[FFT_LENGTH] = {0};
// uint16_t ADC_Value[FFT_LENGTH] = {0};
// int32_t lBufOutArray[FFT_LENGTH / 2] = {0};
// int32_t lBufMagArray[FFT_LENGTH / 2] = {0};

// void InitBufInArray(void)
//{
//   unsigned short i;
//   float fx;
//   for(i = 0; i < FFT_LENGTH; i++)
//   {
//     fx = 1024*sin(2*PI*i * 13500.0 / Fs)
//		+512*sin(2*PI*i * 8500.0 / Fs)
//		+512*sin(2*PI*i * 3500.0 / Fs);
//		ADC_Value[i] = fx+2048;
//     lBufInArray[i] = ((signed short)ADC_Value[i]) << 16;
////		printf("%d\n", ADC_Value[i]);
//  }
//}
// void GetPowerMag(void)
//{
//  signed short lX, lY;
//  float X, Y, Mag;
//  unsigned short i;
//  for(i = 0; i < FFT_LENGTH / 2; i++)
//  {
//    lX  = (lBufOutArray[i] << 16) >> 16;
//    lY  = (lBufOutArray[i] >> 16);
//    X = FFT_LENGTH * ((float)lX) / 32768;
//    Y = FFT_LENGTH * ((float)lY) / 32768;
//    Mag = sqrt(X * X + Y * Y) / FFT_LENGTH;
//    if(i == 0)
//      lBufMagArray[i] = (unsigned long)(Mag * 32768);
//    else
//      lBufMagArray[i] = (unsigned long)(Mag * 65536);
//  }
//}

// void FFT(void)
//{
//	InitBufInArray();
//	cr4_fft_1024_stm32(lBufOutArray, lBufInArray, FFT_LENGTH);
//	GetPowerMag();
//   for(int16_t i = 1; i < FFT_LENGTH/2; i++)
//   {
//     printf("%d\n",lBufMagArray[i]);
//   }
// }
