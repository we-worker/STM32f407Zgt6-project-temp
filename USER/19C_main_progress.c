#include "19C_main_progress.h"

#include "adc.h"
#include "math.h"
#include <stdio.h>
#include "AD9833.h"


//=========================19年国赛C题主程序代码============================
//=======================================================================

//恒流源控制程序
void I_control(void){
	



}


// 定义一个用于控制dds模块扫频的函数
// 输入参数：
// start_freq: 扫频起始频率，单位Hz
// end_freq: 扫频终止频率，单位Hz
// step_freq: 扫频步进频率，单位Hz
// real_part: 用于保存扫频结果的数组指针
// show_len: 扫频结果的数组长度
void dds_sweep(uint16_t start_freq, uint16_t end_freq, uint16_t step_freq, uint64_t *real_part, int show_len)
{
	// 计算扫频次数
	int sweep_count = (end_freq - start_freq) / step_freq + 1;
	// 判断数组长度是否足够
	if (show_len < sweep_count)
	{
		return; // 如果不足够，直接返回
	}
	// 循环扫频
	for (int i = 0; i < sweep_count; i++)
	{
		uint16_t freq = start_freq + i * step_freq; // 计算当前频率
		// 设置频率
		AD9833_WaveSeting(freq , 0, SIN_WAVE, 0); // 频率寄存器0，正弦波输出，初相位0
		delay_ms(1);
		TIM_Cmd(TIM2, ENABLE); // 使能定时器2
		while (TIM2->CR1 != 0)
		{
		} // 等待被关闭，说明采样完毕
		uint64_t uo_sum = 0;
		for (int i = 0; i < 1024; i++)
		{
			uo_sum += ADC_Value[i];
		}
		uo_sum = uo_sum / 1024;
		real_part[i] = uo_sum; // 幅度
	}
}




//  绘制幅频特性曲线
void Display_characteristic2()
{
	const int show_len = 256;
	int real_part[show_len] = {0};
	int key_index[show_len] = {0};
	int show_index = 0;

	int sqrt2_index = 0;
	int minsqrt2 = 10000;
	int k_freq = 1e2;

	for (int i = 0; i < show_len; i++)
	{
		uint16_t freq = (i + 1);
		// 设置频率
		AD9833_WaveSeting(freq * k_freq, 0, SIN_WAVE, 0); // 2KHz, 频率寄存器0，正弦波输出，初相位0
		delay_ms(1);
		TIM_Cmd(TIM2, ENABLE); // 使能定时器2
		while (TIM2->CR1 != 0)
		{
		} // 等待被关闭，说明采样完毕
		uint64_t uo_sum = 0;
		for (int i = 0; i < 1024; i++)
		{
			uo_sum += ADC_Value[i];
		}
		uo_sum = uo_sum / 1024;
		real_part[i] = uo_sum; // 幅度
	}

	// 清空屏幕
	LCD_Fill_onecolor(0, 0, lcd_width, lcd_height, 0xffff);

	if (real_part[3] - real_part[show_len - 2] < 100)
	{
		char display_str[30];
		int uo = real_part[3];
		sprintf((char *)display_str, "Rvalue:%.4f", 98.6f * uo / (1572 - uo)); // 1024/2
		LCD_DisplayString(50, 24, 24, display_str);							   // 实际电压数值																			   // 实际电压数值
	}
	else
	{
		for (int i = 0; i < show_len; i++)
		{
			if (fabs(real_part[i] - real_part[3] * 1.414f / 2) < minsqrt2)
			{
				minsqrt2 = fabs(real_part[i] - real_part[3] * 1.414f / 2);
				sqrt2_index = i;
			}
		}

		char display_str[30];
		sprintf((char *)display_str, "Cvalue:%.4f", 1 / (2 * 3.1415926 * (k_freq)*sqrt2_index * 98.6f) * 1e9); // 1024/2
		LCD_DisplayString(50, 24, 24, display_str);															   // 实际电压数值																			   // 实际电压数值
	}

	u16 waveform_height = lcd_height / 2; // 计算波形显示区域的高度

	// 计算每个ADC值在屏幕上的垂直位置范围
	float adc_value_range1 = waveform_height / 2800.0f; // 假设ADC的取值范围为0-4095
	int show_buffer_size = show_len;					// 绘制的范围，避免太多了。

	// 绘制幅频波形,相频
	for (int i = 0; i < show_buffer_size; i++)
	{
		int index = key_index[i];
		int amplitude = real_part[index];
		//				if(phase>0)
		//					phase=-80;
		// 计算波形点的坐标
		u16 x = i * (lcd_width * 1.0f / show_buffer_size);
		u16 y1 = lcd_height / 2 - (amplitude * adc_value_range1);
		// 绘制当前幅度值的波形点
		LCD_Color_DrawPoint(x, y1, RED);

		// 绘制连接上一个幅度值的波形线段
		if (i > 0)
		{
			u16 prev_x = (i - 1) * (lcd_width * 1.0f / show_buffer_size);
			u16 prev_y1 = lcd_height / 2 - (real_part[key_index[i - 1]] * adc_value_range1);
			LCD_DrawLine(prev_x, prev_y1, x, y1, RED); // 使用指定颜色画线
		}
	}
}
