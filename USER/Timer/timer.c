#include "timer.h"
#include "led.h"
#include "adc.h"
#include "math.h"
#include <string.h>
#include "fft.h"
#include "dac.h"
#include "05D_UI.h"


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


// 定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 溢出中断
    {

        Dac1_Set_Vol(dac1_sinval[dac_sin_index++]*out_with_sin);
        if(dac_sin_index>=128)
            dac_sin_index=0;


        //		//下面两行配合adc——init原始，实现定时器软触发。
        //ADC_Value[adc1_data_count++] = Get_Adc(ADC_Channel_6);
                // u16 t2 = Get_Adc(ADC_Channel_7);
//        if (adc1_data_count >= FFT_LENGTH)
//        {
//            adc1_data_count=0;
//			FFT(ADC_Value);
            //自动前移数据
            //adc1_data_count = ADC_BUFFER_SIZE;
//			memmove(&ADC_Value[0], &ADC_Value[1], (ADC_BUFFER_SIZE - 1) * sizeof(int));
//			ADC_Value[ADC_BUFFER_SIZE - 1] = Get_Adc(ADC_Channel_6);
        //}

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
    }

}
