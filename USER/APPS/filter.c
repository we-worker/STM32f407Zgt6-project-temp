#include "filter.h"



//低通滤波：
//rcPara-指向滤波参数
//val-采样值
//返回值-滤波结果
float rcLfFiter(rcPara_t *rcPara, float val)
{
    rcPara->lVal=((float)val*rcPara->k+rcPara->lVal*(1-rcPara->k));
    return rcPara->lVal;
}

//高通滤波：
float rcHpFiter(rcPara_t *rcPara, float val)
{
    rcPara->lVal=((float)val*rcPara->k+rcPara->lVal*(1-rcPara->k));
    //    return -(val-rcPara->lVal);//滤波结果
    return (val-rcPara->lVal);//如果直接返回滤波结果，滤波后图像是倒转的，在心电图等一些场合，需要将图像再镜像过来
}


//===============一阶低通滤波器==================
//  lpfdata += (1.0 / (1.0 + 1.0/(2.0f * 3.14f *T*fc)))*(rawdata - lpfdata );
// lpfdata ： 滤波后的数据。
// rawdata ： 滤波前的原始数据。
// T： 数据的采样频率的倒数，即采样周期，单位是秒。
// fc : 截止频率。截止频率就是超过该频率的数据（噪声）都被过滤掉，只保留低于该截止频率的数据。

//=======================二阶IIR低通滤波器============
//  差分公式 y(n) = b0*x(n) + b1*x(n-1) + b2*x(n-2) - a1*y(n-1) - a2*y(n-2);
//  但是一般不用上面的公式，而是使用下面的共用延时模块的公式
//  	d(n) = x(n) -a1*d(n-1) - a2*d(n-2);
//  	y(n) = b0*d(n) + b1*d(n-1) + b2*d(n-2);
