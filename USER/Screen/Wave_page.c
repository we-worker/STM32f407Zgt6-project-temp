#include "Wave_page.h"
#include "fft.h"
#include "adc.h"
#include "timer.h"
#include "stdbool.h"
#include "timer4.h"
#include "filter.h"
// #include "05D_UI.h"

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

Button Debug;
bool Debug_Mode = 0; //    调试模式。
int Debug_phase[21][21] = {53};

Button btnPhase;
bool is_phase = 0;
int phase_set = 0;

bool is_show_data = 0;
bool is_out_freq = 0;

int adc_show_size = FFT_LENGTH; // 绘制的范围，实现自动缩放时间轴

float max_freq = 0;                     // fft得到的最大频率
float frequency, peak_to_peak, average; // 定义ADC的周期、频率、峰峰值、平均值

Button button1 = {10, 180, 50, 20, 0xfe67, "<<", 16, 0, Button_press_handle, 1};
Button btn1;
// LCD_DrawButton(&btn1); //画出按钮btn1

int freq_change_count = 0;

////////////////////////////////////////

// int deta_f_value[2] = {0};
float f_value[30] = {0};     // 谐波变化保存数组。
int f_value_index = 0;       // 谐波变化保存数组当前下标。
int f_value_sum = 0;         // 保存谐波变化差值之和
int f_value_sum_min = 10000; // 保存谐波变化差值之和 最小值
double sum_min_freq = 0;     // 保存谐波变化差值之和 的最小频率
float sum_min_freqs[5][2] = {10000};

int phase_1 = 360;             // 保存dds输出波形 相位
int phase_2 = 360;             // 保存dds输出波形 相位
double set_freq = 100e3 - 1.2; // 保存dds输出波形 频率
// int last_f_value = 0;
// int f_value_flag = 1;

int base_freq[2][3] = {0};   // 信号1，2的频率；0代表正弦波，1代表三角波。
int feed_back_freq[2] = {0}; // 信号测量反馈的频率下标。也就是观测用的频率下标。

// rcPara_t di = {0, 0};
// pid的参数初始化
PID phase_pid1 = {
    .kp = 0.75,
    .ki = 0,
    .kd = -0.4,
    .error_acc = 0,
    .lastError = 0

};
PID phase_pid2 = {
    .kp = 0.75,
    .ki = 0,
    .kd = -0.4,
    .error_acc = 0,
    .lastError = 0

};
/// @brief ////////////////////////////


void Screen_page1_init(void)
{

    LCD_Fill_onecolor(0, 30, lcd_width - 1, lcd_height - 1, 0xffff);

    btn2[0] = button_init(10, 300, 50, 20, 0xfe67, "<<", 16, 0, Button_press_handle, 1);  // 初始化一个按钮btn1
    btn2[1] = button_init(180, 300, 50, 20, 0xfe67, ">>", 16, 0, Button_press_handle, 2); // 初始化一个按钮btn1

    LCD_DrawButton(&btn2[0]); // 画出每个按钮
    LCD_DrawButton(&btn2[1]); // 画出每个按钮

    btn_data = button_init(65, 300, 50, 20, 0xfe67, "Double", 16, 0, Button_press_handle, 3); // 初始化一个按钮btn1
    LCD_DrawButton(&btn_data);                                                                // 画出开始按钮

    Freq_open = button_init(125, 300, 50, 20, 0xfe67, "Start", 12, 0, Button_press_handle, 4); // 初始化一个按钮btn1
    LCD_DrawButton(&Freq_open);                                                                // 画出开始按钮
    Debug = button_init(180, 100, 50, 20, 0xfe67, "start2", 12, 0, Button_press_handle, 5);     // 初始化一个按钮btn1
    LCD_DrawButton(&Debug);                                                                    // 画出开始按钮
    btnPhase = button_init(65, 280, 50, 20, 0xfe67, "Phase", 12, 0, Button_press_handle, 6);   // 初始化一个按钮btn1
    LCD_DrawButton(&btnPhase);                                                                 // 画出开始按钮

    ADC1_Init2(); // 高速信号采集dma、等
    ADC2_Init2();
    // ADC12_Init3();
    ADC3_Init2();
    TIM2_Init2(19, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
    Fs = 700000;
}

void display_fft(int which_one)
{
    if (which_one == 1)
        FFT(ADC1_Value); // 计算fft
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

    if (peaks[0] > peaks[1])
    {
        int t = peaks[0];
        peaks[0] = peaks[1];
        peaks[1] = t;
    }
    if (fabs(fft_value(peaks[0]) - fft_value(peaks[1])) > 0.15 * fft_value(fft_max_index()))
    {
        if (fft_value(peaks[0]) - fft_value(peaks[1]) > 0)
        {
            base_freq[0][1] = SIN_WAVE; // 设置为正弦波。
            float freq = fft_freq(peaks[0], 0);
            base_freq[0][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;

            sprintf((char *)display_str, "value:%.4f  sin", fft_value(peaks[0])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
            base_freq[1][1] = TRI_WAVE; // 设置为三角。
            freq = fft_freq(peaks[1], 0);
            base_freq[1][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;

            sprintf((char *)display_str, "value:%.4f  TRI", fft_value(peaks[1])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
        }
        else
        {
            base_freq[0][1] = TRI_WAVE; // 设置为正弦波。
            float freq = fft_freq(peaks[0], 0);
            base_freq[0][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;

            sprintf((char *)display_str, "value:%.4f  TRI", fft_value(peaks[0])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
            base_freq[1][1] = SIN_WAVE; // 设置为正弦波。
            freq = fft_freq(peaks[1], 0);
            base_freq[1][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;

            sprintf((char *)display_str, "value:%.4f  sin", fft_value(peaks[1])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
        }
    }
    else
    {

        // for (int i = 0; i < 2; i++)
        // {
        float out1 = fft_value(peaks[0]);
        float out2 = fft_value(peaks[1]);

        // float xiebo = fft_value(fft_index2freq(fft_freq(peaks[i], 1)) * 3);
        // sprintf((char *)display_str, "xiebo:%.4f;", xiebo); // 1024/2
        // LCD_DisplayString(120, 100, 12, display_str);       // 实际电压数值
        if ((out2 + out1) / 2 > 520)
        {
            base_freq[0][1] = SIN_WAVE; // 设置为正弦波。
            base_freq[1][1] = SIN_WAVE; // 设置为正弦波。

            sprintf((char *)display_str, "value:%.4f  sin", out1);         // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str); // 实际电压数值
            show_index++;
            sprintf((char *)display_str, "value:%.4f  sin", out2);         // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str); // 实际电压数值
            show_index++;
        }
        else
        {
            base_freq[0][1] = TRI_WAVE; // 设置为三角波。
            base_freq[1][1] = TRI_WAVE; // 设置为三角波。

            sprintf((char *)display_str, "value:%.4f  thi", out1);         // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str); // 实际电压数值
            show_index++;
            sprintf((char *)display_str, "value:%.4f  thi", out2);         // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str); // 实际电压数值
            show_index++;
        }

        float freq = fft_freq(peaks[0], 0);
        base_freq[0][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;
        freq = fft_freq(peaks[1], 0);
        base_freq[1][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;
        // }
    }
    show_index = 0;
    sprintf((char *)display_str, "freq:%d  ;", base_freq[0][0]);  // 1024/2
    LCD_DisplayString(10, show_index * 12 + 30, 12, display_str); // 实际电压数值
    show_index++;
    sprintf((char *)display_str, "freq:%d  ;", base_freq[1][0]);  // 1024/2
    LCD_DisplayString(10, show_index * 12 + 30, 12, display_str); // 实际电压数值
    show_index++;
    LCD_Fill_onecolor(0, show_index * 12 + 30, lcd_width, lcd_height / 4, 0x9f31);
    BACK_COLOR = WHITE; // 背景色

    //测试用途
//    base_freq[0][0]=20e3;
//    base_freq[1][0]=60e3;
//    base_freq[0][1]=1;
//    base_freq[1][1]=1;
    if (fft_value(peaks[0]) < 30) // 没有输入
       base_freq[0][0]=0;
    if (fft_value(peaks[1]) < 30) // 没有输入
       base_freq[1][0]=0;


    set_freq = base_freq[0][0] - 0; // 设置频率
    feed_back_freq[0] = fft_index2freq(base_freq[0][0]);
    f_value_sum_min = 1000000; // 保存谐波变化差值之和 最小值


    AD9833_WaveSeting(set_freq, 0, base_freq[0][1], 0, 1); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
    set_freq = base_freq[1][0] - 0;                        // 设置频率
    AD9833_WaveSeting(set_freq, 0, base_freq[1][1], 0, 2); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
    freq_change_count = 3;

}


void display_fft_datas2(void)
{
    int peaks[FFT_LENGTH / 2]; // fft峰值数组
    int peaks_num = 0;         // fft峰值数量
    AMPD(peaks, &peaks_num);

    char display_str[30];
    BACK_COLOR = 0x9f31; // 背景色
    BRUSH_COLOR = BLACK; // 画笔颜色
    // LCD_Fill_onecolor(0, 30, lcd_width,lcd_height/2, 0x9f31);
    int show_index = 0;

    if (peaks[0] > peaks[1])
    {
        int t = peaks[0];
        peaks[0] = peaks[1];
        peaks[1] = t;
    }
    if (fft_freq(peaks[0],1)>40e3 && fft_value(peaks[0])>100 &&fft_value(peaks[0]*3)>15)
    {
        base_freq[0][1] = TRI_WAVE; // 设置为三角。
        sprintf((char *)display_str, "value:%.4f  TRI", fft_value(peaks[1])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
    }
    else if (fft_value(peaks[0])>100 && fft_value(peaks[0]*3)>15&& fft_value(peaks[0]*5)>15){
        base_freq[0][1] = TRI_WAVE; // 设置为三角。
        sprintf((char *)display_str, "value:%.4f  TRI", fft_value(peaks[1])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
    }else{
        base_freq[0][1] = SIN_WAVE; // 设置为正弦波。
        sprintf((char *)display_str, "value:%.4f  sin", fft_value(peaks[0])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
    }
    float freq = fft_freq(peaks[0], 0);
    base_freq[0][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;

    if (fft_freq(peaks[1],1)>55e3 && fft_value(peaks[1])>100 &&fft_value(peaks[1]*3)>15)
    {
        base_freq[1][1] = TRI_WAVE; // 设置为三角。
        sprintf((char *)display_str, "value:%.4f  TRI", fft_value(peaks[1])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
    }
    else if (fft_value(peaks[1])>100 && fft_value(peaks[1]*3)>15&& fft_value(peaks[1]*5)>15){
        base_freq[1][1] = TRI_WAVE; // 设置为三角。
        sprintf((char *)display_str, "value:%.4f  TRI", fft_value(peaks[1])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
    }else{
        base_freq[1][1] = SIN_WAVE; // 设置为正弦波。
        sprintf((char *)display_str, "value:%.4f  sin", fft_value(peaks[1])); // 1024/2
            LCD_DisplayString(120, show_index * 12 + 30, 12, display_str);        // 实际电压数值
            show_index++;
    }
    freq = fft_freq(peaks[1], 0);
    base_freq[1][0] = ((int)(freq / 5e3 + 0.5)) * 5e3;

    show_index = 0;
    sprintf((char *)display_str, "freq:%d  ;", base_freq[0][0]);  // 1024/2
    LCD_DisplayString(10, show_index * 12 + 30, 12, display_str); // 实际电压数值
    show_index++;
    sprintf((char *)display_str, "freq:%d  ;", base_freq[1][0]);  // 1024/2
    LCD_DisplayString(10, show_index * 12 + 30, 12, display_str); // 实际电压数值

    if (fft_value(peaks[0]) < 30) // 没有输入
       base_freq[0][0]=0;
    if (fft_value(peaks[1]) < 30) // 没有输入
       base_freq[1][0]=0;


    set_freq = base_freq[0][0] - 0; // 设置频率
    feed_back_freq[0] = fft_index2freq(base_freq[0][0]);
    f_value_sum_min = 1000000; // 保存谐波变化差值之和 最小值


    AD9833_WaveSeting(set_freq, 0, base_freq[0][1], 0, 1); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
    set_freq = base_freq[1][0] - 0;                        // 设置频率
    AD9833_WaveSeting(set_freq, 0, base_freq[1][1], 0, 2); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
    freq_change_count = 3;

}

int error[2];
float set_freqs[2] = {0, 1};



int Perror[2];
float nowPhaseis[2];


int phase_count = 0;
int peak_count = 0;
int min_nowpeak;
int goal_phase = 2000;
int min_peak2peak = 10000;
int move_step = 15;
int set_peak1 = 2000;
int set_peak2 = 2000;
int lowest_peak1 = 2000;
int lowest_peak2 = 2000;
int lowphase_add = 100;
int lower_count = 0;
int phase_step = 1;
int stop_move = 1;

int deta_peak2[2] = {0};
float peak2peak_last1 = 0;
float peak2peak_last2 = 0;
void DDS_out(void)
{
    char display_str[50];

    // // 相位控制
    if (freq_change_count >= 2)
    {
        phase_count++;
        if (phase_count >= 4)
            phase_count = 0;
        float nowpeak1 = ADC_peak2peak(ADC1_Value);
        float nowpeak2 = ADC_peak2peak(ADC2_Value);

        int deta = Follow_PID(&phase_pid1, set_peak1 - nowpeak1 + fft_show_idx, 1);
        phase_1 = phase_1 - deta;
        deta = Follow_PID(&phase_pid2, set_peak2 - nowpeak2 + fft_show_idx, 1);
        phase_2 = phase_2 + deta;


        while (phase_1 >= 4095)
            phase_1 -= 4095;
        while (phase_1 < 0)
            phase_1 += 4095;
        while (phase_2 >= 4095)
            phase_2 -= 4095;
        while (phase_2 < 0)
            phase_2 += 4095;
        SetAD9833PhaseRegister(phase_1, 1); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
        SetAD9833PhaseRegister(phase_2, 2); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
    }

    // 屏幕一开就会有大杂波
}
int phase3 = 0;
float phase3_cor = 50 * 4096 / 360 + 10;

PID phase_pid3 = {
    .kp = 0.8, // 0.8
    .ki = 0,  // 0.00001
    .kd = 0,   // 0.001
    .error_acc = 0,
    .lastError = 0

};
int step_count = 0;
float pass_times[10];
float pass_time_n;
int pass_time_index = 0;
void double_dds_out()
{
    // TIM2_Init2(9, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
    // Fs = 1400000;
    float32_t average1 = 0;
    float32_t average2 = 0;
    for (int k = 0; k < FFT_LENGTH; k++) {
        average1 += ADC1_Value[k];
        average2 += ADC2_Value[k];
    }
    average2 /= FFT_LENGTH;
    average1 /= FFT_LENGTH;

    int first_p[2] = {0};
    float t2, t1;
    for (int j = 0; j < 3; j++)
    {
        int i = 10;
        float32_t average;
        uint16_t *adc_value;
        if (j == 0 || j == 2)
        {
            adc_value = ADC2_Value;
            average=average2;
        }
        else
        {
            adc_value = ADC1_Value;
            i = first_p[0] - 1; // 后一相位必定只能在0-360
            average=average1;
        }


        int E = 10;
        if (j == 2)
        {   // 往前找
            i = first_p[1] + 2;
            // 第一个i点，如果波形第一个点就大于平均值，那么说明前面有点波形找不到了。
            if (adc_value[i] < average)
            {
                // 找到第一个小于平均值的元素的索引
                for (; i >= 0; i--)
                    if (adc_value[i] > average - E)
                        break;
            }
            // 找到第一个大于平均值的元素的索引
            for (; i >= 0; i--)
                if (adc_value[i] < average + E)
                    break;
            first_p[0] = i;
        }
        else
        {
            // 第一个i点，如果波形第一个点就大于平均值，那么说明前面有点波形找不到了。
            if (adc_value[i] > average)
            {
                // 找到第一个小于平均值的元素的索引
                for (; i < FFT_LENGTH; i++)
                    if (adc_value[i] < average - E)
                        break;
            }
            // 找到第一个大于平均值的元素的索引
            for (; i < FFT_LENGTH; i++)
                if (adc_value[i] > average + E)
                    break;
            first_p[j] = i;
        }

        if (j == 0 || j == 2)

            t1 = (first_p[0] - 1) + (average - adc_value[first_p[0] - 1]) / (adc_value[first_p[0]] - adc_value[first_p[0] - 1]);
        //t1 = first_p[0];
        else
            t2 = (first_p[1] - 1) + (average - adc_value[first_p[1] - 1]) / (adc_value[first_p[1]] - adc_value[first_p[1] - 1]);
        //t2 = first_p[1];
    }
    float pass_time = (t2 - t1) / (Fs / base_freq[1][0]) * 4096; // 过零点

    if (pass_time >= 4095)
        pass_time -= 4095;
    if (pass_time < 0)
        pass_time += 4095;

    //     pass_time /=4;
    // pass_time_n = 4095 -pass_time;
    // phase3_cor = fft_show_idx * 4096 / 360;
    // phase3_cor = phase_set * 4096 / 360 + Debug_phase[(int)(base_freq[0][0] / 5e3)][(int)(base_freq[1][0] / 5e3)] * 4096 / 360;
    phase3_cor = phase_set * 4096 / 360 ;
    // phase3_cor +=20 * 4096 / 360;
    //int deta = Follow_PID(&phase_pid3, pass_time - phase3_cor, 1);
    // phase3 -= deta;
    int deta = 8;
    char display_str[50];
    // base_freq[1][0]=90e3;
    //    sprintf((char *)display_str, "pass_time:%.2f   ;", pass_time); // 1024/2
    //    LCD_DisplayString(10, 42, 12, display_str);                  // 实际电压数值
    // step_count++;
    // if (step_count>10){
    //     step_count = 0;
    if(fabs(pass_time - (phase3_cor))>200&&fabs(pass_time - (phase3_cor))){
        phase3 -= deta;
    }
    if (pass_time > phase3_cor && fabs(pass_time - (phase3_cor)) > 2047)
    {
        phase3 += deta;
    }
    else if (pass_time < phase3_cor && fabs(pass_time - (phase3_cor)) < 2047)
    {
        phase3 += deta;
    }
    else if (pass_time < phase3_cor && fabs(pass_time - (phase3_cor)) > 2047)
    {
        phase3 -= deta;
    }

    // if (pass_time >= phase3_cor-10){
    //     phase3 -= 10;
    // }
    // if (pass_time < phase3_cor-10){
    //     phase3 += 10;
    // }
    // }

    while (phase3 >= 4095)
        phase3 -= 4075;
    while (phase3 < 0)
        phase3 += 4075;
    SetAD9833PhaseRegister(phase3, 2);
    // }
//    char display_str[30];
//		sprintf((char *)display_str, "%.2f,%.2f,%d\n", pass_time, phase3_cor, phase3); // 1024/2
    // USART_OUT(USART6, (uint8_t *)display_str);
    // pass_time/=11;


    // char display_str[30];
    // sprintf((char *)display_str, "%.2f,%.2f,%d\n", pass_time, phase3_cor, phase3); // 1024/2
    // USART_OUT(USART6, (uint8_t *)display_str);
    // phase3=-phase3;

    //char display_str[50];
//        sprintf((char *)display_str, "Point_toP:%.2f   ;", t2 - t1); // 1024/2
//        LCD_DisplayString(10, 50, 12, display_str);                  // 实际电压数值
    // if(fabs(fabs(t2-t1)-phase3)>1){
    // drawWaveform(50, ADC2_Value, 0, lcd_height / 2 - 60, 60, 4096, RED);
    // drawWaveform(50, ADC1_Value, 0, lcd_height / 2 - 60, lcd_height / 2, 4096, RED);
    // char display_str[50];
    // sprintf((char *)display_str, "Point_toP:%.2f   ;", t2 - t1); // 1024/2
    // LCD_DisplayString(10, 50, 12, display_str);                  // 实际电压数值
    // sprintf((char *)display_str, "t1:%.2f   ;", t1);             // 1024/2
    // LCD_DisplayString(10, 72, 12, display_str);                  // 实际电压数值
    // }
    // phase3=fabs(t2-t1);
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
    LCD_CheckButton(&Debug);
    LCD_CheckButton(&btnPhase);

    //

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

            //char display_str[50];
            // sprintf((char *)display_str, "phase:%d+%d;", phase_set, Debug_phase[(int)(base_freq[0][0] / 5e3)][(int)(base_freq[1][0] / 5e3)]); // 1024/2
            // LCD_DisplayString(10, 174, 24, display_str);
            if(is_phase==0)//方便一键启动
                double_dds_out();
            // double_dds_out2();

            //  SetAD9833PhaseRegister(550, 2);

            // 开启AD转换器
						delay_ms(10);//这里等待，可以不让单片机工作，优化波形。
        }
        else
        {
            // 开启AD转换器
            ADC_Cmd(ADC3, DISABLE);
            // drawWaveform(adc_show_size, ADC3_Value, 0, lcd_height / 2 - 30, 30, 4096, RED);

            if (freq_change_count < 2)
            {
                // display_fft(2);
                FFT(ADC2_Value); // 计算fft
                FFT(ADC1_Value); // 计算fft
            }
            if (base_freq[0][0] != 0)
                DDS_out();
        }

        // 为了fft指示线快速移动
        for (int i = 0; i < 2; i++)
        {
            if (btn2[i].state == 1)
            {
                Page1_handle(btn2[i].id);
            }
        }
        // if (Debug_Mode)
        // {
        //     Debug_phase[(int)(base_freq[0][0] / 5e3)][(int)(base_freq[1][0] / 5e3)] = fft_show_idx;
        //     char display_str[50];
        //     sprintf((char *)display_str, "Debug_Mode"); // 1024/2
        //     LCD_DisplayString(10, 150, 24, display_str);
        //     sprintf((char *)display_str, "phase:%d,%d==%d   ;", phase_set, (int)(base_freq[1][0] / 5e3), Debug_phase[(int)(base_freq[0][0] / 5e3)][(int)(base_freq[1][0] / 5e3)]); // 1024/2
        //     LCD_DisplayString(10, 174, 24, display_str);
        // }

        if (is_phase)
        {
            phase_set = fft_show_idx;

        }
        
    }
}

void Page1_handle(int id)
{
    if (id == 1)
    {
        LCD_Fill_onecolor(0, 174, lcd_width, 280, 0x9f31);
        fft_show_idx -= 1;
        char display_str[50];
        if (is_show_data==1)
        {
            char display_str[50];
            sprintf((char *)display_str, "phase:%d+%d;", phase_set, Debug_phase[(int)(base_freq[0][0] / 5e3)][(int)(base_freq[1][0] / 5e3)]); // 1024/2
            LCD_DisplayString(10, 174, 24, display_str);
        }
    }
    else if (id == 2)
    {
        LCD_Fill_onecolor(0, 174, lcd_width, 280, 0x9f31);
        fft_show_idx += 1;
        char display_str[50];
        if (is_show_data==1)
        {
            char display_str[50];
            sprintf((char *)display_str, "phase:%d+%d;", phase_set, Debug_phase[(int)(base_freq[0][0] / 5e3)][(int)(base_freq[1][0] / 5e3)]); // 1024/2
            LCD_DisplayString(10, 174, 24, display_str);
        }

        // sprintf((char *)display_str, "phase:%d,fft_show_idx:%d   ;", fft_show_idx * 4096 / 360 , fft_show_idx); // 1024/2
        // LCD_DisplayString(10, 100, 12, display_str);                                               // 实际电压数值
        // 实际电压数值
        // base_freq[1][0]+=0.1;
        //  AD9833_WaveSeting( base_freq[1][0], 0, base_freq[1][1], 0,2); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
    }
    else if (id == 3)
    {
        LCD_Fill_onecolor(0, 60, lcd_width, 280, 0x9f31);
        is_show_data = 1 - is_show_data;

        char display_str[50];

        if (is_show_data)
        {
            ADC12_Init3();
            Fs =700000*4;
            TIM2_Init2(9, 2); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
            sprintf((char *)display_str, "phase:%d+%d;", phase_set, Debug_phase[(int)(base_freq[0][0] / 5e3)][(int)(base_freq[1][0] / 5e3)]); // 1024/2
            LCD_DisplayString(10, 174, 24, display_str);
        }
        else
        {
            ADC1_Init2(); // 高速信号采集dma、等
            ADC2_Init2();
            TIM2_Init2(19, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
            Fs = 700000;
            sprintf((char *)display_str, "                        "); // 1024/2
            LCD_DisplayString(10, 174, 24, display_str);
        }
    }
    else if (id == 4)
    {

        ADC_Cmd(ADC3, ENABLE);
        // 高速时需要开关tim2实现采集，不卡顿
        if (TIM2->CR1 == 0)        // 等待被关闭  说明FTT计算完毕
            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕

        display_fft(3);
        Debug_Mode=0;
        if(Debug_Mode==0)
            display_fft_datas2();
        else
            display_fft_datas();
        //Page1_handle(3);
                    char display_str[50];
            sprintf((char *)display_str, "fft_way_1"); // 1024/2
            LCD_DisplayString(10, 150, 24, display_str);

    }
    else if (id == 5)
    {
        Debug_Mode = 1;
        if (is_phase == 1 && Debug_Mode == 1)
            is_phase = 0;
        LCD_Fill_onecolor(0, 60, lcd_width, 280, 0x9f31);

        ADC_Cmd(ADC3, ENABLE);
        // 高速时需要开关tim2实现采集，不卡顿
        if (TIM2->CR1 == 0)        // 等待被关闭  说明FTT计算完毕
            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕

        display_fft(3);
        if(Debug_Mode==0)
            display_fft_datas2();
        else
            display_fft_datas();
                if(Debug_Mode==0){

            char display_str[50];
            sprintf((char *)display_str, "fft_way_1"); // 1024/2
            LCD_DisplayString(10, 150, 24, display_str);
        }else{
            char display_str[50];
            sprintf((char *)display_str, "fft_way_2"); // 1024/2
            LCD_DisplayString(10, 150, 24, display_str);
        }
    }
    else if (id == 6)
    {
        is_phase = 1 - is_phase;
        if (is_phase == 1 && Debug_Mode == 1)
            Debug_Mode = 0;
        if(is_phase==0) {
            char display_str[50];
            sprintf((char *)display_str, "                 "); // 1024/2
            LCD_DisplayString(10, 162, 12, display_str);
        } else {
            char display_str[50];
            sprintf((char *)display_str, "phase_change_mode"); // 1024/2
            LCD_DisplayString(10, 162, 12, display_str);
        }
    }
}


