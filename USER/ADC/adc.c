#include "adc.h"
#include "stm32f4xx_dma.h" //DMA头文件
#include "fft.h"

uint16_t ADC_Value[ADC_BUFFER_SIZE];
uint16_t ADC2_Value[ADC_BUFFER_SIZE];
// 初始化ADC
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 使能ADC1时钟

	// GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_USART1);
	// 先初始化ADC1通道6 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		   // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	   // 不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   // 初始化

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); // 复位结束

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					  // 独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles; // 两个采样阶段之间的延迟10个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		  // DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					  // 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									  // 初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;						// 12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								// 非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							// 关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						// 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;									// 1个转换在规则序列中 也就是只转换规则序列1
	ADC_Init(ADC1, &ADC_InitStructure);											// ADC初始化

	ADC_Cmd(ADC1, ENABLE); // 开启AD转换器
}

/****************************************************************************
 * 名    称: u16 Get_Adc(u8 ch)
 * 功    能：获得ADC值
 * 入口参数：ch: 通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
 * 返回参数：12位ADC有效值
 * 说    明：
 ****************************************************************************/
u16 Get_Adc(u8 ch)
{
	// 设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_84Cycles); // ADC1,ADC通道,480个周期,提高采样时间可以提高精确度

	ADC_SoftwareStartConv(ADC1); // 使能指定的ADC1的软件转换启动功能

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		; // 等待转换结束

	return ADC_GetConversionValue(ADC1); // 返回最近一次ADC1规则组的转换结果
}

//====================init2配置了tim2定时器触发adc采集，dma数据搬运，速度更快

// 初始化DMA
void ADC1_DMA1_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // 使能DMA2时钟

	// DMA2_Stream0通道0配置
	DMA_DeInit(DMA2_Stream0); // 复位DMA2 Stream0
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
	{
	} // 等待DMA可配置

	DMA_InitStructure.DMA_Channel = DMA_Channel_0;								// 通道0
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;					// 外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_Value;					// 内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;						// 外设到内存模式
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;							// 数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// 内存增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据长度:16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// 内存数据长度:16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								// 循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							// 高优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			// 内存突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // 外设突发单次传输
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);							// 初始化DMA Stream

	// 使能DMA中断
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

	// 开启DMA传输
	DMA_Cmd(DMA2_Stream0, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;	  // DMA2_Stream0中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // 子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化NVIC寄存器
}
// 初始化ADC
void ADC1_Init2(void)
{

	ADC1_DMA1_Init(); // ADC DMA配置。

	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 使能ADC1时钟

	// 先初始化ADC1通道6和7 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PA6 PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		   // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	   // 不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   // 初始化

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					  // 双模式，同时采样
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles; // 两个采样阶段之间的延迟10个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		  // DMA模式1，两个ADC交替存放
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					  // 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									  // 初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // 12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		   // 非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	   // 关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;			   // 1个转换在规则序列中 也就是只转换规则序列1
	ADC_Init(ADC1, &ADC_InitStructure);					   // ADC初始化

	// 配置ADC1通道6，第一个转换，采样时间为480个周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_15Cycles);

	// 使能DMA请求
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE); // 使能ADC_DMA

	// 开启AD转换器
	ADC_Cmd(ADC1, ENABLE);
}
// 初始化ADC2--PC2
void ADC2_Init2(void)
{

	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // 使能DMA2时钟

	// DMA2_Stream0通道0配置
	DMA_DeInit(DMA2_Stream2); // 复位DMA2 Stream0
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE)
	{
	} // 等待DMA可配置

	DMA_InitStructure.DMA_Channel = DMA_Channel_1;								// 通道0
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC2->DR;					// 外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC2_Value;					// 内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;						// 外设到内存模式
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;							// 数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// 内存增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据长度:16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// 内存数据长度:16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								// 循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							// 高优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			// 内存突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // 外设突发单次传输
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);							// 初始化DMA Stream

	// 使能DMA中断
	//	DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);

	// 开启DMA传输
	DMA_Cmd(DMA2_Stream2, ENABLE);

	//	NVIC_InitTypeDef NVIC_InitStructure;
	//	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;	  // DMA2_Stream0中断
	//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级1
	//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // 子优先级1
	//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	//	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化NVIC寄存器

	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);  // 使能ADC1时钟

	// 先初始化ADC1通道6和7 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		 // PA6 PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不带上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);			 // 初始化

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					  // 双模式，同时采样
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles; // 两个采样阶段之间的延迟10个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		  // DMA模式1，两个ADC交替存放
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					  // 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									  // 初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // 12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		   // 非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	   // 关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;			   // 1个转换在规则序列中 也就是只转换规则序列1
	ADC_Init(ADC2, &ADC_InitStructure);					   // ADC初始化

	// 配置ADC1通道6，第一个转换，采样时间为480个周期
	ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 1, ADC_SampleTime_15Cycles);

	// 使能DMA请求
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
	ADC_DMACmd(ADC2, ENABLE); // 使能ADC_DMA

	// 开启AD转换器
	ADC_Cmd(ADC2, ENABLE);
}

// uint16_t dma_inter_count = 0;
// DMA中断服务函数
void DMA2_Stream0_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET) // 传输完成中断
	{
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);	   // 清除传输完成标志
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0); // 清除中断标志

		// 在这里添加你的处理代码
		//		dma_inter_count++;
		//		if(dma_inter_count==0)//故意慢一点执行。

		TIM_Cmd(TIM2, DISABLE); // 关闭定时器2
								// FFT(ADC_Value);
	}
}

//=========================ADC功能函数=============

// adc数组的平均值，包括线性拟合真实值。
float ADC_average(uint16_t *adc_value)
{
	float average = 0;
	int i;
	for (i = 0; i < FFT_LENGTH; i++)
		average += adc_value[i];
	average /= FFT_LENGTH;

	average = average / 4096 * 3.3 * 0.983261462083069 - 0.007369084852037; // ADC拟合结果
	return average;
}

// adc数组的峰峰值。
float ADC_peak2peak(uint16_t *adc_value)
{
	uint16_t max = adc_value[0];
	uint16_t min = adc_value[0];
	for (int i = 1; i < FFT_LENGTH; i++)
	{
		if (adc_value[i] > max)
			max = adc_value[i];
		if (adc_value[i] < min)
			min = adc_value[i];
	}
	// 计算峰值、峰峰值和平均值
	return max - min;
}

// adc数组计算频率.
float ADC_freq(uint16_t *adc_value)
{
	int i, j = 0, k = 0; // 索引
	double total_time;	 // 总时间
	float E = 30;		 // 中心点判断偏移量

	float average = ADC_average(adc_value);
	// 第一个i点，如果波形第一个点就大于平均值，那么说明前面有点波形找不到了。
	if (adc_value[0] > average + E)
	{
		// 找到第一个小于平均值的元素的索引
		for (i = 0; i < FFT_LENGTH; i++)
			if (adc_value[i] < average - E)
				break;
	}
	// 找到第一个大于平均值的元素的索引
	for (; i < FFT_LENGTH; i++)
		if (adc_value[i] > average + E)
			break;

	int start_i = i;
	// 初始化总时间为零
	total_time = 0;
	// m代码adc数组中的周期总数
	float m = 0;
	while (j < FFT_LENGTH && k < FFT_LENGTH)
	{
		// 找到第一个小于平均值的元素的索引
		for (j = i + 1; j < FFT_LENGTH; j++)
			if (adc_value[j] < average - E)
				break;
		if (j >= FFT_LENGTH)
			break;
		// 找到第二个大于平均值的元素的索引
		for (k = j + 1; k < FFT_LENGTH; k++)
			if (adc_value[k] > average + E)
				break;
		if (k >= FFT_LENGTH)
			break;

		// 更新i为k，继续下一个周期的测量
		i = k;
		m++;
	}
	if (j >= FFT_LENGTH)
	{
		float t2 = (k - 1) + (average - adc_value[k - 1]) / (adc_value[k] - adc_value[k - 1]);
		total_time = t2 - (start_i - 1) + (average - adc_value[start_i - 1]) / (adc_value[start_i] - adc_value[start_i - 1]);
	}
	else
	{
		float t1 = (j - 1) + (average - adc_value[j - 1]) / (adc_value[j] - adc_value[j - 1]);
		total_time = t1 - (start_i - 1) + (average - adc_value[start_i - 1]) / (adc_value[start_i] - adc_value[start_i - 1]);
		m += 0.5;
	}

	// 计算周期和频率
	float frequency = 0;
	if (m != 0)
		frequency = 1.0f * Fs / total_time * m;
	return frequency;
}