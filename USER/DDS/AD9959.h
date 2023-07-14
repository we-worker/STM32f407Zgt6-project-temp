#ifndef _AD9959_H
#define _AD9959_H

#define uchar unsigned char
#define uint unsigned int 


//AD998 I/O��������


//����ӳ���
/*
//PD11 PG3 PD12 PF11 PB0 PA15
//PG5  PG4 PD13 PB1 PC5 PC4

//SD3 SD1 SCK IU P2 P0
//RET SD2 SD0 CS P3 P1

PD11�C>SD3 PG3�C>SD1 PD12�C>SCK PF11�C>IU PB0�C>P2 PA15�C>P0
PG5�C>RET PG4�C>SD2 PD13�C>SD0 PB1�C>CS PC5�C>P3 PC4�C>P1

*/
//�˿�C�궨��
#define SCLK_1 (GPIO_SetBits(GPIOD,GPIO_Pin_12)) 
#define SCLK_0 (GPIO_ResetBits(GPIOD,GPIO_Pin_12)) 

#define CS_1 (GPIO_SetBits(GPIOB,GPIO_Pin_1))
#define CS_0 (GPIO_ResetBits(GPIOB,GPIO_Pin_1))

#define IO_update_1 (GPIO_SetBits(GPIOF,GPIO_Pin_11)) 
#define IO_update_0 (GPIO_ResetBits(GPIOF,GPIO_Pin_11)) 

#define SDIO0_1 (GPIO_SetBits(GPIOD,GPIO_Pin_13))
#define SDIO0_0 (GPIO_ResetBits(GPIOD,GPIO_Pin_13))

#define PS0_1 (GPIO_SetBits(GPIOA,GPIO_Pin_15)) 
#define PS0_0 (GPIO_ResetBits(GPIOA,GPIO_Pin_15)) 

#define PS1_1 (GPIO_SetBits(GPIOC,GPIO_Pin_4))
#define PS1_0 (GPIO_ResetBits(GPIOC,GPIO_Pin_4))

#define PS2_1 (GPIO_SetBits(GPIOB,GPIO_Pin_0))	
#define PS2_0 (GPIO_ResetBits(GPIOB,GPIO_Pin_0))	

#define PS3_1 (GPIO_SetBits(GPIOC,GPIO_Pin_5))
#define PS3_0 (GPIO_ResetBits(GPIOC,GPIO_Pin_5))

//�˿�A�궨��
#define SDIO1_1 (GPIO_SetBits(GPIOG,GPIO_Pin_3))
#define SDIO1_0 (GPIO_ResetBits(GPIOG,GPIO_Pin_3))

#define SDIO2_1  (GPIO_SetBits(GPIOG,GPIO_Pin_4))
#define SDIO2_0 (GPIO_ResetBits(GPIOG,GPIO_Pin_4))

#define SDIO3_1  (GPIO_SetBits(GPIOD,GPIO_Pin_11))		
#define SDIO3_0 (GPIO_ResetBits(GPIOD,GPIO_Pin_11))	

#define Reset_1  (GPIO_SetBits(GPIOG,GPIO_Pin_5))
#define Reset_0 (GPIO_ResetBits(GPIOG,GPIO_Pin_5))




//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void GPIO_AD9959_Configuration(void)  ;//I/O�˿�����
extern void AD9959_Init(void);  //��λ
extern void WrFrequencyTuningWorddata(double f,uchar *ChannelFrequencyTuningWorddata);
extern void IO_update(void)  ;
extern void WriteToAD9959ViaSpi(uchar RegisterAddress, uchar NumberofRegisters, uchar *RegisterData,uchar temp) ;
extern void WrPhaseOffsetTuningWorddata(double f,uchar *ChannelPhaseOffsetTuningWorddata);
extern void WrAmplitudeTuningWorddata(double f,uchar *ChannelAmplitudeTuningWorddata);//�������ע�������Ƶ������
extern void WrAmplitudeTuningWorddata1(double f,uchar *ChannelAmplitudeTuningWorddata,uchar *ASRAmplituteWordata);

#endif
