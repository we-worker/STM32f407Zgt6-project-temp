#include "Wave_page.h"
#include "fft.h"
#include "adc.h"
#include "timer.h"
#include "stdbool.h"
#include "timer4.h"
#include "filter.h"

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
    TIM2_Init2(19, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
    Fs = 700000;
}

void auto_change(void)
{
    char display_str[30];
    //======================自动缩放波形======================
    int max_indx = fft_max_index();
    max_freq = (fft_freq(max_indx, 1) + max_freq) / 2;
    adc_show_size = Fs / max_freq * 5;

    adc_show_size = _constrain(adc_show_size, 5, 1024);
    //======================自动缩放FFT_FS======================

    if (Frequency > 120e3) // 高频使用等效采用
    {
        uint32_t test_freq = ((uint32_t)(1.0f * Frequency / 1e3 + 0.5)) * 1e3; // 这里没有完成，需要知道信号的准确频率才行
        int fenpin = Frequency / 10e5 + 1;                                     // 定时器频率与信号频率的倍数。
        uint32_t Fs_tim = test_freq / fenpin;
        // int delay=0;//自动控制延迟时间
        int delay = 250e3 / Frequency + 1;
        // double Fs_tim_true = 14e6 / ((14e6 / Fs_tim) + delay);
        // Fs = (uint32_t)(1.0f * test_freq / (test_freq - fenpin * Fs_tim_true) * Fs_tim_true + 0.5);
        Fs = 1.0f * Frequency / max_freq * Fs; // 也不根据采集的波形计算了，直接让fft解算的最大频率值等于频率计的。

        TIM2_Init2((u16)(42e6 / Fs_tim) - 1 + delay, 1);                                 // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
        sprintf((char *)display_str, "Tim2:%d    ", ((u16)(14e6 / Fs_tim) - 1 + delay)); // 1024/2
        LCD_DisplayString(70, 268, 12, display_str);                                     // 实际电压数值
    }
    else
    {
        // 需要10k下100hz分辨率时 Fs_temp = (int)(max_freq / 1000 / 2 + 1) * 1 * 7e3;
        uint32_t Fs_temp = (int)(max_freq / 1000 / 2 + 1) * 2 * 7e3;
        if (fabs(Fs_temp / (2 * 7e3) - Fs / (2 * 7e3)) >= 3) // 防止来回疯狂变化。
        {
            Fs = _constrain(Fs_temp, 1e3, 7e5);
            // Fs=30000;
            TIM2_Init2((u16)(14e6 / Fs) - 1, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
        }
        sprintf((char *)display_str, "Tim2:%d    ", (u16)(14e6 / Fs) - 1); // 1024/2
        LCD_DisplayString(70, 268, 12, display_str);                       // 实际电压数值
    }
}

void display_fft()
{

    FFT(ADC_Value); // 计算fft

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

    auto_change(); // 自动切换量程和自动调整Fs，等效采样。

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
                         // LCD_Fill_onecolor(0, 30, lcd_width,lcd_height/2, 0x9f31);
    int show_index = 0;
    for (int i = 0; i < peaks_num; i++)
    {
        float out = fft_value(peaks[i]);
        if (out <= 30 || out > 100e4)
            continue;
        sprintf((char *)display_str, "value:%.4f", out);           // 1024/2
        LCD_DisplayString(120, show_index * 12 + 30, 12, display_str); // 实际电压数值
        float freq = fft_freq(peaks[i], 0);
        sprintf((char *)display_str, "freq:%.4f", freq);              // 1024/2
        LCD_DisplayString(10, show_index * 12 + 30, 12, display_str); // 实际电压数值
        show_index++;
    }
    LCD_Fill_onecolor(0, show_index * 12 + 30, lcd_width, lcd_height / 4, 0x9f31);
    BACK_COLOR = WHITE; // 背景色
}

void ADC_progress()
{

    // 计算峰值、峰峰值和平均值
    peak_to_peak =ADC_peak2peak(ADC_Value);
    average = ADC_average(ADC_Value);
    frequency=ADC_freq(ADC_Value);
    char display_str[30];
    BACK_COLOR = 0x9f31;                                                    // 背景色
    sprintf((char *)display_str, "adc_freq:%.0f", frequency);               // 1024/2
    LCD_DisplayString(120, 50, 12, display_str);                            // 实际电压数值
    average = average / 4096 * 3.3 * 0.983261462083069 - 0.007369084852037; // ADC拟合结果
    sprintf((char *)display_str, "average:%.4f", average);                  // 1024/2
    LCD_DisplayString(120, 62, 12, display_str);                            // 实际电压数值
    sprintf((char *)display_str, "peak_to_peak:%.4f", peak_to_peak);        // 1024/2
    LCD_DisplayString(120, 74, 12, display_str);                            // 实际电压数值
    BACK_COLOR = WHITE;                                                     // 背景色
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

// rcPara_t di = {0, 0};

void Screen_page1_flash(void)
{

    Screen_flash_cnt++;
    // delay_ms(1);
    if (Screen_flash_cnt >= 100)
        Screen_flash_cnt = 0;

    for (char i = 0; i < 2; i++)
        LCD_CheckButton(&btn2[i]);
    LCD_CheckButton(&btn_data);
    LCD_CheckButton(&Freq_open);

    if (Screen_flash_cnt == 0)
    {

        // 高速时需要开关tim2实现采集，不卡顿
        if (TIM2->CR1 == 0)        // 等待被关闭  说明FTT计算完毕
            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕

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

        if (is_show_data)
        {
            display_fft_datas();
        }
        else
        {
            drawWaveform(adc_show_size, ADC_Value, 0, lcd_height / 2 - 30, 30, 4096, RED);
            ADC_progress();
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
        display_fft();
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
