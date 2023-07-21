#include "Wave_page.h"
#include "fft.h"
#include "adc.h"
#include "timer.h"


#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
Button btn2[2];

int adc_show_size = 1024; // 绘制的范围，实现自动缩放时间轴

float max_freq=0;
// 定义一个显示当前数字的标签
// Label label1 = {18, 40, 200, 50, BLACK, 0xE73F, BLACK, 24, NULL};

void Screen_page1_init(void)
{

    LCD_Fill_onecolor(0, 30, lcd_width - 1, lcd_height - 1, 0xffff);
    for (char i = 0; i < 2; i++)
    {                                         // 遍历数组
        btn2[i].x = i * (150) + 20;           // 计算每个按钮的x坐标
        btn2[i].y = 300;                      // 计算每个按钮的y坐标
        btn2[i].width = 50;                   // 设置每个按钮的宽度为40
        btn2[i].height = 20;                  // 设置每个按钮的高度为40
        btn2[i].color = 0xfe67;               // 设置每个按钮的颜色为蓝色
        btn2[i].size = 16;                    // 设置每个按钮的文字大小为16
        btn2[i].state = 0;                    // 设置每个按钮的初始状态为未按下
        btn2[i].id = i + 1;                   // 设置每个按钮的id为数字
        btn2[i].action = Button_press_handle; // 设置每个按钮的action为Button_press_handle
    }
    btn2[0].text = "<<"; // 把字符串数组的元素赋值给btn9
    btn2[1].text = ">>";
    LCD_DrawButton(&btn2[0]); // 画出每个按钮
    LCD_DrawButton(&btn2[1]); // 画出每个按钮

    // label1.text = malloc(20);             // 给text分配10个字节的空间
    // strcpy(label1.text, "Hello Darkarc"); // 初始化text
    // LCD_Draw_Label(&label1);              // 画出标签框

    ADC1_Init2(); // 高速信号采集dma、等
    ADC2_Init2();
    TIM2_Init2(19, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
    Fs = 700000;
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

    BRUSH_COLOR = BLUE;                                                // 显示颜色变为红色
    sprintf((char *)display_str, "freq:%.4f", fft_freq(fft_show_idx)); // 浮点型数据  e-01  就是除于10      /10
    LCD_DisplayString(10, 280, 12, display_str);                       // 实际电压数值

    float out = fft_value(fft_show_idx);
    sprintf((char *)display_str, "value:%.4f", out); // 1024/2
    LCD_DisplayString(120, 280, 12, display_str);    // 实际电压数值																			   // 实际电压数值
    
    int max_indx=fft_max_index();
    max_freq=fft_freq(max_indx);
    adc_show_size=Fs/max_freq*5;
		
		adc_show_size=_constrain(adc_show_size,5,1024);
    
    sprintf((char *)display_str, "freq:%.4f", max_freq); // 1024/2
    LCD_DisplayString(120, 268, 12, display_str);    // 实际电压数值				
}

void Screen_page1_flash(void)
{

    Screen_flash_cnt++;
    // delay_ms(1);
    if (Screen_flash_cnt >= 100)
        Screen_flash_cnt = 0;

    // send_rect_wave(input_num);

    for (char i = 0; i < 2; i++)
        LCD_CheckButton(&btn2[i]);
    // LCD_CheckButton(&btn_start);

    if (Screen_flash_cnt == 0)
    {
        // 高速时需要开关tim2实现采集，不卡顿
        if (TIM2->CR1 == 0)        // 等待被关闭  说明FTT计算完毕
            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕


        drawWaveform(adc_show_size, ADC_Value, 0, lcd_height / 2 - 30, 30, 4096, RED);

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
    // char str[10];
    // sprintf((char *)str, "%d", input_num); // 浮点型数据  e-01  就是除于10      /10
    // strcpy(label1.text, str);
    // LCD_Update_Label(&label1);
}
