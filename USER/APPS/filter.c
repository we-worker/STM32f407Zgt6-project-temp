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
