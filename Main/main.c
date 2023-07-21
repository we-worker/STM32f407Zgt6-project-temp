#include "lcd.h"
#include "math.h"
//#include "SPI.h"
#include "key.h"
#include "adc.h"
#include "dac.h"
#include "touch.h"
#include "timer.h"
#include "Input_event.h"
//#include "Screen_main.h"
#include "timer4.h"
#include "fft.h"
#include "valuepack.h"
#include "AD9959.h"
#include "AD9959_Outset.h"
//#include "AD9833.h"
#include "05D_UI_progress.h"
//TxPack txpack;
//RxPack rxpack;
//		if(readValuePack(&rxpack))
//			{
//				// �����ǽ����յ�����ԭ���ش�
//				txpack.floats[0] = rxpack.floats[0];
//			}
//			// �ڴ˶����ݰ���ֵ�������ݷ��͵��ֻ�
//			sendValuePack(&txpack);

int main()
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����ϵͳ�ж����ȼ�����2
    delay_init();									// ��ʼ����ʱ����
    LCD_Init();										// ��Ļ��ʼ��

    Touch_Init(); // ��������ʼ��

//	SPI_init(); // spi��ʼ��
    initValuePack(115200);

    KEY_Init();	 // ������ʼ��
    DAC1_Init(); // DACͨ��1��ʼ��
//	Dac1_Set_Vol(2200);
    //DAC1_Init2();

    if (Fs == 700000)
    {
        ADC1_Init2();	  // �����źŲɼ�dma����
        ADC2_Init2();
        TIM2_Init2(9, 5); // ��ʱ��2ʱ��84M����Ƶϵ��84��84M/6=14000K ����9��Ϊ1400k
    } else if(Fs ==28000) {
        ADC1_Init2();	  // �����źŲɼ�dma����
        ADC2_Init2();
        TIM2_Init2(499, 5); // ��ʱ��2ʱ��84M����Ƶϵ��84��84M/6=14000K ����499��Ϊ28k
    }
    else
    {
        ADC1_Init();	   // adc��ʼ��
        TIM2_Init(49, 5); // ��ʱ��2ʱ��84M����Ƶϵ��84��84M/84=1000K ����9��Ϊ10.0us,100k
    }

    Screen_main2_init();

    //=========================DDS===========================
    GPIO_AD9959_Configuration();
    AD9959_Init();
    AD9959_enablechannel0(); 				//ʹ��ͨ��0
    AD9959_Setwavefrequency(40e6);
    AD9959_enablechannel1(); 				//ʹ��ͨ��1
    //AD9959_Setwaveamplitute(10e5,2500);
    AD9959_Setwavefrequency(10e6);
    AD9959_enablechannel2(); 				//ʹ��ͨ��2
    AD9959_Setwaveamplitute(40e6,300);
    AD9959_enablechannel3(); 				//ʹ��ͨ��3
    AD9959_SetAM2(5e6);

//	AD9833_Init();							 // ��ʼ����AD9833���ӵ�Ӳ���ӿ�
//	AD9833_WaveSeting(5000, 0, SIN_WAVE, 0); // 2KHz,	Ƶ�ʼĴ���0�����Ҳ���� ,����λ0
//	AD9833_AmpSet(100);						 // ���÷�ֵ����ֵ��� 255
    //=================================

    while (1)
    {

        input_event();
        Screen_main2();
        //AD9959_Setwavefrequency(4e5);



        //ADC2_Value[0]=Get_Adc(ADC_Channel_7);;

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


