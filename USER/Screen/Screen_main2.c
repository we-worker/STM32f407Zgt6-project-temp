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
#include "UI.h"

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
        // AD9833_WaveSeting(freq * 1e2, 0, SIN_WAVE, 0); // 2KHz, 频率寄存器0，正弦波输出，初相位0
        delay_ms(1);
        TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕

        //  计算波形显示区域的高度
        u16 waveform_height = lcd_height / 2;
        // 计算每个ADC值在屏幕上的垂直位置范围
        float adc_value_range = waveform_height / 2000.0f; // 假设ADC的取值范围为0-4095
        int show_buffer_size = 50;                         // 绘制的范围，避免太多了。
        //		drawWaveform(show_buffer_size, ADC1_Value, 0, lcd_height / 2, 0, adc_value_range, RED);
        //		drawWaveform(show_buffer_size, ADC2_Value, 0, lcd_height / 2, 0, adc_value_range, BLUE);
        LCD_DrawLine(100, 0, 100, lcd_height - 1, BLUE); // 使用
        FFT(ADC1_Value);                                  // 计算fft
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
            real_part[max_i / 2] = uo_sum;                    // 幅度
            imag_part[max_i / 2] = Phase2 * 10 - Phase1 * 10; // 相位
        }
        else
        {
            real_part[max_i / 2] = (real_part[max_i / 2] + uo_sum) / 2.0f;                    // 幅度
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
        LCD_DisplayString(50, 24, 24, display_str);                            // 实际电压数值																			   // 实际电压数值
    }
    else
    {
        char display_str[30];
        sprintf((char *)display_str, "Cvalue:%.4f", 1 / (2 * 3.1415926 * (Fs * 1.0 / FFT_LENGTH) * sqrt2_index * 98.6f) * 1e9); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);                                                                             // 实际电压数值																			   // 实际电压数值
    }

    u16 waveform_height = lcd_height / 2; // 计算波形显示区域的高度

    // 计算每个ADC值在屏幕上的垂直位置范围
    float adc_value_range1 = waveform_height / 2800.0f; // 假设ADC的取值范围为0-4095
    float adc_value_range2 = waveform_height / 2500.0f; // 假设ADC的取值范围为0-4095
    int show_buffer_size = show_index;                  // 绘制的范围，避免太多了。

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

//  绘制幅频特性曲线
void Display_characteristic2()
{
    const int show_len = 256;
    int real_part[show_len] = {0};
    int show_index = 0;

    int sqrt2_index = 0;
    int minsqrt2 = 10000;
    int k_freq = 1e2;

    for (int i = 0; i < show_len; i++)
    {
        uint16_t freq = (i + 1);
        // 设置频率
        AD9833_WaveSeting(freq * k_freq, 0, SIN_WAVE, 0); // 2KHz, 频率寄存器0，正弦波输出，初相位0
        delay_us(10);
        TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕
        uint64_t uo_sum = 0;
        for (int i = 0; i < 1024; i++)
        {
            uo_sum += ADC1_Value[i];
        }
        uo_sum = uo_sum / 1024;
        real_part[i] = uo_sum; // 幅度
    }

    // 清空屏幕

    LCD_Fill_onecolor(0, 0, lcd_width, lcd_height, 0xffff);

    if (real_part[3] < real_part[show_len - 2] - 100)
    {
        for (int i = 0; i < show_len; i++)
        {
            if (fabs(real_part[i] - real_part[show_len - 1] * 1.414f / 4) < minsqrt2)
            {
                minsqrt2 = fabs(real_part[i] - real_part[show_len - 1] * 1.414f / 4);
                sqrt2_index = i;
            }
        }
        char display_str[30];

        float R3 = 98.2f, Rs = 0.5f;
        float w = 2 * PI * (k_freq) * (sqrt2_index + 1);
        float L = sqrtf((R3 * R3 + 2 * R3 * Rs - 7 * Rs * Rs) / 7 / (w * w)) * 1e6;
        sprintf((char *)display_str, "Lvalue:%.4f", L * 0.9); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);           // 实际电压数值
    }
    else if (real_part[3] - real_part[show_len - 2] < 200)
    {
        char display_str[30];
        int uo = real_part[show_len / 2];
        sprintf((char *)display_str, "Rvalue:%.4f", 98.2f * uo / (1098 - uo)); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);                            // 实际电压数值																			   // 实际电压数值
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
        sprintf((char *)display_str, "Cvalue:%.4f", 1 / (2 * PI * (k_freq) * (sqrt2_index + 1) * 92.2f) * 1e9); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);                                                             // 实际电压数值																			   // 实际电压数值
    }

    u16 waveform_height = lcd_height / 2; // 计算波形显示区域的高度

    // 计算每个ADC值在屏幕上的垂直位置范围
    float adc_value_range1 = waveform_height / 2800.0f; // 假设ADC的取值范围为0-4095
    int show_buffer_size = show_len;                    // 绘制的范围，避免太多了。

    // 绘制幅频波形,相频
    for (int i = 0; i < show_buffer_size; i++)
    {
        int amplitude = real_part[i];
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
            u16 prev_y1 = lcd_height / 2 - (real_part[i - 1] * adc_value_range1);
            LCD_DrawLine(prev_x, prev_y1, x, y1, RED); // 使用指定颜色画线
        }
    }
    AD9833_WaveSeting(1000, 0, SIN_WAVE, 0);
    delay_ms(1000);
}

//  绘制幅频特性曲线
void Display_characteristic3(void)
{
    const int show_len = 256;
    int real_part[show_len] = {0};
    int show_index = 0;

    int sqrt2_index = 0;
    int minsqrt2 = 10000;
    int k_freq = 1e2;

    for (int i = 0; i < show_len; i++)
    {
        uint16_t freq = (i + 1);
        // 设置频率
        AD9833_WaveSeting(freq * k_freq, 0, SIN_WAVE, 0); // 2KHz, 频率寄存器0，正弦波输出，初相位0
        delay_us(10);
        TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕

        real_part[i] = ADC_average(ADC1_Value); // 幅度
    }
    AD9833_WaveSeting(1000, 0, SIN_WAVE, 0);
}

// 粗扫+精细扫描代码框架，功能还未实现，留作准备。
void Display_characteristic4(void)
{

    // 定义变量
    float freq_start = 1000; // Hz
    float freq_end = 2000;   // Hz
    float freq_step = 10;    // Hz
    int num_steps = (freq_end - freq_start) / freq_step;
    float real_part[num_steps];
    float imag_part[num_steps];
    float amplitude[num_steps];
    float max_amplitude = 0;
    float max_frequency = 0;

    // 扫描频率范围并记录每个频率的幅度
    for (int i = 0; i < num_steps; i++)
    {
        float freq = freq_start + i * freq_step;
        AD9833_WaveSeting(freq, 0, SIN_WAVE, 0); // 设置频率
        delay(5);
        real_part[i] = ADC_average(ADC1_Value); // 获取幅度
    }

    // 使用二分法查找幅度最大的频率
    int left = 0;
    int right = num_steps - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (amplitude[mid] > max_amplitude)
        {
            max_amplitude = amplitude[mid];
            max_frequency = freq_start + mid * freq_step;
        }
        if (amplitude[mid] < amplitude[mid + 1])
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    // 在该频率附近进行更精细的扫描，以获得更准确的峰值
    freq_step = 10; // Hz
    freq_start = max_frequency - freq_step;
    freq_end = max_frequency + freq_step;
    num_steps = (freq_end - freq_start) / freq_step;
    for (int i = 0; i < num_steps; i++)
    {
        float freq = freq_start + i * freq_step;
        AD9833_WaveSeting(freq, 0, SIN_WAVE, 0); // 设置频率
        delay(5);
        real_part[i] = ADC_average(ADC1_Value); // 获取幅度
    }
}
