#ifndef _Filter_H
#define _Filter_H
#include "common.h"

//k值的计算方法。
//符号:Sr-采样率(sampling rate,次/秒),f-截止频率(Hz),Pi-圆周率(3.14...)
//k=(2*Pi*f)/Sr
typedef struct
{
  float k;//滤波系数
  float lVal;//上次计算值
}rcPara_t;


//低通滤波：
//rcPara-指向滤波参数
//val-采样值
//返回值-滤波结果
float rcLfFiter(rcPara_t *rcPara, float val);

//高通滤波：
float rcHpFiter(rcPara_t *rcPara, float val);

#endif