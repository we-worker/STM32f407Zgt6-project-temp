#ifndef __UART_H
#define __UART_H	

#include "common.h"
#include "stdio.h"
#include "stdarg.h"


#define FRAME_BYTE_LENGTH 9 //串口通讯一帧数据的字节数（含帧头和帧尾），譬如20个字节为一个完整的数据帧，第1个字节帧头，第2个字节代表命令类型，第3~6字节是命令参数，第7个字节为帧尾
#define FRAME_START 0xA5 //帧头
#define FRAME_END 0x5A  //帧尾

//这里把数据帧接收标志和接收缓冲区定义成可供BTModule.c以外的源代码直接使用，只要include了BTModule.h文件。
extern __IO uint8_t USART_Rx2Buff[FRAME_BYTE_LENGTH]; //接收缓冲区
extern __IO uint8_t USART_FrameFlag; //接收完整数据帧标志，1完整，0不完整

void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);
int fputc(int ch, FILE *f);
void USART_GetChar(uint8_t nChar); //串口接收到一个字节
void USART_Process(void);

void USART6_Init(void);

void Xuehui_app_send(uint8_t *string);
#endif
