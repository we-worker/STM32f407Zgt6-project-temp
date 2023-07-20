#include "timer.h"
#include "led.h"
#include "adc.h"
#include "math.h"
#include <string.h>
#include "fft.h"
#include "dac.h"
#include "05D_UI_progress.h"
/****************************************************************************
 * 名    称: TIM2_Init(u16 auto_data,u16 fractional)
 * 功    能：定时器2初始化
 * 入口参数：auto_data: 自动重装值
 *           fractional: 时钟预分频数
 * 返回参数：无
 * 说    明：定时器溢出时间计算方法:Tout=((auto_data+1)*(fractional+1))/Ft(us)  Ft定时器时钟
 ****************************************************************************/

uint16_t adc1_data_count=0;

void TIM2_Init(u16 auto_data, u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 使能TIM2时钟

	TIM_TimeBaseInitStructure.TIM_Period = auto_data;				// 自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = fractional;			// 定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2

	//选择定时器2的触发输出为更新事件
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	    //使能定时器中断    
	

	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 允许定时器2更新中断
	TIM_Cmd(TIM2, ENABLE);					   // 使能定时器2

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				 // 定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	

}

void TIM2_Init2(u16 auto_data, u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 使能TIM2时钟

	TIM_TimeBaseInitStructure.TIM_Period = auto_data;				// 自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = fractional;			// 定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // 初始化TIM2

	//选择定时器2的触发输出为更新事件
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	    //使能定时器中断    
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 允许定时器2更新中断
	TIM_Cmd(TIM2, ENABLE);					   // 使能定时器2

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

int text=0;
// 定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 溢出中断
	{
//		//下面两行配合adc——init原始，实现定时器软触发。
//		ADC_Value[adc1_data_count] = Get_Adc(ADC_Channel_6);
		//adc1_data_count++;
		Dac1_Set_Vol(dac1_sinval[dac_sin_index++]*out_with_sin);
		if(dac_sin_index>=128)
			dac_sin_index=0;
//		if (adc1_data_count >= FFT_LENGTH)
//		{
//			
//			adc1_data_count=0;
//			FFT(ADC_Value);
			//自动前移数据
			//adc1_data_count = ADC_BUFFER_SIZE;
			
//			memmove(&ADC_Value[0], &ADC_Value[1], (ADC_BUFFER_SIZE - 1) * sizeof(int));
//			ADC_Value[ADC_BUFFER_SIZE - 1] = Get_Adc(ADC_Channel_6);
		//}
		
		
		// u16 t2 = Get_Adc(ADC_Channel_7);
		// adc_data1=(float)(kalman_filter(t1,&kal1)) * (adc1_Vref / 4096);//都使用卡尔曼滤波
		// adc_data2=(float)(kalman_filter(t2,&kal2)) * (adc1_Vref / 4096);
		// temp=temp*multiple2;//电流转换
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
	}

}
