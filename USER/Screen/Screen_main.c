#include "Screen_main.h"
#include "fft.h"
#include "adc.h"
#include "math.h"
#include <stdio.h>
#include "input_event.h"
#include "uart.h"
#include "SPI.h"
#include "AD9833.h"

// OutBufArray[m]=Num=a+b*i (i是虚部单位，a=Num/65536,b=Num%65536）。相位Pha=atan2(a, b)，注意结果是弧度制。

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

		//
		// printf("%d\n",adc1_datas[0]);
		// USART_OUT(USART6, "%d\n",adc1_datas[0]);//发送蓝牙数据。
		// Xuehui_app_send((uint8_t *)adc1_datas);
		// 高速时需要开关tim2实现采集，不卡顿
//		if (!TIM2->CR1)			   // 等待被关闭  说明FTT计算完毕
//			TIM_Cmd(TIM2, ENABLE); // 使能定时器2
	}
	if (is_show__wave)
	{
		if (Screen_flash_cnt == 0)
		{
			// 清空屏幕
			LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height - 1, 0xffff);
			display_waveform();

			for (int i = 0; i < 7; i++)
			{
				SPI_data[i] = RecvFrom_FPGA(i, 4);
				sprintf((char *)display_str, "data:%#x", SPI_data[i]); // 浮点型数据  e-01  就是除于10      /10
				LCD_DisplayString(10, 200 + 12 * i, 12, display_str);  // 实际电压数值
			}
		}
	}
	else
	{
		//display_fft();
		Display_characteristic();
		//delay_ms(500);

		// Num=a+b*i (i是虚部单位，a=Num/65536,b=Num%65536）。相位Pha=atan2(a, b)
		// 找到最大频率点
		//		int max_i=2;
		//		for(int16_t i = 2; i < FFT_LENGTH/2; i++)
		//		{
		//			if(lBufOutArray[max_i]<lBufOutArray[i])
		//				max_i=i;
		//		}
		//		int a=(lBufOutArray[max_i]*512)/1;
		//		int b=((int)(lBufOutArray[max_i]*512))%65536;
		//		float Phase=atan2(a,b)*180/PI;
		//		sprintf((char *)display_str, "Phase:%.4f",Phase); // 浮点型数据  e-01  就是除于10      /10
		//		LCD_DisplayString(120, 300, 12, display_str);									   // 实际电压数值
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
	float adc_value_range = waveform_height / 4555.0f; // 假设ADC的取值范围为0-4095

	int show_buffer_size = 20; // 绘制的范围，避免太多了。
	// 绘制波形
	for (u16 i = 0; i < show_buffer_size; i++)
	{
		u16 x = i * (lcd_width * 1.0f / show_buffer_size);
		u16 y = lcd_height / 2 - (adc1_datas[i] * adc_value_range);

		// 绘制当前ADC值的波形点
		// LCD_Fill_onecolor(x, y, x + 1, y + 1, 0xFFFF);  // 使用白色填充
		LCD_Color_DrawPoint(x, y, RED);
		// 绘制连接上一个ADC值的波形线段
		if (i > 0)
		{
			u16 prev_x = (i - 1) * (lcd_width * 1.0f / show_buffer_size);
			u16 prev_y = lcd_height / 2 - (adc1_datas[i - 1] * adc_value_range);

			LCD_DrawLine(prev_x, prev_y, x, y, RED); // 使用白色画线
		}
	}
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
	for (u16 i = 0; i < FFT_LENGTH / 2; i++)
	{
		u16 x = i * (lcd_width * 1.0 / FFT_LENGTH * 2);
		u16 y = lcd_height / 2 - (lBufOutArray[i] * value_range);

		// 绘制当前ADC值的波形点
		LCD_Color_DrawPoint(x, y, RED);
		// 绘制连接上一个ADC值的波形线段
		if (i > 0)
		{
			u16 prev_x = (i - 1) * (lcd_width * 1.0 / FFT_LENGTH * 2);
			u16 prev_y = lcd_height / 2 - (lBufOutArray[i - 1] * value_range);

			LCD_DrawLine(prev_x, prev_y, x, y, RED); // 使用白色画线
		}
	}

	char display_str[30];
	//		for (int i = 0; i < numHarmonics; i++) {
	//			sprintf((char *)display_str, "freq:%.4f", harmonics[i]); // 浮点型数据  e-01  就是除于10      /10
	//			LCD_DisplayString(10, 150+i*12, 12, display_str);		// 实际电压数值
	//    }
	LCD_DrawLine(fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH * 2), 0, fft_show_idx * (lcd_width * 1.0 / FFT_LENGTH * 2), lcd_height / 2 - 1, BLUE); // 使用白色画线
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
		LCD_DisplayString(120, 220, 12, display_str);			   // 实际电压数值

		// n次谐波显示。
		float uo[5] = {0};
		for (int i = 0; i < 5; i++)
		{
			sprintf((char *)display_str, "freq:%.2f", (Fs * 1.0 / FFT_LENGTH) * 51 * (i + 1)); // 浮点型数据  e-01  就是除于10      /10
			LCD_DisplayString(10, 250 + 12 * i, 12, display_str);							   // 实际电压数值
		}
		for (int i = 0; i < 5; i++)
		{
			uo[i] = fft_value(51 + 51 * i) * 1.3636f;						   // 输入电压
			sprintf((char *)display_str, "Uo:%.2fV", uo[i] * 2.79148f / 1000); // 代表Uo电压
			LCD_DisplayString(120, 250 + 12 * i, 12, display_str);			   // 实际电压数值
		}

		BRUSH_COLOR = RED; // 显示颜色变为红色

		int adc_max = 0;
		int adc_min = 4096;
		for (int i = 0; i < FFT_LENGTH; i++)
		{
			if (adc1_datas[i] > adc_max)
				adc_max = adc1_datas[i];
			if (adc1_datas[i] < adc_min)
				adc_min = adc1_datas[i];
		}

		sprintf((char *)display_str, "Vp-p:%.2fV", (adc_max - adc_min) * 0.00191235f); // 浮点型数据  e-01  就是除于10      /10
		LCD_DisplayString(120, 230, 12, display_str);								   // 实际电压数值

		sprintf((char *)display_str, "THD:%.2f%", sqrtf(uo[1] * uo[1] + uo[2] * uo[2] + uo[3] * uo[3] + uo[4] * uo[4]) / uo[0] * 100); // 浮点型数据  e-01  就是除于10      /10
		LCD_DisplayString(10, 230, 12, display_str);																				   // 实际电压数值
	}
}

//// 幅频特性曲线、相频特性曲线。
void Display_characteristic(void)
{
	#define show_len 256
	int real_part[show_len]={0};
	int imag_part[show_len]={0};

	//int32_t freqs[FFT_LENGTH];
	char key_index[show_len]={0};
	int16_t show_index = 0;

	for (int i = 0; i < 10; i++)
	{
		uint16_t freq = (i + 1) ;
		//AD9833_WaveSeting(freq*10e3, 0, SIN_WAVE, 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
		//delay_ms(10);
		TIM_Cmd(TIM2, ENABLE); // 使能定时器2
		while (!TIM2->CR1)
		{} // 等待被关闭  说明FTT计算完毕

		// 找到最大频率点
		int max_i = 1;
		for (int16_t i = 1; i < FFT_LENGTH / 2; i++)
		{
			if (lBufOutArray[max_i] < lBufOutArray[i])
				max_i = i;
		}

		// int center_freq_index=freq/(Fs * 1.0 / FFT_LENGTH);
		int a = (lBufOutArray[max_i] * 512) / 1;
		int b = ((int)(lBufOutArray[max_i] * 512)) % 65536;
		float Phase = atan2(a, b) * 180 / PI;	//相位

		real_part[max_i/4] = lBufOutArray[max_i];	//幅度
		imag_part[max_i/4] = Phase;	//相位
		// freqs[real_imag_index] = (Fs * 1.0 / FFT_LENGTH) * fft_show_idx;
	}
	for(int i=0;i<show_len;i++){
		if(real_part[i]!=0){
			key_index[show_index]=i;
			show_index++;
		}
	}

	// 清空屏幕
	LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height / 2 - 1, 0xffff);

	// 计算波形显示区域的高度
	u16 waveform_height = lcd_height / 4;

	// 计算每个ADC值在屏幕上的垂直位置范围
	float adc_value_range = waveform_height / 2000.0f; // 假设ADC的取值范围为0-4095

	int show_buffer_size = show_index; // 绘制的范围，避免太多了。
	// 绘制幅频波形
	for (u16 i = 0; i < show_buffer_size; i++)
	{
		u16 x = i * (lcd_width * 1.0f / show_buffer_size);
		u16 y = lcd_height / 4 - (real_part[key_index[i]] * adc_value_range);

		LCD_Color_DrawPoint(x, y, RED);
		// 绘制连接上一个ADC值的波形线段
		if (i > 0)
		{
			u16 prev_x = (i - 1) * (lcd_width * 1.0f / show_buffer_size);
			u16 prev_y = lcd_height / 4 - (-1 * real_part[key_index[i - 1]] * adc_value_range);

			LCD_DrawLine(prev_x, prev_y, x, y, RED); // 使用白色画线
		}
	}
	// 绘制相频波形
	for (u16 i = 0; i < show_buffer_size; i++)
	{
		u16 x = i * (lcd_width * 1.0f / show_buffer_size);
		u16 y = lcd_height / 2 - (imag_part[key_index[i]] * adc_value_range);

		LCD_Color_DrawPoint(x, y, RED);
		// 绘制连接上一个ADC值的波形线段
		if (i > 0)
		{
			u16 prev_x = (i - 1) * (lcd_width * 1.0f / show_buffer_size);
			u16 prev_y = lcd_height / 2 - (imag_part[key_index[i-1]] * adc_value_range);

			LCD_DrawLine(prev_x, prev_y, x, y, RED); // 使用白色画线
		}
	}
}
