#include "uart.h"
#define BUFFER_SIZE 128

__IO uint8_t nRx2Counter=0; //接收字节数
__IO uint8_t USART_Rx2Buff[FRAME_BYTE_LENGTH]; //接收缓冲区
__IO uint8_t USART_FrameFlag = 0; //接收完整数据帧标志，1完整，0不完整

volatile uint8_t rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;

void USART6_Init(void) {
    // 使能 USART6 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    // GPIO 初始化结构体
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;


    // 配置 USART6 引脚
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // 配置 PC6 为 USART6_TX 引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 配置 PC7 为 USART6_RX 引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 将 PC6 和 PC7 引脚与 USART6 的功能映射起来
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);  // USART6_TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);  // USART6_RX

    // 配置 USART6
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART6, &USART_InitStruct);



    // 配置接收中断
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

    // 使能 USART6
    USART_Cmd(USART6, ENABLE);

    // 配置 NVIC 中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


/******************************************************
		格式化串口输出函数
        "\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
		"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
		"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
		"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
**********************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //判断是否到达字符串结束符
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   
					Data++;
					break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
	
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //十进制
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

//重定向printf到串口
int fputc(int ch, FILE *f)
{
 	USART_SendData(USART6, (unsigned char) ch);
 	while( USART_GetFlagStatus(USART6,USART_FLAG_TC)!= SET);
	return (ch);
}

void USART_GetChar(uint8_t nChar) //串口接收到一个字节
{

	if(USART_FrameFlag == 1) return;   //如果上次的数据帧还没处理过，则返回
	
	if(nRx2Counter==0 && nChar == FRAME_START)
	{
		USART_Rx2Buff[nRx2Counter++]=nChar;  //保存到缓冲区
	}
	else if(nRx2Counter>0) //接收到帧头以后才继续保存
	{
		USART_Rx2Buff[nRx2Counter++]=nChar;  //保存到缓冲区
		if(nRx2Counter>=FRAME_BYTE_LENGTH)  //接收到一帧数据
		{
			nRx2Counter = 0;
			if(USART_Rx2Buff[FRAME_BYTE_LENGTH-1] == FRAME_END) //如果最后一个字节是帧尾，则数据帧完整
			{
				USART_FrameFlag=1;
			}
		}
	}	
}


//ch1,ch2,ch3这样一组
void Xuehui_app_send(uint8_t *string){
	uint8_t cmdf[2]={0x03,0xFC};//帧头
	uint8_t cmdr[2]={0xFC,0x03};//帧头
	
	USART_OUT(USART6,cmdf);
	USART_OUT(USART6,string);
	USART_OUT(USART6,cmdr);

}

void USART6_IRQHandler(void) {
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
					// 从 USART6 接收数据
        uint8_t data = USART_ReceiveData(USART6);

        // 在这里进行接收数据的处理
        // 例如，将数据存储到缓冲区
        rx_buffer[rx_index++] = data;

        // 如果缓冲区已满，则重置索引
        if (rx_index >= BUFFER_SIZE) {
            rx_index = 0;
        }

        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
    }
}
