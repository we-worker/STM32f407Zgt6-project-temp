#include "SPI.h"

//==================================
// 与UART同时使用时，PC7会冲突。
////==========================================
#define CLK  PCout(6)  // 引脚定义clk信号C6
#define CS_N PCout(8) // 引脚定义cs片选信号C8
#define MOSI PCout(9) // 引脚定义MOSI信号C9
#define MISO PCin(11) // 引脚定义MISO信号C11

// 初始化spi用的引脚
void SPI_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使用某个引脚前要先启用引脚的时钟

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOC时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    CS_N = 1;
}

// 发送给fpga，不用看
void SendTo_FPGA(unsigned long dataSend, unsigned char address, unsigned char NumOfBits)
{
    unsigned char i = 0;
    CS_N = 0;
    // 发送读写bit -- 写
    MOSI = 0;
    CLK = 1;
    // delay_ms(50);
    CLK = 0;
    // 发送地址
    for (i = 0; i < 7; i = i + 1)
    {
        address = address << 1;
        if (address & 0x80)
            MOSI = 1;
        else
            MOSI = 0;
        CLK = 1;
        // delay_ms(50);
        CLK = 0;
    }
    dataSend = dataSend << (32 - NumOfBits);
    // 发送数据
    for (i = 0; i < NumOfBits; i++)
    {
        if (dataSend & 0x80000000)
            MOSI = 1;
        else
            MOSI = 0;
        CLK = 1;
        // delay_ms(50);
        CLK = 0;
        dataSend = dataSend << 1;
    }
    CS_N = 1;
}

// 从fpga接受信号
unsigned long RecvFrom_FPGA(unsigned char address, unsigned char NumOfBits)
{
    unsigned char i = 0;
    unsigned long dataRecv = 0;
    CS_N = 0;
    // 发送读写bit -- 读
    MOSI = 1;
    CLK = 1;
    // delay_us(10);
    CLK = 0;
    // 发送地址
    for (i = 0; i < 7; i = i + 1)
    {
        address = address << 1;
        if (address & 0x80)
            MOSI = 1;
        else
            MOSI = 0;
        CLK = 1;
        // delay_us(10);
        CLK = 0;
    }
    // 接收数据
    for (i = 0; i < NumOfBits; i++)
    {
        dataRecv = dataRecv << 1;
        CLK = 1;
        CLK = 0;
        if (MISO)
            dataRecv = dataRecv | 0x01;
    }
    CS_N = 1;
    return dataRecv;
}
