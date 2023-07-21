#ifndef __SPI_H
#define __SPI_H
#include "common.h"

#define SPI1_CS PFout(4)

void SPI_init(void);				  //初始化SPI1口
//发送给fpga，不用看
void SendTo_FPGA(unsigned long dataSend, unsigned char address, unsigned char NumOfBits);
//从fpga接受信号
unsigned long RecvFrom_FPGA(unsigned char address, unsigned char NumOfBits);
#endif
