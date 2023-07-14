
#include "AD9833.h"
#include <stdio.h>


#define ad9833_sdata   PGout(5) 
#define ad9833_sclk    PGout(4)
#define ad9833_fsync   PGout(3)

void AD9833_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  

  //除A4为复位位，其余为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4  |	GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	// 推挽输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}

/*向AD9833写16位数据*/
void write_ad9833_d16(unsigned int x)
{
	int i,w;
	ad9833_sclk=1;
	ad9833_fsync=1;
	ad9833_fsync=0;
	w=x;                       //写16位数据
	for(i=0;i<16;i++) 
	{
		ad9833_sdata=w&0x8000;
		ad9833_sclk=0;
		ad9833_sclk=1;
		w=w<<1;
	}
		ad9833_fsync=1;
		ad9833_sclk=0;
	}


/************************************************************
AD9833波形输出

mode:   输出波形选择，s_wave    方波
										 t_wave    三角波
										 sin_wave  正弦波

Freq：  输出波形的频率
************************************************************/

void AD9833_WaveOut(unsigned int mode,unsigned long Freq)    //Freq是频率，单位为hz
{
	unsigned long Freq_Reg = 0;
	
	write_ad9833_d16(0x0100); 													//复位AD9833
	write_ad9833_d16(0x2100); 													//选择数据一次写入
	
	Freq_Reg = Freq * 10.737;  													//	Freq_Reg = (Freq * M_mclk)/(F_mclk)将频率转换为写入寄存器的值，10.737 = (M_mclk)/(F_mclk)，防止数据溢出
	
	write_ad9833_d16((0x4000 | (Freq_Reg & 0x3FFF))); 	//写入频率寄存器0 L14
	write_ad9833_d16((0x4000 | (Freq_Reg >> 14))); 			//写入频率寄存器0 H14 
	
	write_ad9833_d16((0x8000 | (Freq_Reg & 0x3FFF)));		//写入频率寄存器1 L14
	write_ad9833_d16((0x8000 | (Freq_Reg >> 14))); 			//写入频率寄存器1 H14 
	
	switch(mode)
	{
		case s_wave:                     //方波
			{
				write_ad9833_d16(0x2028);   //芯片进入工作状态,寄存器频率0输出方波波形
				delay_ms(2000);             //方便查看波形  
			
			}break;	
			
		case t_wave:       						  //寄存器频率0输出三角波波形
			{
				write_ad9833_d16(0x2002); 
				delay_ms(2000);
			
			}break;	
			
		case sin_wave:       					  //正弦波，寄存器频率1输出正弦波形
		{
				write_ad9833_d16(0x2800); 
				delay_ms(2000);
		
		}break;	
	}
}