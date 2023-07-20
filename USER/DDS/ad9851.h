#ifndef _AD9851_H_
#define _AD9851_H_
#include "common.h"
/*******************************************************************************
																						CG
*******************************************************************************/


//#define SPI1_CS PFout(4)
/*����I/O�ӿ�*/
#define ad9851_w_clk    PDout(11)  
#define ad9851_rest     PGout(3)  
#define ad9851_bit_data PGout(4) //D7
#define ad9851_fq_up    PGout(5) 

//PD11 PG3 PD12 PF11 PB0 PA15



//PB0 PF11 PD12 PG3 1 3 5 7
//PD13 PB1 PC5 PC4 6 4 2 0
#define AD98851_DATA0 PCout(4)
#define AD98851_DATA1 PBout(0)
#define AD98851_DATA2 PCout(4)
#define AD98851_DATA3 PFout(11)
#define AD98851_DATA4 PCout(1)
#define AD98851_DATA5 PDout(12)
#define AD98851_DATA6 PDout(13)
#define AD98851_DATA7 PGout(3)



// #define ad9851_w_clk_l (GPIO_ResetBits(GPIOD,GPIO_Pin_11)) 
// #define ad9851_w_clk_h (GPIO_SetBits(GPIOD,GPIO_Pin_11)) 
// #define ad9851_rest_h (GPIO_SetBits(GPIOG,GPIO_Pin_3)) 
// #define ad9851_rest_l (GPIO_ResetBits(GPIOG,GPIO_Pin_3)) 
// #define ad9851_fq_up_l (GPIO_ResetBits(GPIOG,GPIO_Pin_5)) 
// #define ad9851_fq_up_h (GPIO_SetBits(GPIOG,GPIO_Pin_5)) 

// #define ad9851_data_h (GPIO_SetBits(GPIOG,GPIO_Pin_4)) 
// #define ad9851_data_l (GPIO_ResetBits(GPIOG,GPIO_Pin_4)) 

/*������̰���״̬*/
#define NO_KEY 0XFF
#define SET 0X0A
#define CHOOSE 0x0b


void ad9851_init(void);

void ad9851_reset(void);
void ad9851_reset_serial(void);
void ad9851_wr_parrel(unsigned char w0,double frequence);
void ad9851_wr_serial1(unsigned char w0,double frequence);

void ad9851_reset_serial2(void);
void ad9851_wr_serial2(u8 w0,u32 frequence);

#endif