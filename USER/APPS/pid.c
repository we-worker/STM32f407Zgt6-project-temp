#include "pid.h"
#include "dac.h"
#include "adc.h"
#include "lcd.h"
#include "stdio.h"

extern u16 dac1val;
extern u8 dac1StopFlag;
extern float dac1_Vref;

// pid的参数初始化
PID dac_pid = {
	.kp = 100,
	.ki = 1,
	.kd = 50,
	.error_acc = 0,
	.lastError = 0

};

// 电压调整pid
int Follow_PID(PID *s_PID, float error)
{
	float iError = 0;
	int output = 0;
	iError =-error; // 误差值计算
	s_PID->error_acc += iError;	  // 积分
	output = s_PID->kp * iError + s_PID->ki * s_PID->error_acc * 0.5f + s_PID->kd * iError - s_PID->lastError;

	s_PID->lastError = iError; // error值存储

	return (output);
}

void Auto_Justment(void)
{

	// dac1val=dac1val+Follow_PID(&dac_pid);
	if (dac1val > 4000) // 限定最大输出电压
		dac1val = 4000;

	DAC_SetChannel1Data(DAC_Align_12b_R, dac1val); // 设置DAC值

	u16 dac_data = DAC_GetDataOutputValue(DAC_Channel_1); // 读取前面设置DAC的值

	char display_str[30];
	float dac_out = (float)dac_data * (dac1_Vref / 4096);  // 得到DAC电压值
	sprintf((char *)display_str, "DAC VOL:%.4f", dac_out); // 浮点型数据  e-01  就是除于10      /10  //%06e
	LCD_DisplayString(10, 50, 24, display_str);			   // 实际电压数值
}

typedef struct
{
	float Last_P;	  // 上次估算协方差 不可以为0 ! ! ! ! !
	float Now_P;	  // 当前估算协方差
	float kalman_old; // 卡尔曼滤波器上次数据
	float Kg;		  // 卡尔曼增益
	float Q;		  // 过程噪声协方差
	float R;		  // 观测噪声协方差
} Kalman;

Kalman kal1 = {1, 0, 0, 1, 0.0001, 10};
Kalman kal2 = {1, 0, 0, 1, 0.0001, 10};

unsigned long kalman_filter(unsigned long Input_Value, Kalman *kal)
{
	float x_k, temp;

	float Z_k;
	float kalman_adc;

	Z_k = Input_Value;

	if (fabs(kal->kalman_old - Input_Value) >= 50)
	{
		x_k = Input_Value * 0.582f + kal->kalman_old * 0.418f;
	}
	else
	{
		x_k = kal->kalman_old;
	}
	temp = x_k;
	// 预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
	kal->Now_P = kal->Last_P + kal->Q;
	// 卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
	kal->Kg = kal->Now_P / (kal->Now_P + kal->R);
	// 更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
	kalman_adc = temp + kal->Kg * (Z_k - kal->kalman_old);
	// 更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
	kal->Now_P = (1 - kal->Kg) * kal->Last_P;
	kal->Last_P = kal->Now_P;

	kal->kalman_old = kalman_adc;

	return kalman_adc;
}
// adc_data1=(float)(kalman_filter(t1,&kal1)) * (adc1_Vref / 4096);//都使用卡尔曼滤波
// adc_data2=(float)(kalman_filter(t2,&kal2)) * (adc1_Vref / 4096);