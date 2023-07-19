#include "Screen_main.h"
#include "fft.h"
#include "adc.h"
#include "math.h"
#include <stdio.h>
#include "input_event.h"
#include "SPI.h"
#include "AD9833.h"
#include "valuepack.h"
#include "19C_main_progress.h"
#include "timer.h"


uint16_t Screen_flash_cnt = 0; // 屏幕刷新次数计数，实现简易延迟动画。
uint32_t flash=0;
uint8_t SPI_data[7];

void drawpic(void);
void display_waveform(void);
void display_fft(void);
void Display_characteristic(void);
void drawWaveform(u16 Xpoint, void *yValues, uint8_t isfloat, u16 lcdHeight, u16 y_offset, float valueRange, u16 color);
void useless_main(void);


double period, frequency, peak_to_peak, average; // 定义ADC的周期、频率、峰峰值、平均值
void ADC_progress(void);


void Screen_main(void)
{

    Screen_flash_cnt++;
		//delay_ms(1);
    if (Screen_flash_cnt >= 100)
        Screen_flash_cnt = 0;

    char display_str[30];
    BRUSH_COLOR = RED; // 显示颜色变为红色

    if (Screen_flash_cnt == 0)
    {

        // 高速时需要开关tim2实现采集，不卡顿
        if (TIM2->CR1 == 0)		   // 等待被关闭  说明FTT计算完毕
            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
    }
    if (is_show__wave == 1)
    {
        if (Screen_flash_cnt == 0)
        {
            // 清空屏幕
            LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height - 1, 0xffff);
            u16 waveform_height = lcd_height;				   // 计算波形显示区域的高度
            float adc_value_range = waveform_height / 4000.0f; // 假设ADC的取值范围为0-4095
						useless_main();
					  int show_buffer_size = Fs*10/2/frequency;						   // 绘制的范围，避免太多了。
            drawWaveform(show_buffer_size, ADC_Value, 0, lcd_height / 2, lcd_height / 2, adc_value_range, RED);
            drawWaveform(show_buffer_size, ADC2_Value, 0, lcd_height / 2, lcd_height / 2, adc_value_range, BLUE);
            // for (int i = 0; i < 7; i++)
            // {
            // 	SPI_data[i] = RecvFrom_FPGA(i, 4);
            // 	sprintf((char *)display_str, "data:%#x", SPI_data[i]); // 浮点型数据  e-01  就是除于10      /10
            // 	LCD_DisplayString(10, 200 + 12 * i, 12, display_str);  // 实际电压数值
            // }
        }
    }
    else if (is_show__wave == 2)
    {
        display_fft();
    }
    else
    {
        if (Screen_flash_cnt == 0)
            Display_characteristic2();
        // delay_ms(500);
    }
}

// 绘制波形的函数,Xpoint:x轴要绘制的点数,yValues:数据,isfloat,输入数据是否是浮点型，仅支持浮点型或u16
// lcdHeight绘制屏幕范围，y_offset:绘制高度偏移，valueRange：数据分布范围
void drawWaveform(u16 Xpoint, void *yValues, uint8_t isfloat, u16 lcdHeight, u16 y_offset, float valueRange, u16 color)
{
    u16 *yValuesU16 = (u16 *)yValues;
    float *yValuesFloat = (float *)yValues;
    // 绘制波形
    for (u16 i = 0; i < Xpoint; i++)
    {
        u16 x = i * (lcd_width * 1.0f / Xpoint);
        u16 y;
        if (isfloat)
            y = lcdHeight - (yValuesFloat[i] * valueRange) + y_offset;
        else
            y = lcdHeight - (yValuesU16[i] * valueRange) + y_offset;

        // 绘制当前ADC值的波形点
        LCD_Color_DrawPoint(x, y, color);
        // 绘制连接上一个ADC值的波形线段
        if (i > 0)
        {
            u16 prev_x = (i - 1) * (lcd_width * 1.0f / Xpoint);
            u16 prev_y;
            if (isfloat)
                prev_y = lcdHeight - (yValuesFloat[i - 1] * valueRange) + y_offset;
            else
                prev_y = lcdHeight - (yValuesU16[i - 1] * valueRange) + y_offset;
            LCD_DrawLine(prev_x, prev_y, x, y, color); // 使用指定颜色画线
        }
    }
}

void useless_main(void)
{
		ADC_progress();
    char display_str[30];

    BRUSH_COLOR = BLUE;										  // 显示颜色变为红色
    sprintf((char *)display_str, "ADC1 freq:%.4f", frequency); // 浮点型数据  e-01  就是除于10      /10
    LCD_DisplayString(10, 110, 24, display_str);			  // 实际电压数值

    sprintf((char *)display_str, "peak to p:%d", flash++); // 浮点型数据  e-01  就是除于10      /10
    LCD_DisplayString(10, 170, 24, display_str);		// 实际电压数值
}

void display_waveform()
{
    // 清空屏幕
    LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height / 2 - 1, 0xffff);
    // 计算波形显示区域的高度
    u16 waveform_height = lcd_height / 2;
    // 计算每个ADC值在屏幕上的垂直位置范围
    float adc_value_range = waveform_height / 4000.0f; // 假设ADC的取值范围为0-4095
    int show_buffer_size = 1024;					   // 绘制的范围，避免太多了。

    drawWaveform(show_buffer_size, ADC2_Value, 0, lcd_height / 2, 0, adc_value_range, RED);
}

float fft_value(int n)
{
    return sqrtf(lBufOutArray[n] * lBufOutArray[n] + lBufOutArray[n - 1] * lBufOutArray[n - 1] + lBufOutArray[n - 2] * lBufOutArray[n - 2] + lBufOutArray[n + 1] * lBufOutArray[n + 1] + lBufOutArray[n + 2] * lBufOutArray[n + 2]);
}

void display_fft()
{
    FFT(ADC_Value); // 计算fft
    // 清空屏幕
    LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height / 2 - 1, 0xffff);
    // 计算波形显示区域的高度
    u16 waveform_height = lcd_height / 2;

    // 计算每个ADC值在屏幕上的垂直位置范围
    float value_range = waveform_height / 2000.0f; // 假设ADC的取值范围为0-4095

    // 绘制波形
    drawWaveform(FFT_LENGTH / 2, lBufOutArray, 1, lcd_height / 2, 0, value_range, RED);

    char display_str[30];

    LCD_DrawLine(fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH * 2), 0, fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH * 2), lcd_height / 2 - 1, BLUE); // 使用白色画线
    if (Screen_flash_cnt == 0)
    {
        BRUSH_COLOR = BLUE;																   // 显示颜色变为红色
        sprintf((char *)display_str, "freq:%.4f", (Fs * 1.0 / FFT_LENGTH) * fft_show_idx); // 浮点型数据  e-01  就是除于10      /10
        LCD_DisplayString(10, 220, 12, display_str);									   // 实际电压数值

        float out = fft_value(fft_show_idx);

        double uo_sum = 0;
        for (int i = -3; i < 3; i++)
        {
            uo_sum += lBufOutArray[fft_show_idx + i] * lBufOutArray[fft_show_idx + i];
        }
        sprintf((char *)display_str, "value:%.4f", sqrtf(uo_sum)); // 1024/2
        LCD_DisplayString(120, 220, 12, display_str);			   // 实际电压数值																			   // 实际电压数值
    }
}

// int phases[1024];
//  绘制幅频特性曲线和相频特性曲线
void Display_characteristic()
{
    const int show_len = 256;
    int real_part[show_len] = {0};
    int imag_part[show_len] = {0};
    int key_index[show_len] = {0};
    int show_index = 0;

    int sqrt2_index = 0;
    int minsqrt2 = 10000;

    for (int i = 0; i < 150; i++)
    {
        uint16_t freq = (i + 1);
        // 设置频率
        AD9833_WaveSeting(freq * 1e2, 0, SIN_WAVE, 0); // 2KHz, 频率寄存器0，正弦波输出，初相位0
        delay_ms(1);
        TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕

        //  计算波形显示区域的高度
        u16 waveform_height = lcd_height / 2;
        // 计算每个ADC值在屏幕上的垂直位置范围
        float adc_value_range = waveform_height / 2000.0f; // 假设ADC的取值范围为0-4095
        int show_buffer_size = 50;						   // 绘制的范围，避免太多了。
        //		drawWaveform(show_buffer_size, ADC_Value, 0, lcd_height / 2, 0, adc_value_range, RED);
        //		drawWaveform(show_buffer_size, ADC2_Value, 0, lcd_height / 2, 0, adc_value_range, BLUE);
        LCD_DrawLine(100, 0, 100, lcd_height - 1, BLUE);   // 使用
        FFT(ADC_Value);									   // 计算fft
        // 找到最大频率点
        int max_i = 1;
        for (int j = 1; j < FFT_LENGTH / 2; j++)
        {
            if (lBufOutArray[max_i] < lBufOutArray[j])
                max_i = j;
        }

        double uo_sum = 0;
        for (int i = -3; i < 3; i++)
        {
            if (max_i + i <= 0)
                continue;
            uo_sum += lBufOutArray[max_i + i] * lBufOutArray[max_i + i];
        }
        uo_sum = sqrtf(uo_sum);


        float Phase1 = atan2(lBufInArray[2 * max_i + 1], lBufInArray[2 * max_i]) * 180.0f / 3.1415926f + 90 + 10.39;

        // 测量原始的FFT相位信息
        FFT(ADC2_Value); // 计算fft
        // 找到最大频率点
        max_i = 1;
        for (int j = 1; j < FFT_LENGTH / 2; j++)
        {
            if (lBufOutArray[max_i] < lBufOutArray[j])
                max_i = j;
        }
        float Phase2 = atan2(lBufInArray[2 * max_i + 1], lBufInArray[2 * max_i]) * 180.0f / 3.1415926f + 90 + 10.39;

        if (Phase2 - Phase1 < -50)
            Phase2 += 360;

        if (fabs(uo_sum - real_part[3] * 1.414f / 2) < minsqrt2)
        {
            minsqrt2 = fabs(uo_sum - real_part[3] * 1.414f / 2);
            sqrt2_index = max_i;
        }

        if (real_part[max_i / 2] == 0)
        {
            key_index[show_index] = max_i / 2;
            show_index++;
            real_part[max_i / 2] = uo_sum;					  // 幅度
            imag_part[max_i / 2] = Phase2 * 10 - Phase1 * 10; // 相位
        }
        else
        {
            real_part[max_i / 2] = (real_part[max_i / 2] + uo_sum) / 2.0f;					  // 幅度
            imag_part[max_i / 2] = (imag_part[max_i / 2] + Phase2 * 10 - Phase1 * 10) / 2.0f; // 相位
        }
    }

    // 清空屏幕
    LCD_Fill_onecolor(0, 0, lcd_width, lcd_height, 0xffff);

    if (real_part[key_index[3]] - real_part[key_index[show_index - 2]] < 100)
    {
        char display_str[30];
        int uo = real_part[key_index[3]];
        sprintf((char *)display_str, "Rvalue:%.4f", 98.6f * uo / (1572 - uo)); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);							   // 实际电压数值																			   // 实际电压数值
    }
    else
    {
        char display_str[30];
        sprintf((char *)display_str, "Cvalue:%.4f", 1 / (2 * 3.1415926 * (Fs * 1.0 / FFT_LENGTH) * sqrt2_index * 98.6f) * 1e9); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);																				// 实际电压数值																			   // 实际电压数值
    }

    u16 waveform_height = lcd_height / 2; // 计算波形显示区域的高度

    // 计算每个ADC值在屏幕上的垂直位置范围
    float adc_value_range1 = waveform_height / 2800.0f; // 假设ADC的取值范围为0-4095
    float adc_value_range2 = waveform_height / 2500.0f; // 假设ADC的取值范围为0-4095
    int show_buffer_size = show_index;					// 绘制的范围，避免太多了。

    // 绘制幅频波形,相频
    for (int i = 0; i < show_buffer_size; i++)
    {
        int index = key_index[i];
        int amplitude = real_part[index];
        int phase = imag_part[index];
        //				if(phase>0)
        //					phase=-80;
        // 计算波形点的坐标
        u16 x = i * (lcd_width * 1.0f / show_buffer_size);
        u16 y1 = lcd_height / 2 - (amplitude * adc_value_range1);
        u16 y2 = lcd_height / 4 * 3 + (phase * adc_value_range2);
        // 绘制当前幅度值的波形点
        LCD_Color_DrawPoint(x, y1, RED);

        // 绘制连接上一个幅度值的波形线段
        if (i > 0)
        {
            u16 prev_x = (i - 1) * (lcd_width * 1.0f / show_buffer_size);
            u16 prev_y1 = lcd_height / 2 - (real_part[key_index[i - 1]] * adc_value_range1);
            LCD_DrawLine(prev_x, prev_y1, x, y1, RED); // 使用指定颜色画线

            u16 prev_y2 = lcd_height / 4 * 3 + (imag_part[key_index[i - 1]] * adc_value_range2);
            LCD_DrawLine(prev_x, prev_y2, x, y2, RED); // 使用指定颜色画线
        }
    }
}

void ADC_progress()
{
    double max, min; // 最大值、最小值、峰值、
    int i, j, k; // 索引
		double total_time; // 总时间
//		int M=10; // 测量周期数
//		if(frequency>1000 && Fs==700000)
//			M=frequency/2000; // 测量周期数
//		else if(frequency>1000)
//			M=frequency/200; // 测量周期数
//		else
//			M=1;

		float E=0.01; // 中心点判断偏移量
    // 找到最大值和最小值
    max = ADC_Value[0];
    min = ADC_Value[0];
    for (i = 1; i < FFT_LENGTH; i++)
    {
        if (ADC_Value[i] > max)
            max = ADC_Value[i];
        if (ADC_Value[i] < min)
            min = ADC_Value[i];
    }

    // 计算峰值、峰峰值和平均值
    peak_to_peak = max - min;
    average = 0;
    for (i = 0; i < FFT_LENGTH; i++)
        average += ADC_Value[i];
    average /= FFT_LENGTH;

//    // 找到第一个大于平均值的元素的索引
//    for (i = 0; i < FFT_LENGTH; i++)
//        if (ADC_Value[i] > average + E)
//            break;

//    // 初始化总时间为零
//    total_time = 0;

//    // 循环测量M个周期的时间
//    for (int m = 0; m < M; m++)
//    {
//        // 找到第一个小于平均值的元素的索引
//        for (j = i + 1; j < FFT_LENGTH; j++)
//            if (ADC_Value[j] < average - E)
//                break;

//        // 找到第二个大于平均值的元素的索引
//        for (k = j + 1; k < FFT_LENGTH; k++)
//            if (ADC_Value[k] > average + E)
//                break;

//        // 累加一个周期的时间
//        total_time += (k - i) * 1.0f/(Fs);

//        // 更新i为k，继续下一个周期的测量
//        i = k;
//    }
//    // 计算周期和频率
//    period = total_time / M;
//    frequency = 1 / period;
		
		//使用FFT进行频率计算方案，待定
		FFT(ADC_Value);
				// 找到最大频率点
		int max_i = 1;
		for (int j = 1; j < FFT_LENGTH / 2; j++)
		{
				if (lBufOutArray[max_i] < lBufOutArray[j])
						max_i = j;
		}
		frequency=(Fs * 1.0 / FFT_LENGTH) * max_i;
//		if(frequency>=10e3){
//		Fs=700000;
//				ADC1_Init2();	  // 高速信号采集dma、等
//		ADC2_Init2();
//		TIM2_Init2(9, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
//		}else{
//					Fs=28000;
//				ADC1_Init2();	  // 高速信号采集dma、等
//		ADC2_Init2();
//		TIM2_Init2(499, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
//		}
		
}

