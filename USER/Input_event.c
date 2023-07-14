#include "Input_event.h"
#include "xpt2046.h"
#include "dac.h"
#include "stdio.h"

extern u16 dac1val;
extern u8 dac1StopFlag;
extern float dac1_Vref;

int is_show__wave=1;//显示波形或fft切换

//触摸屏控制器参数	
extern	u16 Xdown; 		 
extern	u16 Ydown;	   //触摸屏被按下就返回的的坐标值
extern u16  lcd_width;       //LCD的宽度
extern u16  lcd_height;      //LCD的高度

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

	if (keydown_data == KEY0_DATA)
	{
		if (dac1val < 4000)//限定最大输出电压
			dac1val += 10;
		DAC_SetChannel1Data(DAC_Align_12b_R, dac1val); //设置DAC值
		
		fft_show_idx+=1;
	}
	else if (keydown_data == KEY1_DATA)
	{
		if (dac1val > 200)
			dac1val -= 10;
		else
			dac1val = 0;
		DAC_SetChannel1Data(DAC_Align_12b_R, dac1val); //设置DAC值
		
		fft_show_idx-=1;
	}
	else if (keydown_data == KEY2_DATA)
	{
		dac1StopFlag = 1 - dac1StopFlag;
		is_show__wave=1-is_show__wave;
	}

	if (keydown_data == KEY1_DATA || keydown_data == KEY0_DATA) // KEY1按下了
	{
		dac_data = DAC_GetDataOutputValue(DAC_Channel_1); //读取前面设置DAC的值
		dac1_out = (float)dac_data * (dac1_Vref / 4096);			//得到DAC电压值
		sprintf((char *)display_str, "DAC VOL:%.4f", dac1_out); //浮点型数据  e-01  就是除于10      /10  //%06e
		LCD_DisplayString(10, 50, 24, display_str);			//实际电压数值
	}

	if (dac1StopFlag)
	{
		BRUSH_COLOR = RED;
		sprintf((char *)display_str, "DAC STOP");		//浮点型数据  e-01  就是除于10      /10  //%06e
		LCD_DisplayString(10, 30, 16, display_str); //实际电压数值
		BRUSH_COLOR = BLUE;							//设置字体为蓝色

		DAC_Cmd(DAC_Channel_1, DISABLE);
	}
	else
	{
		sprintf((char *)display_str, "    ");		//浮点型数据  e-01  就是除于10      /10  //%06e
		LCD_DisplayString(10, 30, 16, display_str); //实际电压数值
		BRUSH_COLOR = BLUE;							//设置字体为蓝色

		DAC_Cmd(DAC_Channel_1, ENABLE);
	}
}
