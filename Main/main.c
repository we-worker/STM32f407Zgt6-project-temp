#include "lcd.h"
#include "math.h"
#include "touch.h"
#include "key.h"
#include "Input_event.h"

#include "adc.h"
#include "dac.h"
#include "timer.h"
#include "timer9_phase.h"

// #include "Screen_main.h"
// #include "timer4.h"
#include "fft.h"
// #include "valuepack.h"
// #include "SPI.h"
#include "AD9959.h"
#include "AD9959_Outset.h"
// #include "AD9833.h"
#include "Screen_main.h"

int main()
{

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组2
  delay_init();                                   // 初始化延时函数
  LCD_Init();                                     // 屏幕初始化
  Touch_Init();                                   // 触摸屏初始化

  //	SPI_init(); // spi初始化
  // initValuePack(115200);

  KEY_Init(); // 按键初始化

  //	Dac1_Set_Vol(2200);
  // DAC1_Init2();

  //=========================DDS===PF11-触屏判断引脚冲突-影响不大========================
  GPIO_AD9959_Configuration();
  AD9959_Init();
  AD9959_enablechannel0(); // 使能通道0
  AD9959_Setwavefrequency(40e6);
  // AD9959_Setwaveamplitute(40e6,1);
  AD9959_enablechannel1(); // 使能通道1
  // AD9959_Setwaveamplitute(10e5,2500);
  AD9959_Setwavefrequency(10e6);
  AD9959_enablechannel2(); // 使能通道2
  AD9959_Setwaveamplitute(40e6, 300);
  AD9959_enablechannel3(); // 使能通道3
  AD9959_SetAM2(5e6);

  //	AD9833_Init();							 // 初始化与AD9833连接的硬件接口
  //	AD9833_WaveSeting(5000, 0, SIN_WAVE, 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0
  //	AD9833_AmpSet(100);						 // 设置幅值，幅值最大 255
  //=================================

  Screen_init();
 // phase_init();
  while (1)
  {     

    input_event();
    Screen_flash();
  }
}

//
