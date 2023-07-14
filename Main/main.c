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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����ϵͳ�ж����ȼ�����2
	delay_init();									// ��ʼ����ʱ����
	LCD_Init();										// ��Ļ��ʼ��

	Touch_Init(); // ��������ʼ��

	SPI_init(); // spi��ʼ��
	// USART6_Init();

	KEY_Init();	 // ������ʼ��
	DAC1_Init(); // DACͨ��1��ʼ��
	// DAC1_Init2();

	if (Fs == 700000)
	{
		ADC1_Init2();	  // �����źŲɼ�dma����
		TIM2_Init2(9, 5); // ��ʱ��2ʱ��84M����Ƶϵ��84��84M/6=14000K ����9��Ϊ1400k
	}
	else
	{
		ADC1_Init();	   // adc��ʼ��
		TIM2_Init(49, 83); // ��ʱ��2ʱ��84M����Ƶϵ��84��84M/84=1000K ����9��Ϊ10.0us,100k
	}

	// delay_ms(1000);

	//=========================DDS===========================
	//		GPIO_AD9959_Configuration();
	//		AD9959_Init();
	//		AD9959_enablechannel0(); 				//ʹ��ͨ��0
	//		AD9959_Setwavefrequency(4e5);
	//		AD9959_enablechannel1(); 				//ʹ��ͨ��1
	//		//AD9959_Setwaveamplitute(10e5,2500);
	//		AD9959_Setwavefrequency(10e6);
	//		AD9959_enablechannel2(); 				//ʹ��ͨ��2
	//		AD9959_Setwaveamplitute(40e6,300);
	//		AD9959_enablechannel3(); 				//ʹ��ͨ��3
	//		AD9959_SetAM2(5e6);

//	AD9833_Init();							 // ��ʼ����AD9833���ӵ�Ӳ���ӿ�
//	AD9833_WaveSeting(10e3, 0, SIN_WAVE, 0); // 2KHz,	Ƶ�ʼĴ���0�����Ҳ���� ,����λ0
//	AD9833_AmpSet(100);						 // ���÷�ֵ����ֵ��� 255
						//=================================

	while (1)
	{

		input_event();
		Screen_main();

		// USART_OUT(USART6, "�ַ����ǣ�%s","abcdefg");
		// while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET); // �ȴ��������
		/*
	data1 = RecvFrom_FPGA(0, 24);                              //��fpga0ͨ������16λ
	sprintf((char *)display_str, "data1:%ld         ", data1); //��sprintf��ʽ����ʾ������ʾ����
	LCD_DisplayString(30, 110, 24, display_str);               // lcd��Ļ��ʾ����

	// data2 = RecvFrom_FPGA(1, 16);                     //��fpga1ͨ������16λ
	// sprintf((char *)display_str, "data2:%6d", data2); //��sprintf��ʽ����ʾ������ʾ����
	// LCD_DisplayString(30, 160, 24, display_str);      // lcd��Ļ��ʾ����
		*/
	}
}

// Ƶ�ʲ���ģ��
void freq(void)
{
	TIM3_Init(9999, 8399); // ��ʱ��2ʱ��84M����Ƶϵ��8400��84M/8400=10K ���Լ���10000��Ϊ1000ms

	TIM4_Init2(0xffff, 3); // ��Ƶģʽ
	// EXTIX_Init();				//PD14�ⲿ�жϣ���Ƶ
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
	LCD_DisplayString(10, 110, 24, display_str); // ʵ�ʵ�ѹ��ֵ

	if (Frequency_avg > 10000)
	{
		sprintf((char *)display_str, "freq:%dkhz      ", Frequency_avg / 1000);
	}
	else
		sprintf((char *)display_str, "freq:%d      ", Frequency_avg);
}
