#include "AD9833.h"


#define		FSYNC_1()     	GPIO_SetBits(GPIOG,GPIO_Pin_3)//�����
#define		FSYNC_0()   	GPIO_ResetBits(GPIOG,GPIO_Pin_3)//�����
#define     SCK_1()			GPIO_SetBits(GPIOG,GPIO_Pin_4)//�����
#define 	SCK_0()			GPIO_ResetBits(GPIOG,GPIO_Pin_4)//�����
#define 	DAT_1()			GPIO_SetBits(GPIOG,GPIO_Pin_5)//�����
#define 	DAT_0()			GPIO_ResetBits(GPIOG,GPIO_Pin_5)//�����

#define		FSYNC2_1()     	GPIO_SetBits(GPIOD,GPIO_Pin_3)//�����
#define		FSYNC2_0()   	GPIO_ResetBits(GPIOD,GPIO_Pin_3)//�����
#define     SCK2_1()			GPIO_SetBits(GPIOA,GPIO_Pin_8)//�����
#define 	SCK2_0()			GPIO_ResetBits(GPIOA,GPIO_Pin_8)//�����
#define 	DAT2_1()			GPIO_SetBits(GPIOE,GPIO_Pin_6)//�����
#define 	DAT2_0()			GPIO_ResetBits(GPIOE,GPIO_Pin_6)//�����


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
*	�� �� ��: AD9833_Delay
*	����˵��: ʱ����ʱ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AD9833_Delay(void)
{
	unsigned int i;
	for (i = 0; i < 1; i++);
}



/*
*********************************************************************************************************
*	�� �� ��: AD9833_Write
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: TxData : ����
*	�� �� ֵ: ��
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
*	�� �� ��: AD9833_AmpSet
*	����˵��: �ı�����źŷ���ֵ
*	��    ��: 1.amp ������ֵ  0- 255
*	�� �� ֵ: ��
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
/* ������λ�Ĵ�����ֵ */
void SetAD9833PhaseRegister(unsigned int Phase,uint8_t drive_id)
{
	unsigned int Phs_data=0;
if(last_phase_switch[drive_id-1]==1){
	 Phs_data=Phase|0xC000;	//��λֵ
	last_phase_switch[drive_id-1]=0;
}else{

	 Phs_data=Phase|0xE000;	//��λֵ
	last_phase_switch[drive_id-1]=1;
}
	//AD9833_Write(0x2100);
//int Phs_data=Phase|0xC000;	//��λֵ
// Phase&=0x0FFF;
// Phase|=0xD000;
AD9833_Write(Phs_data,drive_id);	//������λ
}

/*
*********************************************************************************************************
*	�� �� ��: AD9833_WaveSeting
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: 1.Freq: Ƶ��ֵ, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 �� 1
			  3.WaveMode: TRI_WAVE(���ǲ�),SIN_WAVE(���Ҳ�),SQU_WAVE(����)
			  4.Phase : ���εĳ���λ
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase,uint8_t drive_id )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
		frequence_mid=268435456/25;//�ʺ�25M����//frequence_mid=268435456/25;//�ʺ�25M����
		//���ʱ��Ƶ�ʲ�Ϊ25MHZ���޸ĸô���Ƶ��ֵ����λMHz ��AD9833���֧��25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex; //frequence_hex��16λ�͸�frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ
		frequence_MSB=frequence_hex>>14; //frequence_hex��16λ�͸�frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ

		Phs_data=Phase|0xC000;	//��λֵ
		AD9833_Write(0x0100,drive_id); //��λAD9833,��RESETλΪ1
		AD9833_Write(0x2100,drive_id); //ѡ������һ��д�룬B28λ��RESETλΪ1

		if(Freq_SFR==0)				  //���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9833_Write(frequence_LSB,drive_id); //L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_Write(frequence_MSB,drive_id); //H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9833_Write(Phs_data,drive_id);	//������λ
			//AD9833_Write(0x2000); /**����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������**/
	    }
		if(Freq_SFR==1)				//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9833_Write(frequence_LSB,drive_id); //L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_Write(frequence_MSB,drive_id); //H14 Ƶ�ʼĴ���1Ϊ
			AD9833_Write(Phs_data,drive_id);	//������λ
			//AD9833_Write(0x2800); /**����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������**/
		}

		if(WaveMode==TRI_WAVE) //������ǲ�����
		 	AD9833_Write(0x2002,drive_id); 
		if(WaveMode==SQU_WAVE)	//�����������
			AD9833_Write(0x2028,drive_id); 
		if(WaveMode==SIN_WAVE)	//������Ҳ���
			AD9833_Write(0x2000,drive_id); 

}

