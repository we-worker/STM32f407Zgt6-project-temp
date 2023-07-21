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
//				// 这里是将接收的数据原样回传
//				txpack.floats[0] = rxpack.floats[0];
//			}
//			// 在此对数据包赋值并将数据发送到手机
//			sendValuePack(&txpack);

int main()
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组2
    delay_init();									// 初始化延时函数
    LCD_Init();										// 屏幕初始化

    Touch_Init(); // 触摸屏初始化

//	SPI_init(); // spi初始化
    initValuePack(115200);

    KEY_Init();	 // 按键初始化
    DAC1_Init(); // DAC通道1初始化
//	Dac1_Set_Vol(2200);
    //DAC1_Init2();

    if (Fs == 700000)
    {
        ADC1_Init2();	  // 高速信号采集dma、等
        ADC2_Init2();
        TIM2_Init2(9, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以9次为1400k
    } else if(Fs ==28000) {
        ADC1_Init2();	  // 高速信号采集dma、等
        ADC2_Init2();
        TIM2_Init2(499, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
    }
    else
    {
        ADC1_Init();	   // adc初始化
        TIM2_Init(49, 5); // 定时器2时钟84M，分频系数84，84M/84=1000K 所以9次为10.0us,100k
    }

    Screen_main2_init();

    //=========================DDS===========================
    GPIO_AD9959_Configuration();
    AD9959_Init();
    AD9959_enablechannel0(); 				//使能通道0
    AD9959_Setwavefrequency(40e6);
    AD9959_enablechannel1(); 				//使能通道1
    //AD9959_Setwaveamplitute(10e5,2500);
    AD9959_Setwavefrequency(10e6);
    AD9959_enablechannel2(); 				//使能通道2
    AD9959_Setwaveamplitute(40e6,300);
    AD9959_enablechannel3(); 				//使能通道3
    AD9959_SetAM2(5e6);

//	AD9833_Init();							 // 初始化与AD9833连接的硬件接口
//	AD9833_WaveSeting(5000, 0, SIN_WAVE, 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
//	AD9833_AmpSet(100);						 // 设置幅值，幅值最大 255
    //=================================

    while (1)
    {

        input_event();
        Screen_main2();
        //AD9959_Setwavefrequency(4e5);



        //ADC2_Value[0]=Get_Adc(ADC_Channel_7);;

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


