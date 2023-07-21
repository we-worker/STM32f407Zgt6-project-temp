#include "dac.h"
#include "math.h"

u16 dac1val = 0;
u8 dac1StopFlag=0;
float dac1_Vref=3.259;


#define PI 3.14159265f
//#define VREF 3.3 //参考电压
//#define N 36 //采样点数
//#define F 100000 //正弦波频率

////定义一个变量，用来存储36点的电压值
uint16_t dac1_sinval[128];
uint8_t dac_sin_index=0;


//DAC通道1输出初始化
void DAC1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);   //使能DAC通道1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
	
			for(int i =0 ;i < 128 ; i++)
	{
			dac1_sinval[i] = 512.0f*sin((1.0f/64) * PI*i) + 512; 
	}
	
}

/****************************************************************************
* 名    称: void Dac1_Set_Vol(u16 vol)
* 功    能：设置通道1输出电压
* 入口参数：vol:0~3300,代表0~3.3V
* 返回参数：无
* 说    明：       
****************************************************************************/
void Dac1_Set_Vol(u16 vol)
{
//	double temp=vol;
//	temp/=1000;
//	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);//12位右对齐数据格式设置DAC值
}






////=====================dac tim2触发 dma
////DAC通道1输出初始化
void DAC1_Init2(void)
{  
	
		for(int i =0 ;i < 128 ; i++)
	{
			dac1_sinval[i] = 512.0f*sin((1.0f/64) * PI*i) + 512; 
	}
	
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//使能DMA1时钟
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_T2_TRGO;	//使用TIM6触发 TEN1=1
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);   //使能DAC通道1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
	

	
	//配置DMA1_Stream5 channel7用于DAC1
	DMA_InitStructure.DMA_Channel = DMA_Channel_7; //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1; //外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&dac1_sinval; //内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //传输方向：内存到外设
	DMA_InitStructure.DMA_BufferSize = 128; //传输数据量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址不增加
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据大小：半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //内存数据大小：半字
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级：高
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //FIFO阈值：半满
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //内存突发传输：单次
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发传输：单次
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Stream5, ENABLE); //使能DMA1_Stream5
	DAC_DMACmd(DAC_Channel_1, ENABLE); //使能DAC通道1的DMA



}












































