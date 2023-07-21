#include "Input_event.h"
#include "xpt2046.h"
#include "dac.h"
#include "stdio.h"

int is_show__wave = 1; // 显示波形或fft切换

// 触摸屏控制器参数
extern u16 Xdown;
extern u16 Ydown;	   // 触摸屏被按下就返回的的坐标值
extern u16 lcd_width;  // LCD的宽度
extern u16 lcd_height; // LCD的高度


void Event_handle();

void input_event(void)
{
	char display_str[30];
	u16 dac_data;
	float dac1_out;

	key_scan(0);
	XPT2046_Scan(0);

	if (Xdown < lcd_width && Ydown > 280)
	{
		if (Xdown < 100)
			keydown_data = KEY1_DATA;
		else if (Xdown < 180)
			keydown_data = KEY0_DATA;
		else
			keydown_data = KEY2_DATA;
	}
	Event_handle();
}

void Event_handle()
{
	if (keydown_data == KEY0_DATA)
	{

		// fft_show_idx += 1;
	}
	else if (keydown_data == KEY1_DATA)
	{

		// fft_show_idx -= 1;
	}
	else if (keydown_data == KEY2_DATA)
	{
		is_show__wave = is_show__wave+1;
		if(is_show__wave>3)
			is_show__wave=1;
	}

	if (keydown_data == KEY1_DATA || keydown_data == KEY0_DATA) // KEY1按下了
	{

	}
}
