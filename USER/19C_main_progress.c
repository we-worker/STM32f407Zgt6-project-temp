#include "19C_main_progress.h"

#include "adc.h"
#include "math.h"
#include <stdio.h>
#include "AD9833.h"

#define PI 3.1415926f

//=========================19�����C�����������============================
//=======================================================================

//����Դ���Ƴ���
void I_control(void) {




}


// ����һ�����ڿ���ddsģ��ɨƵ�ĺ���
// ���������
// start_freq: ɨƵ��ʼƵ�ʣ���λHz
// end_freq: ɨƵ��ֹƵ�ʣ���λHz
// step_freq: ɨƵ����Ƶ�ʣ���λHz
// real_part: ���ڱ���ɨƵ���������ָ��
// show_len: ɨƵ��������鳤��
void dds_sweep(uint16_t start_freq, uint16_t end_freq, uint16_t step_freq, uint64_t *real_part, int show_len)
{
    // ����ɨƵ����
    int sweep_count = (end_freq - start_freq) / step_freq + 1;
    // �ж����鳤���Ƿ��㹻
    if (show_len < sweep_count)
    {
        return; // ������㹻��ֱ�ӷ���
    }
    // ѭ��ɨƵ
    for (int i = 0; i < sweep_count; i++)
    {
        uint16_t freq = start_freq + i * step_freq; // ���㵱ǰƵ��
        // ����Ƶ��
        AD9833_WaveSeting(freq, 0, SIN_WAVE, 0);  // Ƶ�ʼĴ���0�����Ҳ����������λ0
        delay_ms(1);
        TIM_Cmd(TIM2, ENABLE); // ʹ�ܶ�ʱ��2
        while (TIM2->CR1 != 0)
        {
        } // �ȴ����رգ�˵���������
        uint64_t uo_sum = 0;
        for (int i = 0; i < 1024; i++)
        {
            uo_sum += ADC_Value[i];
        }
        uo_sum = uo_sum / 1024;
        real_part[i] = uo_sum; // ����
    }
}




//  ���Ʒ�Ƶ��������
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
        // ����Ƶ��
        AD9833_WaveSeting(freq * k_freq, 0, SIN_WAVE, 0); // 2KHz, Ƶ�ʼĴ���0�����Ҳ����������λ0
        delay_us(10);
        TIM_Cmd(TIM2, ENABLE); // ʹ�ܶ�ʱ��2
        while (TIM2->CR1 != 0)
        {
        } // �ȴ����رգ�˵���������
        uint64_t uo_sum = 0;
        for (int i = 0; i < 1024; i++)
        {
            uo_sum += ADC_Value[i];
        }
        uo_sum = uo_sum / 1024;
        real_part[i] = uo_sum; // ����
    }

    // �����Ļ
		 
    LCD_Fill_onecolor(0, 0, lcd_width, lcd_height, 0xffff);

    if(real_part[3] < real_part[show_len - 2]-100) {
        for (int i = 0; i < show_len; i++)
        {
            if (fabs(real_part[i] - real_part[show_len-1] * 1.414f / 4) < minsqrt2)
            {
                minsqrt2 = fabs(real_part[i] - real_part[show_len-1] * 1.414f / 4);
                sqrt2_index = i;
            }
        }
        char display_str[30];

        float R3=98.2f,Rs=0.5f;
        float w=2*PI* (k_freq)*(sqrt2_index+1);
        float L=sqrtf((R3*R3+2*R3*Rs-7*Rs*Rs)/7/(w*w))*1e6;
        sprintf((char *)display_str, "Lvalue:%.4f",L*0.9); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);							   // ʵ�ʵ�ѹ��ֵ
    } else if (real_part[3] - real_part[show_len - 2] < 200)
    {
        char display_str[30];
        int uo = real_part[show_len/2];
        sprintf((char *)display_str, "Rvalue:%.4f", 98.2f * uo / (1098 - uo)); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);							   // ʵ�ʵ�ѹ��ֵ																			   // ʵ�ʵ�ѹ��ֵ
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
        sprintf((char *)display_str, "Cvalue:%.4f", 1 / (2 * PI * (k_freq)*(sqrt2_index+1) * 92.2f) * 1e9); // 1024/2
        LCD_DisplayString(50, 24, 24, display_str);															   // ʵ�ʵ�ѹ��ֵ																			   // ʵ�ʵ�ѹ��ֵ
    }

    u16 waveform_height = lcd_height / 2; // ���㲨����ʾ����ĸ߶�

    // ����ÿ��ADCֵ����Ļ�ϵĴ�ֱλ�÷�Χ
    float adc_value_range1 = waveform_height / 2800.0f; // ����ADC��ȡֵ��ΧΪ0-4095
    int show_buffer_size = show_len;					// ���Ƶķ�Χ������̫���ˡ�

    // ���Ʒ�Ƶ����,��Ƶ
    for (int i = 0; i < show_buffer_size; i++)
    {
        int amplitude = real_part[i];
        //				if(phase>0)
        //					phase=-80;
        // ���㲨�ε������
        u16 x = i * (lcd_width * 1.0f / show_buffer_size);
        u16 y1 = lcd_height / 2 - (amplitude * adc_value_range1);
        // ���Ƶ�ǰ����ֵ�Ĳ��ε�
        LCD_Color_DrawPoint(x, y1, RED);

        // ����������һ������ֵ�Ĳ����߶�
        if (i > 0)
        {
            u16 prev_x = (i - 1) * (lcd_width * 1.0f / show_buffer_size);
            u16 prev_y1 = lcd_height / 2 - (real_part[i-1] * adc_value_range1);
            LCD_DrawLine(prev_x, prev_y1, x, y1, RED); // ʹ��ָ����ɫ����
        }
    }
		AD9833_WaveSeting(1000, 0, SIN_WAVE, 0);
		delay_ms(1000);
}