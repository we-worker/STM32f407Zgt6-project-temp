#include "filter.h"



//��ͨ�˲���
//rcPara-ָ���˲�����
//val-����ֵ
//����ֵ-�˲����
float rcLfFiter(rcPara_t *rcPara, float val)
{
    rcPara->lVal=((float)val*rcPara->k+rcPara->lVal*(1-rcPara->k));
    return rcPara->lVal;
}

//��ͨ�˲���
float rcHpFiter(rcPara_t *rcPara, float val)
{
    rcPara->lVal=((float)val*rcPara->k+rcPara->lVal*(1-rcPara->k));
    //    return -(val-rcPara->lVal);//�˲����
    return (val-rcPara->lVal);//���ֱ�ӷ����˲�������˲���ͼ���ǵ�ת�ģ����ĵ�ͼ��һЩ���ϣ���Ҫ��ͼ���پ������
}


//===============һ�׵�ͨ�˲���==================
//  lpfdata += (1.0 / (1.0 + 1.0/(2.0f * 3.14f *T*fc)))*(rawdata - lpfdata );
// lpfdata �� �˲�������ݡ�
// rawdata �� �˲�ǰ��ԭʼ���ݡ�
// T�� ���ݵĲ���Ƶ�ʵĵ��������������ڣ���λ���롣
// fc : ��ֹƵ�ʡ���ֹƵ�ʾ��ǳ�����Ƶ�ʵ����ݣ��������������˵���ֻ�������ڸý�ֹƵ�ʵ����ݡ�

//=======================����IIR��ͨ�˲���============
//  ��ֹ�ʽ y(n) = b0*x(n) + b1*x(n-1) + b2*x(n-2) - a1*y(n-1) - a2*y(n-2);
//  ����һ�㲻������Ĺ�ʽ������ʹ������Ĺ�����ʱģ��Ĺ�ʽ
//  	d(n) = x(n) -a1*d(n-1) - a2*d(n-2);
//  	y(n) = b0*d(n) + b1*d(n-1) + b2*d(n-2);
