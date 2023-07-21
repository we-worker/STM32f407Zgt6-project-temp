#include "timer4.h"
#include "math.h"
#include "lcd.h"

uint32_t Frequency; //输入脉冲的频率HZ
uint32_t Frequency_temp; //输入脉冲的频率HZ
uint16_t PB0_Counter1 = 0;
uint16_t ClearPB0Count = 0; //用来计数，2s内不在有脉冲进入则清零 测速值
uint32_t flag_65536=0;
uint32_t speed_time=0;
uint32_t get_sign_count=0;

/***************************************************
//中断优先级配置
*****************************************************/
static void GENERAL_TIM4_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// 设置中断组为1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	// 设置主优先级为 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//100k-10M以上高频测量
void TIM4_Init2(u16 auto_data, u16 fractional)
{

	GENERAL_TIM4_NVIC_Config();

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
														 // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period = auto_data;
	// 时钟预分频数
	TIM_TimeBaseStructure.TIM_Prescaler = fractional;
	// 时钟分频因子 ，没用到不用管
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	// 初始化定时器
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//选择输入端 IC1 映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;			//配置输入分频,不分频
	TIM_ICInitStructure.TIM_ICFilter = 0;							// IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);

	// 使能计数器
	TIM_TIxExternalClockConfig(TIM4,TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising,0);
	TIM_SetCounter(TIM4,0);//计数器清零
	// 开启计数器中断
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM4, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//使能GPIOF时钟
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); // GPIOA0 复用位定时器 4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		   // KEY0 KEY1 KEY2 KEY3对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100M
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	   //下拉

	GPIO_Init(GPIOB, &GPIO_InitStructure);			   //初始化GPIOD14 15
}


void TIM3_Init(u16 auto_data, u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能TIM2时钟

	TIM_TimeBaseInitStructure.TIM_Period = auto_data;				//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = fractional;			//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //初始化TIM2

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM3, ENABLE);					   //使能定时器2

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



/***************************************************
//定时器中断
*****************************************************/
void TIM4_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		flag_65536++;
		if (flag_65536 > 10000) // 1s
		{
			flag_65536 = 0;
		}
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
			Frequency = 4*(TIM4->CNT+65536*flag_65536);
			TIM3->CNT = 0;
			flag_65536=0;
			TIM4->CNT=0;
		
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
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

//	sprintf((char *)display_str, "VPP:%.4f", adc_data1);
    LCD_DisplayString(10, 110, 24, display_str); // 实际电压数值

    if (Frequency_avg > 10000)
    {
        sprintf((char *)display_str, "freq:%dkhz      ", Frequency_avg / 1000);
    }
    else
        sprintf((char *)display_str, "freq:%d      ", Frequency_avg);
}
