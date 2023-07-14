
#include "AD9833.h"
#include <stdio.h>


#define ad9833_sdata   PGout(5) 
#define ad9833_sclk    PGout(4)
#define ad9833_fsync   PGout(3)

void AD9833_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  

  //��A4Ϊ��λλ������Ϊ�͵�ƽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4  |	GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	// �������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //�������
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}

/*��AD9833д16λ����*/
void write_ad9833_d16(unsigned int x)
{
	int i,w;
	ad9833_sclk=1;
	ad9833_fsync=1;
	ad9833_fsync=0;
	w=x;                       //д16λ����
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
AD9833�������

mode:   �������ѡ��s_wave    ����
										 t_wave    ���ǲ�
										 sin_wave  ���Ҳ�

Freq��  ������ε�Ƶ��
************************************************************/

void AD9833_WaveOut(unsigned int mode,unsigned long Freq)    //Freq��Ƶ�ʣ���λΪhz
{
	unsigned long Freq_Reg = 0;
	
	write_ad9833_d16(0x0100); 													//��λAD9833
	write_ad9833_d16(0x2100); 													//ѡ������һ��д��
	
	Freq_Reg = Freq * 10.737;  													//	Freq_Reg = (Freq * M_mclk)/(F_mclk)��Ƶ��ת��Ϊд��Ĵ�����ֵ��10.737 = (M_mclk)/(F_mclk)����ֹ�������
	
	write_ad9833_d16((0x4000 | (Freq_Reg & 0x3FFF))); 	//д��Ƶ�ʼĴ���0 L14
	write_ad9833_d16((0x4000 | (Freq_Reg >> 14))); 			//д��Ƶ�ʼĴ���0 H14 
	
	write_ad9833_d16((0x8000 | (Freq_Reg & 0x3FFF)));		//д��Ƶ�ʼĴ���1 L14
	write_ad9833_d16((0x8000 | (Freq_Reg >> 14))); 			//д��Ƶ�ʼĴ���1 H14 
	
	switch(mode)
	{
		case s_wave:                     //����
			{
				write_ad9833_d16(0x2028);   //оƬ���빤��״̬,�Ĵ���Ƶ��0�����������
				delay_ms(2000);             //����鿴����  
			
			}break;	
			
		case t_wave:       						  //�Ĵ���Ƶ��0������ǲ�����
			{
				write_ad9833_d16(0x2002); 
				delay_ms(2000);
			
			}break;	
			
		case sin_wave:       					  //���Ҳ����Ĵ���Ƶ��1������Ҳ���
		{
				write_ad9833_d16(0x2800); 
				delay_ms(2000);
		
		}break;	
	}
}