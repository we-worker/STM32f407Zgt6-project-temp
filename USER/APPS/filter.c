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
