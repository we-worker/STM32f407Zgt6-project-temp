
#include "Screen3.h"

#include "timer4.h"
#include "timer.h"
#include "adc.h"
// Button btn9[9];

Button btn_stop; // 定义一个按钮结构体
int stop_mode=0;

void Screen_page3_init(void)
{

    LCD_Fill_onecolor(0, 30, lcd_width - 1, lcd_height - 1, 0xffff);

    btn_stop = button_init(125, 280, 50, 20, 0xfe67, "stop", 12, 0, Button_press_handle, 1); // 初始化一个按钮btn1
    LCD_DrawButton(&btn_stop);     
    // freq_Measure_init();
}

void Screen_page3_flash(void)
{

    Screen_flash_cnt++;

    if (Screen_flash_cnt >= 100)
        Screen_flash_cnt = 0;

    //    for (char i = 0; i < 9; i++)
    //        LCD_CheckButton(&btn9[i]);
       LCD_CheckButton(&btn_stop);

    if (Screen_flash_cnt == 0)
    {
        char display_str[30];


                // 高速时需要开关tim2实现采集，不卡顿
        if (TIM2->CR1 == 0)        // 等待被关闭  说明FTT计算完毕
            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        while (TIM2->CR1 != 0)
        {
        } // 等待被关闭，说明采样完毕
        // if (Frequency > 10000)
        // {
        //     sprintf((char *)display_str, "freq:%dhz      ",Frequency);
        // }
        // else
        //     sprintf((char *)display_str, "freq:%d      ", Frequency);
        // LCD_DisplayString(10, 110, 24, display_str); // 实际电压数值
        if(stop_mode){

        drawWaveform(30, ADC2_Value, 0, lcd_height / 2 - 60, 60, 4096, RED);
        drawWaveform(30, ADC1_Value, 0, lcd_height / 2 - 60, lcd_height / 2, 4096, RED);
        }
    }
}

void Page3_handle(int id)
{
    if (id==1)
    {
        stop_mode=1-stop_mode;
    }
    
}
