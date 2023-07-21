#include "common.h"
#include "AD9959.h"
#include "stm32f4xx.h" 

#define system_clk 500000000	 //AD9959工作频率设定为500MHz

#define uchar unsigned char
#define uint unsigned int




//子程序-----------------------------------------------------------

void GPIO_AD9959_Configuration(void)  //I/O端口配置
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD |RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);  
	


  //除A4为复位位，其余为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	// 推挽输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);
  //除A4为复位位，其余为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4  |	GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	// 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13  |	GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



void AD9959_Init(void)  //初始化
{
	CS_1;
	SCLK_0;
	IO_update_0;
	PS0_0;
	PS1_0;
	PS2_0;
	PS3_0;
	
	SDIO0_0;
	SDIO1_0;
	SDIO2_0;
	SDIO3_0;
	
 	Reset_0;
 	delay_ms(6);
 	Reset_1;
 	delay_ms(100);
 	Reset_0;
}

//设置频率转换字
void WrFrequencyTuningWorddata(double f,uchar *ChannelFrequencyTuningWorddata)
{
	unsigned char z;
	
	long int y;
	double x;					
	x=4294967296.0/system_clk;	//频率控制字计算
	f=f*x;                      
	y=(long int)f;				//强制转换类型
	
	z=(uchar)(y>>24);     //8位长整型数据强制转为8位字符型
	ChannelFrequencyTuningWorddata[0]=z;
	
	z=(uchar)(y>>16);
	ChannelFrequencyTuningWorddata[1]=z;
	
	z=(uchar)(y>>8);
	ChannelFrequencyTuningWorddata[2]=z;
	
	z=(uchar)(y>>0);
	ChannelFrequencyTuningWorddata[3]=z;
	
}

// 数据更新，通道使能位不需要IO更新
void IO_update(void)   
{
	IO_update_0;
	delay_ms(12);
	IO_update_1;
	delay_ms(24);
	IO_update_0;
}


//通信
void WriteToAD9959ViaSpi(uchar RegisterAddress, uchar NumberofRegisters, uchar *RegisterData,uchar temp) 
{
	uchar	ControlValue = 0;
	uchar		ValueToWrite = 0;
	uchar	RegisterIndex = 0;
	uchar	i = 0;

	//是ControlValue为一个8位数据
	ControlValue = RegisterAddress;

	SCLK_0;
	CS_0;	 //bring CS low
	
	//写控制字
	for(i=0; i<8; i++)
	{
		SCLK_0;
		if(0x80 == (ControlValue & 0x80))
		{
			SDIO0_1;	  //Send one to SDIO0pin
		}
		else
		{
			SDIO0_0;	  //Send zero to SDIO0pin
		}
		SCLK_1;
		ControlValue <<= 1;	//Rotate data
	}
	SCLK_0;
	
	//写数据
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			SCLK_0;
			if(0x80 == (ValueToWrite & 0x80))
			{
				SDIO0_1;	  //Send one to SDIO0pin
			}
			else
			{
				SDIO0_0;	  //Send zero to SDIO0pin
			}
			SCLK_1;
			ValueToWrite <<= 1;	//Rotate data
		}
		SCLK_0;		
	}	
	if(temp==1)
	  {
	  IO_update();
	  }	
  CS_1;	//bring CS high again
}

//设置相位转换字
void WrPhaseOffsetTuningWorddata(double f,uchar *ChannelPhaseOffsetTuningWorddata)
{
	unsigned char z;
	
	long int y;
	double x;					
	x=16384.0/360;	//相位控制字计算
	f=f*x;                      
	y=(long int)f;				//强制转换类型
	
	z=(uchar)(y>>8);     //8位长整型数据强制转为8位字符型
	ChannelPhaseOffsetTuningWorddata[0]=z;
	
	z=(uchar)(y>>0);
	ChannelPhaseOffsetTuningWorddata[1]=z;
	
// 	ChannelPhaseOffsetTuningWorddata[2]=0x00;//下次修改时加上这两句然后在修改数组的变化，扫相时出现不能扫360的情况可能与计算时数组分配
// 	ChannelPhaseOffsetTuningWorddata[3]=0x00;//错误导致的
	
}

void WrAmplitudeTuningWorddata(double f,uchar *ChannelAmplitudeTuningWorddata) // 幅度扫描时设置上升和下降的幅度
																																								//计算过程注意与计算频率区别
{
	unsigned char z;
	
	unsigned long int y;
	double x;					
	x=1024.0/1024;	//幅度控制字计算
	f=f*x;                      
	y=(unsigned long int)f;				//强制转换类型
	
	z=(uchar)(y>>2);     //8位长整型数据强制转为8位字符型
	ChannelAmplitudeTuningWorddata[0]= z ;
	z=(uchar)(y<<6);		// 取低2位
	ChannelAmplitudeTuningWorddata[1] = (z & 0xC0);
	ChannelAmplitudeTuningWorddata[2] = 0x00;
	ChannelAmplitudeTuningWorddata[3] = 0x00;

/* 
	z=(uchar)(y>>8);
	ChannelAmplitudeTuningWorddata[2]=z;
	
	z=(uchar)(y>>0);
	ChannelAmplitudeTuningWorddata[3]=z;
 */
}


void WrAmplitudeTuningWorddata1(double f,uchar *ChannelAmplitudeTuningWorddata,uchar *ASRAmplituteWordata) // 设置输出幅度的大小
																																								//计算过程注意与计算频率区别
{ 
	unsigned char z;
	
	unsigned long int y;
	double x;					
	x=1024.0/1024;	//幅度控制字计算
	f=f*x*2.1;                      
	y=(unsigned long int)f;				//强制转换类型
	
	ASRAmplituteWordata[0] = ChannelAmplitudeTuningWorddata[0] ;
// 	ASRAmplituteWordata[1] = ChannelAmplitudeTuningWorddata[1] ;
// 	ASRAmplituteWordata[2] = ChannelAmplitudeTuningWorddata[2] ;
	
	
	z=(uchar)(y>>8);     //8位长整型数据强制转为8位字符型
	ChannelAmplitudeTuningWorddata[1] = (ChannelAmplitudeTuningWorddata[1] & 0xfc );
	ASRAmplituteWordata[1] = (ChannelAmplitudeTuningWorddata[1] | z );
	z=(uchar)(y<<0);		
	ASRAmplituteWordata[2] = z;
	

}
