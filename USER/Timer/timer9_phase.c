#include "timer9_phase.h"

void phase_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	// Enable the clock for TIM9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	/* GPIOA clock enable */
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 channel 2 pin (PA.01) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; // GPIO配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9); // GPIOA0 复用位定时器 4
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9); // GPIOA0 复用位定时器 4

	// Configure TIM9 to run at 1 MHz
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

	// Configure channel 1 to reset the counter on rising edge
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_PWMIConfig(TIM9, &TIM_ICInitStructure);

	// 	// Configure channel 2 to capture on rising edge and enable interrupt
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_PWMIConfig(TIM9, &TIM_ICInitStructure);

	TIM_SelectSlaveMode(TIM9, TIM_SlaveMode_Reset); // 复位模式-选中的触发输入（TRGI）的上升沿初始化计数器，并且产生一个更新线号
	TIM_SelectMasterSlaveMode(TIM9, TIM_MasterSlaveMode_Enable);//启动定时器的被动触发
	TIM_SelectInputTrigger(TIM9, TIM_TS_TI1FP1);

	// TIM_UpdateRequestConfig(TIM9, TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM9, TIM_IT_CC2, ENABLE);
	//TIM_ITConfig(TIM9, TIM_TS_TI1FP1, ENABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;	 // 定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; // 抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 使能计数器，tim外部时钟，上升沿.不滤波

	TIM_ClearFlag(TIM9, TIM_FLAG_Update); // 清除计数器中断标志位
	TIM_Cmd(TIM9, ENABLE);
}
 uint16_t p_ticks=0,p_h_ticks=0 ;
uint16_t time_phase;
// 定时器2中断服务函数
void TIM1_BRK_TIM9_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM9, TIM_IT_Update) == SET) // 溢出中断
	{

		TIM_ClearITPendingBit(TIM9, TIM_IT_Update); // 清除中断标志位
	}
	/* Edge detected interrupt */
	if (TIM_GetITStatus(TIM9, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC2);

		 p_ticks = TIM_GetCapture1(TIM9);
		 p_h_ticks = TIM_GetCapture2(TIM9);
		time_phase=p_h_ticks-p_ticks;//这个就是相位差
		//                update_device_state(1, p_ticks, p_h_ticks);
	}

}
