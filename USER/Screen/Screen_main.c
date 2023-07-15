#include "Screen_main.h"
#include "fft.h"
#include "adc.h"
#include "math.h"
#include <stdio.h>
#include "input_event.h"
#include "uart.h"
#include "SPI.h"
#include "AD9833.h"

extern float adc_data1;
extern float adc_data2;

uint16_t Screen_flash_cnt = 0; // 屏幕刷新次数计数，实现简易延迟动画。

uint8_t SPI_data[7];





void drawpic(void);
void display_waveform(void);
void display_fft(void);
void Display_characteristic(void);

void Screen_main(void)
{

	Screen_flash_cnt++;
	if (Screen_flash_cnt >= 30)
		Screen_flash_cnt = 0;

	char display_str[30];
	BRUSH_COLOR = RED; // 显示颜色变为红色

	if (Screen_flash_cnt == 0)
	{

		// 高速时需要开关tim2实现采集，不卡顿
		//		if (!TIM2->CR1)			   // 等待被关闭  说明FTT计算完毕
		//			TIM_Cmd(TIM2, ENABLE); // 使能定时器2
	}
	if (is_show__wave==1)
	{
		if (Screen_flash_cnt == 0)
		{
			// 清空屏幕
			LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height - 1, 0xffff);
			display_waveform();

			// for (int i = 0; i < 7; i++)
			// {
			// 	SPI_data[i] = RecvFrom_FPGA(i, 4);
			// 	sprintf((char *)display_str, "data:%#x", SPI_data[i]); // 浮点型数据  e-01  就是除于10      /10
			// 	LCD_DisplayString(10, 200 + 12 * i, 12, display_str);  // 实际电压数值
			// }
		}
	}
	else if(is_show__wave==2)
	{
		display_fft();

	}else{
		if (Screen_flash_cnt == 0)
			Display_characteristic();
		//delay_ms(500);
	}
}

// 绘制波形的函数,Xpoint:x轴要绘制的点数,yValues:数据,isfloat,输入数据是否是浮点型，仅支持浮点型或u16
// lcdHeight绘制屏幕范围，y_offset:绘制高度偏移，valueRange：数据分布范围
void drawWaveform(u16 Xpoint, void *yValues, uint8_t isfloat, u16 lcdHeight, u16 y_offset, float valueRange, u16 color)
{
	u16 *yValuesU16 = (u16 *)yValues;
	float *yValuesFloat = (float *)yValues;
	// 绘制波形
	for (u16 i = 0; i < Xpoint; i++)
	{
		u16 x = i * (lcd_width * 1.0f / Xpoint);
		u16 y;
		if (isfloat)
			y = lcdHeight - (yValuesFloat[i] * valueRange);
		else
			y = lcdHeight - (yValuesU16[i] * valueRange);

		// 绘制当前ADC值的波形点
		LCD_Color_DrawPoint(x, y, color);
		// 绘制连接上一个ADC值的波形线段
		if (i > 0)
		{
			u16 prev_x = (i - 1) * (lcd_width * 1.0f / Xpoint);
			u16 prev_y;
			if (isfloat)
				prev_y = lcdHeight - (yValuesFloat[i - 1] * valueRange);
			else
				prev_y = lcdHeight - (yValuesU16[i - 1] * valueRange);
			LCD_DrawLine(prev_x, prev_y, x, y, color); // 使用指定颜色画线
		}
	}
}

void useless_main(void)
{
	char display_str[30];
	BRUSH_COLOR = RED; // 显示颜色变为红色
	LCD_DisplayString(10, 280, 24, "DEC");
	LCD_DisplayString(100, 280, 24, "ADD");
	LCD_DisplayString(180, 280, 24, "STOP");

	BRUSH_COLOR = BLUE;										  // 显示颜色变为红色
	sprintf((char *)display_str, "ADC1 VOL:%.4f", adc_data1); // 浮点型数据  e-01  就是除于10      /10
	LCD_DisplayString(10, 110, 24, display_str);			  // 实际电压数值

	sprintf((char *)display_str, "I :%.4f", adc_data2); // 浮点型数据  e-01  就是除于10      /10
	LCD_DisplayString(10, 170, 24, display_str);		// 实际电压数值
}

void display_waveform()
{
	// 清空屏幕
	LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height / 2 - 1, 0xffff);
	// 计算波形显示区域的高度
	u16 waveform_height = lcd_height / 2;
	// 计算每个ADC值在屏幕上的垂直位置范围
	float adc_value_range = waveform_height / 2000.0f; // 假设ADC的取值范围为0-4095
	int show_buffer_size = 512;						   // 绘制的范围，避免太多了。

	drawWaveform(show_buffer_size, ADC_Value, 0, lcd_height / 2, 0, adc_value_range, RED);
}

float fft_value(int n)
{
	return sqrtf(lBufOutArray[n] * lBufOutArray[n] + lBufOutArray[n - 1] * lBufOutArray[n - 1] + lBufOutArray[n - 2] * lBufOutArray[n - 2] + lBufOutArray[n + 1] * lBufOutArray[n + 1] + lBufOutArray[n + 2] * lBufOutArray[n + 2]);
}

void display_fft()
{
	// 清空屏幕
	LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height / 2 - 1, 0xffff);
	// 计算波形显示区域的高度
	u16 waveform_height = lcd_height / 2;

	// 计算每个ADC值在屏幕上的垂直位置范围
	float value_range = waveform_height / 2000.0f; // 假设ADC的取值范围为0-4095

	// 绘制波形
	drawWaveform(FFT_LENGTH, lBufOutArray, 1, lcd_height / 2, 0, value_range, RED);

	char display_str[30];

	LCD_DrawLine(fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH ), 0, fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH ), lcd_height / 2 - 1, BLUE); // 使用白色画线
	if (Screen_flash_cnt == 0)
	{
		BRUSH_COLOR = BLUE;																   // 显示颜色变为红色
		sprintf((char *)display_str, "freq:%.4f", (Fs * 1.0 / FFT_LENGTH) * fft_show_idx); // 浮点型数据  e-01  就是除于10      /10
		LCD_DisplayString(10, 220, 12, display_str);									   // 实际电压数值

		float out = fft_value(fft_show_idx);

		double uo_sum = 0;
		for (int i = -3; i < 3; i++)
		{
			uo_sum += lBufOutArray[fft_show_idx + i] * lBufOutArray[fft_show_idx + i];
		}
		sprintf((char *)display_str, "value:%.4f", sqrtf(uo_sum)); // 1024/2
		LCD_DisplayString(120, 220, 12, display_str);			   // 实际电压数值																			   // 实际电压数值
	}
}



// 绘制幅频特性曲线和相频特性曲线
void Display_characteristic()
{
const int show_len = 256;
int real_part[show_len] = {0};
int imag_part[show_len] = {0};
int key_index[show_len] = {0};
int show_index = 0;

    for (int i = 0; i < 300; i++)
    {
        uint16_t freq = (i + 1);
        // 设置频率
//				int phase=0;
//				if(freq>50)
//					phase=90;
        AD9833_WaveSeting(freq * 1e2, 0, SIN_WAVE, 0); // 2KHz, 频率寄存器0，正弦波输出，初相位0
        delay_ms(1);
        TIM_Cmd(TIM2, ENABLE); // 使能定时器2
				//delay_ms(5);
        while (TIM2->CR1!=0)
        {
        } // 等待被关闭，说明FFT计算完毕

        // 找到最大频率点
        int max_i = 1;
        for (int j = 1; j < FFT_LENGTH ; j++)
        {
            if (lBufOutArray[max_i] < lBufOutArray[j])
                max_i = j;
        }
				//max_i=freq * 1e2/(Fs * 1.0 / FFT_LENGTH);
        int a = -(lBufOutArray[max_i] * 512) / 1;
        int b = ((int)(lBufOutArray[max_i] * 512)) % 65536;
        float Phase = atan2(a, b) * 180 / PI; // 相位
				//float Phase =atan2(lBufInArray[2*max_i+1], lBufInArray[2*max_i]) * 180 / PI;
//				if(Phase>0)
//					continue;
				if (real_part[max_i / 2] == 0)
        {
            key_index[show_index] = max_i / 4;
            show_index++;
						real_part[max_i / 2] = lBufOutArray[max_i]; // 幅度
						imag_part[max_i / 2] = Phase;              // 相位
        }
				real_part[max_i / 2] =(real_part[max_i / 2] + lBufOutArray[max_i])/2.0f; // 幅度
        imag_part[max_i / 2] = (imag_part[max_i / 2]+Phase)/2.0f;              // 相位


    }

    // 清空屏幕
    LCD_Fill_onecolor(0, 0, lcd_width , lcd_height , 0xffff);
    
    u16 waveform_height = lcd_height / 2;// 计算波形显示区域的高度

    // 计算每个ADC值在屏幕上的垂直位置范围
    float adc_value_range1 = waveform_height / 1000.0f; // 假设ADC的取值范围为0-4095
		float adc_value_range2 = waveform_height / 500.0f; // 假设ADC的取值范围为0-4095
    int show_buffer_size = show_index; // 绘制的范围，避免太多了。

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
        u16 y1 = lcd_height / 2  - (amplitude * adc_value_range1);
		u16 y2 = lcd_height / 4*3 + (phase * adc_value_range2);
        // 绘制当前幅度值的波形点
        LCD_Color_DrawPoint(x, y1, RED);

        // 绘制连接上一个幅度值的波形线段
        if (i > 0)
        {
            u16 prev_x = (i - 1) * (lcd_width * 1.0f / show_buffer_size);
            u16 prev_y1 = lcd_height / 2  - (real_part[key_index[i - 1]] * adc_value_range1);
            LCD_DrawLine(prev_x, prev_y1, x, y1, RED); // 使用指定颜色画线

			u16 prev_y2 = lcd_height / 4*3 + (imag_part[key_index[i - 1]] * adc_value_range2);
			LCD_DrawLine(prev_x, prev_y2, x, y2, RED); // 使用指定颜色画线
        }
    }

}
