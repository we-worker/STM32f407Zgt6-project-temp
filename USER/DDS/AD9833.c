#include "AD9833.h"


#define		FSYNC_1()     	GPIO_SetBits(GPIOG,GPIO_Pin_3)//输出高
#define		FSYNC_0()   	GPIO_ResetBits(GPIOG,GPIO_Pin_3)//输出低
#define     SCK_1()			GPIO_SetBits(GPIOG,GPIO_Pin_4)//输出高
#define 	SCK_0()			GPIO_ResetBits(GPIOG,GPIO_Pin_4)//输出低
#define 	DAT_1()			GPIO_SetBits(GPIOG,GPIO_Pin_5)//输出高
#define 	DAT_0()			GPIO_ResetBits(GPIOG,GPIO_Pin_5)//输出低

#define		FSYNC2_1()     	GPIO_SetBits(GPIOD,GPIO_Pin_3)//输出高
#define		FSYNC2_0()   	GPIO_ResetBits(GPIOD,GPIO_Pin_3)//输出低
#define     SCK2_1()			GPIO_SetBits(GPIOA,GPIO_Pin_8)//输出高
#define 	SCK2_0()			GPIO_ResetBits(GPIOA,GPIO_Pin_8)//输出低
#define 	DAT2_1()			GPIO_SetBits(GPIOE,GPIO_Pin_6)//输出高
#define 	DAT2_0()			GPIO_ResetBits(GPIOE,GPIO_Pin_6)//输出低


void AD9833_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4  |	GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3  ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
 

/*
*********************************************************************************************************
*	函 数 名: AD9833_Delay
*	功能说明: 时钟延时
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AD9833_Delay(void)
{
	unsigned int i;
	for (i = 0; i < 1; i++);
}



/*
*********************************************************************************************************
*	函 数 名: AD9833_Write
*	功能说明: 向SPI总线发送16个bit数据
*	形    参: TxData : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
void AD9833_Write(unsigned int TxData,uint8_t drive_id)
{
	unsigned char i;

	if (drive_id==1)
	{
		SCK_1();
		//AD9833_Delay();
		FSYNC_1();
		//AD9833_Delay();
		FSYNC_0();
		//AD9833_Delay();
		for(i = 0; i < 16; i++)
		{
			if(TxData&0x8000)
				DAT_1();
			else
				DAT_0();

			AD9833_Delay();
			SCK_0();
			AD9833_Delay();		
			SCK_1();
			
			TxData <<= 1;
		}
		FSYNC_1();
	}else if (drive_id==2)
	{
		SCK2_1();
		//AD9833_Delay();
		FSYNC2_1();
		//AD9833_Delay();
		FSYNC2_0();
		//AD9833_Delay();
		for(i = 0; i < 16; i++)
		{
			if(TxData&0x8000)
				DAT2_1();
			else
				DAT2_0();

			AD9833_Delay();
			SCK2_0();
			AD9833_Delay();		
			SCK2_1();
			
			TxData <<= 1;
		}
		FSYNC2_1();
	}
	
	

	
} 



/*
*********************************************************************************************************
*	函 数 名: AD9833_AmpSet
*	功能说明: 改变输出信号幅度值
*	形    参: 1.amp ：幅度值  0- 255
*	返 回 值: 无
*********************************************************************************************************
*/ 


void AD9833_AmpSet(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;
   	
//	CS_0();
	temp =0x1100|amp;
	for(i=0;i<16;i++)
	{
	    SCK_0();	
	   if(temp&0x8000)
	   	DAT_1();
	   else
		DAT_0();
		temp<<=1;
	    SCK_1();
	    AD9833_Delay();
	}
	
//   	CS_1();
}


int last_phase_switch[2]={0};
/* 设置相位寄存器的值 */
void SetAD9833PhaseRegister(unsigned int Phase,uint8_t drive_id)
{
	unsigned int Phs_data=0;
if(last_phase_switch[drive_id-1]==1){
	 Phs_data=Phase|0xC000;	//相位值
	last_phase_switch[drive_id-1]=0;
}else{

	 Phs_data=Phase|0xE000;	//相位值
	last_phase_switch[drive_id-1]=1;
}
	//AD9833_Write(0x2100);
//int Phs_data=Phase|0xC000;	//相位值
// Phase&=0x0FFF;
// Phase|=0xD000;
AD9833_Write(Phs_data,drive_id);	//设置相位
}

/*
*********************************************************************************************************
*	函 数 名: AD9833_WaveSeting
*	功能说明: 向SPI总线发送16个bit数据
*	形    参: 1.Freq: 频率值, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 或 1
			  3.WaveMode: TRI_WAVE(三角波),SIN_WAVE(正弦波),SQU_WAVE(方波)
			  4.Phase : 波形的初相位
*	返 回 值: 无
*********************************************************************************************************
*/ 
void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase,uint8_t drive_id )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************计算频率的16进制值***********************************/
		frequence_mid=268435456/25;//适合25M晶振//frequence_mid=268435456/25;//适合25M晶振
		//如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
		frequence_LSB=frequence_hex; //frequence_hex低16位送给frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位
		frequence_MSB=frequence_hex>>14; //frequence_hex高16位送给frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位

		Phs_data=Phase|0xC000;	//相位值
		AD9833_Write(0x0100,drive_id); //复位AD9833,即RESET位为1
		AD9833_Write(0x2100,drive_id); //选择数据一次写入，B28位和RESET位为1

		if(Freq_SFR==0)				  //把数据设置到设置频率寄存器0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //使用频率寄存器0输出波形
			AD9833_Write(frequence_LSB,drive_id); //L14，选择频率寄存器0的低14位数据输入
			AD9833_Write(frequence_MSB,drive_id); //H14 频率寄存器的高14位数据输入
			AD9833_Write(Phs_data,drive_id);	//设置相位
			//AD9833_Write(0x2000); /**设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形**/
	    }
		if(Freq_SFR==1)				//把数据设置到设置频率寄存器1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//使用频率寄存器1输出波形
			AD9833_Write(frequence_LSB,drive_id); //L14，选择频率寄存器1的低14位输入
			AD9833_Write(frequence_MSB,drive_id); //H14 频率寄存器1为
			AD9833_Write(Phs_data,drive_id);	//设置相位
			//AD9833_Write(0x2800); /**设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形**/
		}

		if(WaveMode==TRI_WAVE) //输出三角波波形
		 	AD9833_Write(0x2002,drive_id); 
		if(WaveMode==SQU_WAVE)	//输出方波波形
			AD9833_Write(0x2028,drive_id); 
		if(WaveMode==SIN_WAVE)	//输出正弦波形
			AD9833_Write(0x2000,drive_id); 

}

