#include "Wave_page.h"
#include "fft.h"
#include "adc.h"
#include "timer.h"
#include "stdbool.h"
#include "timer4.h"
#include "filter.h"

// #include "AD9959.h"
// #include "AD9959_Outset.h"
#include "pid.h"
#include "AD9833.h"
#include "pid.h"
#include "valuepack.h"
#define _constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

Button btn2[2];
Button btn_data;
Button Freq_open;

bool is_show_data = 0;
bool is_out_freq = 0;

int adc_show_size = FFT_LENGTH; // 绘制的范围，实现自动缩放时间轴

float max_freq = 0;                     // fft得到的最大频率
float frequency, peak_to_peak, average; // 定义ADC的周期、频率、峰峰值、平均值

Button button1 = {10, 180, 50, 20, 0xfe67, "<<", 16, 0, Button_press_handle, 1};
Button btn1;
// LCD_DrawButton(&btn1); //画出按钮btn1

////////////////////////////////////////

// int deta_f_value[2] = {0};
int f_value[30] = {0};       // 谐波变化保存数组。
int f_value_index = 0;       // 谐波变化保存数组当前下标。
int f_value_sum = 0;         // 保存谐波变化差值之和
int f_value_sum_min = 10000; // 保存谐波变化差值之和 最小值
double sum_min_freq = 0;     // 保存谐波变化差值之和 的最小频率

int phase_1 = 360;             // 保存dds输出波形 相位
double set_freq = 100e3 - 1.2; // 保存dds输出波形 频率
// int last_f_value = 0;
// int f_value_flag = 1;

int base_freq[2][3] = {0};   // 信号1，2的频率；0代表正弦波，1代表三角波。
int feed_back_freq[2] = {0}; // 信号测量反馈的频率下标。也就是观测用的频率下标。

// rcPara_t di = {0, 0};
// pid的参数初始化
PID phase_pid = {
    .kp = 15,
    .ki = 0.04,
    .kd = 0,
    .error_acc = 0,
    .lastError = 0

};

/// @brief ////////////////////////////
void ADC_progress();

void Screen_page1_init(void)
{

    LCD_Fill_onecolor(0, 30, lcd_width - 1, lcd_height - 1, 0xffff);

    btn2[0] = button_init(10, 300, 50, 20, 0xfe67, "<<", 16, 0, Button_press_handle, 1);  // 初始化一个按钮btn1
    btn2[1] = button_init(180, 300, 50, 20, 0xfe67, ">>", 16, 0, Button_press_handle, 2); // 初始化一个按钮btn1

    LCD_DrawButton(&btn2[0]); // 画出每个按钮
    LCD_DrawButton(&btn2[1]); // 画出每个按钮

    btn_data = button_init(65, 300, 50, 20, 0xfe67, "Datas", 16, 0, Button_press_handle, 3); // 初始化一个按钮btn1
    LCD_DrawButton(&btn_data);                                                               // 画出开始按钮

    Freq_open = button_init(125, 300, 50, 20, 0xfe67, "freqPB6", 12, 0, Button_press_handle, 4); // 初始化一个按钮btn1
    LCD_DrawButton(&Freq_open);                                                                  // 画出开始按钮

    ADC1_Init2(); // 高速信号采集dma、等
    ADC2_Init2();
    ADC3_Init2();
    TIM2_Init2(19, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
    Fs = 700000;
}

void display_fft(int which_one)
{
    if (which_one == 1)
        FFT(ADC_Value); // 计算fft
    else if (which_one == 2)
        FFT(ADC2_Value); // 计算fft
    else
        FFT(ADC3_Value); // 计算fft

    // 计算波形显示区域的高度
    u16 waveform_height = lcd_height / 2;

    // 计算每个ADC值在屏幕上的垂直位置范围
    float value_range = 2000.0f; // 假设ADC的取值范围为0-4095
    int offset = lcd_height / 2;
    // 绘制波形
    drawWaveform(FFT_LENGTH / 2, lBufOutArray, 1, lcd_height / 3, offset, value_range, RED);

    char display_str[30];

    LCD_DrawLine(fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH * 2), offset, fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH * 2), offset + lcd_height / 3, BLUE); // 使用白色画线

    BRUSH_COLOR = BLUE;                                                   // 显示颜色变为红色
    sprintf((char *)display_str, "freq:%.4f", fft_freq(fft_show_idx, 1)); // 浮点型数据  e-01  就是除于10      /10
    LCD_DisplayString(10, 280, 12, display_str);                          // 实际电压数值

    float out = fft_value(fft_show_idx);
    sprintf((char *)display_str, "value:%.4f", out); // 1024/2
    LCD_DisplayString(120, 280, 12, display_str);    // 实际电压数值																			   // 实际电压数值

    // auto_change(); // 自动切换量程和自动调整Fs，等效采样。

    sprintf((char *)display_str, "Fs:%d    ", Fs); // 1024/2
    LCD_DisplayString(0, 268, 12, display_str);    // 实际电压数值

    sprintf((char *)display_str, "freq:%.4f", max_freq); // 1024/2
    LCD_DisplayString(120, 268, 12, display_str);        // 实际电压数值
}

void display_fft_datas(void)
{
    int peaks[FFT_LENGTH / 2]; // fft峰值数组
    int peaks_num = 0;         // fft峰值数量
    AMPD(peaks, &peaks_num);

    char display_str[30];
    BACK_COLOR = 0x9f31; // 背景色
    BRUSH_COLOR = BLACK; // 画笔颜色
    // LCD_Fill_onecolor(0, 30, lcd_width,lcd_height/2, 0x9f31);
    int show_index = 0;
    for (int i = 0; i < peaks_num; i++)
    {
        float out = fft_value(peaks[i]);
        if (out <= 30 || out > 100e4)
            continue;

        if (out > 630)
        {
            base_freq[i][1] = 0; // 设置为正弦波。

            sprintf((char *)display_str, "value:%.4f  sin", out);          // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str); // 实际电压数值
        }
        else
        {
            base_freq[i][1] = 1; // 设置为三角波。

            sprintf((char *)display_str, "value:%.4f  thi", out);          // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str); // 实际电压数值
        }

        float freq = fft_freq(peaks[i], 0);
        base_freq[i][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;
        sprintf((char *)display_str, "freq:%.4f", ((int)(freq / 5e3 + 0.5)) * 5e3); // 1024/2
        LCD_DisplayString(10, show_index * 12 + 30, 12, display_str);               // 实际电压数值
        show_index++;
    }
    LCD_Fill_onecolor(0, show_index * 12 + 30, lcd_width, lcd_height / 4, 0x9f31);
    BACK_COLOR = WHITE; // 背景色

    set_freq = base_freq[0][0] - 0.5; // 设置频率
    feed_back_freq[0] = fft_index2freq(base_freq[0][0]);
    f_value_sum_min = 1000000; // 保存谐波变化差值之和 最小值
}

float f_GradDesO(int *error, float *phase) // 传入error[2]与freq[2]
{
    int temp_freq;
    float learn_speed = 0.01;
    temp_freq = -learn_speed * error[1] * (error[1] - error[0]) / (phase[1] - phase[0]);

    float re_phase = phase[1] + temp_freq;
    while (re_phase >= 4095)
        re_phase -= 4095;
    while (re_phase < 0)
        re_phase += 4095;
    return re_phase;
}

int freq_change_count = 0;
float peak2peak_last = 0;

void DDS_out(void)
{

    char display_str[30];

    f_value[f_value_index++] = fft_value(feed_back_freq[0]);
    if (freq_change_count == 1)
    {
        set_freq = sum_min_freq;
        AD9833_WaveSeting(set_freq, 0, SIN_WAVE, 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
        freq_change_count++;
    }
    if (f_value_index >= 30 && freq_change_count < 1)
    {

        f_value_index = 0;

        f_value_sum = 0;
        for (int i = 1; i < 30; i++)
        {
            f_value_sum += abs(f_value[i] - f_value[i - 1]);
        }
        if (f_value_sum_min > f_value_sum)
        {
            f_value_sum_min = f_value_sum;
            sum_min_freq = set_freq;
        }
        if (f_value_sum > 3 * f_value_sum_min && set_freq > sum_min_freq + 0.2)
        {
            set_freq = sum_min_freq - 0.2;
            freq_change_count++;
        }
        else if (f_value_sum > f_value_sum_min / 2)
        {
            set_freq += 0.1;
            AD9833_WaveSeting(set_freq, 0, SIN_WAVE, 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
            delay_ms(1);
        }

        else
        {
            //					phase_1=0;
            //					for(int i=0;i<360;i++){
            //							if(fft_value(146)>30){

            //							phase_1+=1;
            //						  AD9833_WaveSeting(set_freq, 0, SIN_WAVE, phase_1); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
            // delay_ms(100);								//1HZ
            //							}
            //					}
        }

        if (set_freq > base_freq[0][0] + 1.2)
        {
            set_freq = sum_min_freq - 0.2;
        }
    }
    else
    {

        // 相位控制
        if (freq_change_count >= 2)
        {
            float nowpeak = ADC_peak2peak(ADC2_Value);
            // nowpeak = (nowpeak + peak2peak_last) / 2;
                    //    if(nowpeak-peak2peak_last>0) {
                    //        phase_1=phase_1+120;
                    //    } else if(nowpeak-peak2peak_last< -0) {
                    //        phase_1=phase_1-115;
                    //    }
                    //    else {
                    //         phase_1=phase_1;
                    //    }
                    // phase_1 = phase_1 + 5*(nowpeak-peak2peak_last);
            int deta = Follow_PID(&phase_pid, nowpeak-peak2peak_last);
            phase_1 = phase_1 + deta;

            // sprintf((char *)display_str, "%.2f,%d,%.2f,%d\n", fft_value(feed_back_freq[0]), f_value_sum, peak2peak_last, phase_1); // 1024/2
            // USART_OUT(USART6, (uint8_t *)display_str);
            peak2peak_last = nowpeak;
            // // phase_1=phase_1+FOLLOW;
            while (phase_1 >= 4095)
                phase_1 -= 4095;
            while (phase_1 < 0)
                phase_1 += 4095;
            SetAD9833PhaseRegister(phase_1); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
            //			delay_ms(1);
            // AD9833_WaveSeting(set_freq,0,SIN_WAVE,0);
        }
    }

    // sprintf((char *)display_str, "deta_f_value0:%d\t", deta_f_value[0]); // 1024/2
    // LCD_DisplayString(120, 86, 12, display_str);                         // 实际电压数值
    // deta_f_value[0] = deta_f_value[1];
    BACK_COLOR = 0x9f31;                                           // 背景色
    sprintf((char *)display_str, "set_freq:%.2f", set_freq);       // 1024/2
    LCD_DisplayString(120, 50, 12, display_str);                   // 实际电压数值
    sprintf((char *)display_str, "f_value_sum:%d  ", f_value_sum); // 1024/2
    LCD_DisplayString(120, 62, 12, display_str);                   // 实际电压数值
    // sprintf((char *)display_str, "deta_f_value1:%d", deta_f_value[1]); // 1024/2
    // LCD_DisplayString(120, 74, 12, display_str);                       // 实际电压数值
}

void out_freq_show()
{
    char display_str[30];
    BACK_COLOR = 0x9f31; // 背景色
    if (Frequency > 10000)
        sprintf((char *)display_str, "OutFreq:%dkhz      ", (uint32_t)(Frequency / 1000.0f + 0.5));
    else
        sprintf((char *)display_str, "OutFreq:%d      ", Frequency);
    LCD_DisplayString(120, 36, 12, display_str); // 实际电压数值
    BACK_COLOR = WHITE;                          // 背景色
}

void Screen_page1_flash(void)
{

    Screen_flash_cnt++;
    // delay_ms(1);
    if (Screen_flash_cnt >= 1)
        Screen_flash_cnt = 0;

    for (char i = 0; i < 2; i++)
        LCD_CheckButton(&btn2[i]);
    LCD_CheckButton(&btn_data);
    LCD_CheckButton(&Freq_open);

    freq_change_count = 3;

    if (Screen_flash_cnt == 0)
    {

        // 高速时需要开关tim2实现采集，不卡顿
        if (TIM2->CR1 == 0)        // 等待被关闭  说明FTT计算完毕
            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕

        if (is_show_data)
        {

            display_fft(3);
            display_fft_datas();
            // 开启AD转换器
            ADC_Cmd(ADC3, ENABLE);
        }
        else
        {
            // 开启AD转换器
            ADC_Cmd(ADC3, DISABLE);
            // drawWaveform(adc_show_size, ADC3_Value, 0, lcd_height / 2 - 30, 30, 4096, RED);
            //            ADC_progress();
            if (freq_change_count < 2)
                display_fft(2);
            if (base_freq[0][0] != 0)
                DDS_out();
        }
        if (is_out_freq)
            out_freq_show();

        // 为了fft指示线快速移动
        for (int i = 0; i < 2; i++)
        {
            if (btn2[i].state == 1)
            {
                Page1_handle(btn2[i].id);
            }
        }
    }
}

void Page1_handle(int id)
{
    if (id == 1)
    {
        fft_show_idx -= 1;
    }
    else if (id == 2)
    {
        fft_show_idx += 1;
    }
    else if (id == 3)
    {
        is_show_data = 1 - is_show_data;
    }
    else if (id == 4)
    {
        is_out_freq = 1 - is_out_freq;
        if (is_out_freq)
        {
            freq_Measure_init();
        }
        else
        {
            TIM_Cmd(TIM4, DISABLE);
            TIM_Cmd(TIM3, DISABLE);
        }
    }
}

void ADC_progress()
{

    // 计算峰值、峰峰值和平均值
    peak_to_peak = ADC_peak2peak(ADC_Value);
    average = ADC_average(ADC_Value);
    frequency = ADC_freq(ADC_Value);
    char display_str[30];

    BACK_COLOR = 0x9f31;                                      // 背景色
    sprintf((char *)display_str, "adc_freq:%.0f", frequency); // 1024/2
    LCD_DisplayString(120, 50, 12, display_str);              // 实际电压数值

    sprintf((char *)display_str, "average:%.2f", average);           // 1024/2
    LCD_DisplayString(120, 62, 12, display_str);                     // 实际电压数值
    sprintf((char *)display_str, "peak_to_peak:%.4f", peak_to_peak); // 1024/2
    LCD_DisplayString(120, 74, 12, display_str);                     // 实际电压数值
    BACK_COLOR = WHITE;                                              // 背景色
}

////////////////////////////////////////////////////////////////////////////////

// 数字滤波器

//								di.k=2*PI*9000/Fs;
//				di.lVal=0;
//        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC_Value[i]);
//					ADC_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC_Value[i]);
//					ADC_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC_Value[i]);
//					ADC_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC_Value[i]);
//					ADC_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC_Value[i]);
//					ADC_Value[i]=_constrain( temp ,0,4095);
//        }
//

void auto_change(void)
{
    char display_str[30];
    //======================自动缩放波形======================
    int max_indx = fft_max_index();
    max_freq = (fft_freq(max_indx, 1) + max_freq) / 2;
    adc_show_size = Fs / max_freq * 5;

    adc_show_size = _constrain(adc_show_size, 5, 1024);
    //======================自动缩放FFT_FS======================

    //    if (Frequency > 120e3) // 高频使用等效采用
    //    {
    //        uint32_t test_freq = ((uint32_t)(1.0f * Frequency / 1e3 + 0.5)) * 1e3; // 这里没有完成，需要知道信号的准确频率才行
    //        int fenpin = Frequency / 10e5 + 1;                                     // 定时器频率与信号频率的倍数。
    //        uint32_t Fs_tim = test_freq / fenpin;
    //        // int delay=0;//自动控制延迟时间
    //        int delay = 250e3 / Frequency + 1;
    //        // double Fs_tim_true = 14e6 / ((14e6 / Fs_tim) + delay);
    //        // Fs = (uint32_t)(1.0f * test_freq / (test_freq - fenpin * Fs_tim_true) * Fs_tim_true + 0.5);
    //        Fs = 1.0f * Frequency / max_freq * Fs; // 也不根据采集的波形计算了，直接让fft解算的最大频率值等于频率计的。

    //        TIM2_Init2((u16)(42e6 / Fs_tim) - 1 + delay, 1);                                 // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
    //        sprintf((char *)display_str, "Tim2:%d    ", ((u16)(14e6 / Fs_tim) - 1 + delay)); // 1024/2
    //        LCD_DisplayString(70, 268, 12, display_str);                                     // 实际电压数值
    //    }
    //    else
    //    {
    //        // 需要10k下100hz分辨率时 Fs_temp = (int)(max_freq / 1000 / 2 + 1) * 1 * 7e3;
    //       uint32_t Fs_tem	p = (int)(max_freq / 1000 / 2 + 1) * 2 * 7e3;
    //       if (fabs(Fs_temp / (2 * 7e3) - Fs / (2 * 7e3)) >= 3) // 防止来回疯狂变化。
    //       {
    //           Fs = _constrain(Fs_temp, 1e3, 7e5);
    //           // Fs=30000;
    //           TIM2_Init2((u16)(14e6 / Fs) - 1, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
    //       }
    //       sprintf((char *)display_str, "Tim2:%d    ", (u16)(14e6 / Fs) - 1); // 1024/2
    //       LCD_DisplayString(70, 268, 12, display_str);                       // 实际电压数值
    //    }
}