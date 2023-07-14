#include "lcd.h"
#include "math.h"
#include "SPI.h"
#include "key.h"
#include "adc.h"
#include "dac.h"
#include "touch.h"
#include "timer.h"
#include "Input_event.h"
#include "Screen_main.h"
#include "timer4.h"
#include "fft.h"
#include "uart.h"
// #include "AD9959.h"
// #include "AD9959_Outset.h"
#include "AD9833.h"

int main()
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组2
	delay_init();									// 初始化延时函数
	LCD_Init();										// 屏幕初始化

	Touch_Init(); // 触摸屏初始化

	SPI_init(); // spi初始化
	// USART6_Init();

	KEY_Init();	 // 按键初始化
	DAC1_Init(); // DAC通道1初始化
	// DAC1_Init2();

	if (Fs == 700000)
	{
		ADC1_Init2();	  // 高速信号采集dma、等
		TIM2_Init2(9, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
	}
	else
	{
		ADC1_Init();	   // adc初始化
		TIM2_Init(49, 83); // 定时器2时钟84M，分频系数84，84M/84=1000K 所以9次为10.0us,100k
	}

	// delay_ms(1000);

	//=========================DDS===========================
	//		GPIO_AD9959_Configuration();
	//		AD9959_Init();
	//		AD9959_enablechannel0(); 				//使能通道0
	//		AD9959_Setwavefrequency(4e5);
	//		AD9959_enablechannel1(); 				//使能通道1
	//		//AD9959_Setwaveamplitute(10e5,2500);
	//		AD9959_Setwavefrequency(10e6);
	//		AD9959_enablechannel2(); 				//使能通道2
	//		AD9959_Setwaveamplitute(40e6,300);
	//		AD9959_enablechannel3(); 				//使能通道3
	//		AD9959_SetAM2(5e6);

//	AD9833_Init();							 // 初始化与AD9833连接的硬件接口
//	AD9833_WaveSeting(10e3, 0, SIN_WAVE, 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
//	AD9833_AmpSet(100);						 // 设置幅值，幅值最大 255
						//=================================

	while (1)
	{

		input_event();
		Screen_main();

		// USART_OUT(USART6, "字符串是：%s","abcdefg");
		// while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET); // 等待发送完成
		/*
	data1 = RecvFrom_FPGA(0, 24);                              //从fpga0通道接受16位
	sprintf((char *)display_str, "data1:%ld         ", data1); //用sprintf格式化显示，给显示数组
	LCD_DisplayString(30, 110, 24, display_str);               // lcd屏幕显示数据

	// data2 = RecvFrom_FPGA(1, 16);                     //从fpga1通道接受16位
	// sprintf((char *)display_str, "data2:%6d", data2); //用sprintf格式化显示，给显示数组
	// LCD_DisplayString(30, 160, 24, display_str);      // lcd屏幕显示数据
		*/
	}
}

// 频率测量模块
void freq(void)
{
	TIM3_Init(9999, 8399); // 定时器2时钟84M，分频系数8400，84M/8400=10K 所以计数10000次为1000ms

	TIM4_Init2(0xffff, 3); // 低频模式
	// EXTIX_Init();				//PD14外部中断，测频
	extern uint32_t Frequency;

	char display_str[30];
	uint32_t Frequency_avg = 0;
	for (int i = 0; i < 100; i++)
	{
		Frequency_avg += Frequency;
		// delay_ms(100);
	}
	Frequency_avg /= 100;

	sprintf((char *)display_str, "VPP:%.4f", adc_data1);
	LCD_DisplayString(10, 110, 24, display_str); // 实际电压数值

	if (Frequency_avg > 10000)
	{
		sprintf((char *)display_str, "freq:%dkhz      ", Frequency_avg / 1000);
	}
	else
		sprintf((char *)display_str, "freq:%d      ", Frequency_avg);
}
