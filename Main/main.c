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

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����ϵͳ�ж����ȼ�����2
  delay_init();                                   // ��ʼ����ʱ����
  LCD_Init();                                     // ��Ļ��ʼ��
  Touch_Init();                                   // ��������ʼ��

  //	SPI_init(); // spi��ʼ��
  // initValuePack(115200);

  KEY_Init(); // ������ʼ��

  //	Dac1_Set_Vol(2200);
  // DAC1_Init2();

  //=========================DDS===PF11-�����ж����ų�ͻ-Ӱ�첻��========================
  GPIO_AD9959_Configuration();
  AD9959_Init();
  AD9959_enablechannel0(); // ʹ��ͨ��0
  AD9959_Setwavefrequency(40e6);
  // AD9959_Setwaveamplitute(40e6,1);
  AD9959_enablechannel1(); // ʹ��ͨ��1
  // AD9959_Setwaveamplitute(10e5,2500);
  AD9959_Setwavefrequency(10e6);
  AD9959_enablechannel2(); // ʹ��ͨ��2
  AD9959_Setwaveamplitute(40e6, 300);
  AD9959_enablechannel3(); // ʹ��ͨ��3
  AD9959_SetAM2(5e6);

  //	AD9833_Init();							 // ��ʼ����AD9833���ӵ�Ӳ���ӿ�
  //	AD9833_WaveSeting(5000, 0, SIN_WAVE, 0); // 2KHz,	Ƶ�ʼĴ���0�����Ҳ���� ,����λ0
  //	AD9833_AmpSet(100);						 // ���÷�ֵ����ֵ��� 255
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
