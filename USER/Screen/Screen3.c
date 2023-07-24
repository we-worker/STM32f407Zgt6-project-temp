
#include "Screen3.h"

#include "timer4.h"

// Button btn9[9];

// Button btn_start; // 定义一个按钮结构体

void Screen_page3_init(void)
{

    LCD_Fill_onecolor(0, 30, lcd_width - 1, lcd_height - 1, 0xffff);


    freq_Measure_init();
}

void Screen_page3_flash(void)
{

    Screen_flash_cnt++;

    if (Screen_flash_cnt >= 100)
        Screen_flash_cnt = 0;

    //    for (char i = 0; i < 9; i++)
    //        LCD_CheckButton(&btn9[i]);
    //    LCD_CheckButton(&btn_start);

    if (Screen_flash_cnt == 0)
    {
        char display_str[30];
        if (Frequency > 10000)
        {
            sprintf((char *)display_str, "freq:%dhz      ",Frequency);
        }
        else
            sprintf((char *)display_str, "freq:%d      ", Frequency);
        LCD_DisplayString(10, 110, 24, display_str); // 实际电压数值
    }
}

void Page3_handle(int id)
{
}
