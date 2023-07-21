#ifndef __SPI_H
#define __SPI_H
#include "common.h"

#define SPI1_CS PFout(4)

void SPI_init(void); // 初始化SPI1口
// 发送给fpga，不用看
void SendTo_FPGA(unsigned long dataSend, unsigned char address, unsigned char NumOfBits);
// 从fpga接受信号
unsigned long RecvFrom_FPGA(unsigned char address, unsigned char NumOfBits);

//=======================使用案例==================

// data1 = RecvFrom_FPGA(0, 24);                              //从fpga0通道接受16位
// sprintf((char *)display_str, "data1:%ld         ", data1); //用sprintf格式化显示，给显示数组
// LCD_DisplayString(30, 110, 24, display_str);               // lcd屏幕显示数据

#endif
