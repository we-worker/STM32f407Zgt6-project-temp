#include "valuepack.h"

// 发送数据包的字节长度
const unsigned short  TXPACK_BYTE_SIZE = ((TX_BOOL_NUM+7)>>3)+TX_BYTE_NUM+(TX_SHORT_NUM<<1)+(TX_INT_NUM<<2)+(TX_FLOAT_NUM<<2);

// 接收数据包的字节长度
const unsigned short  RXPACK_BYTE_SIZE = ((RX_BOOL_NUM+7)>>3)+RX_BYTE_NUM+(RX_SHORT_NUM<<1)+(RX_INT_NUM<<2)+(RX_FLOAT_NUM<<2);

// 接收数据包的原数据加上包头、校验和包尾 之后的字节长度
unsigned short rx_pack_length = RXPACK_BYTE_SIZE+3;

// 接收计数-记录当前的数据接收进度
// 接收计数每次随串口的接收中断后 +1
long rxIndex=0;

// 读取计数-记录当前的数据包读取进度，读取计数会一直落后于接收计数，当读取计数与接收计数之间距离超过一个接收数据包的长度时，会启动一次数据包的读取。
// 读取计数每次在读取数据包后增加 +(数据包长度)
long rdIndex=0;

// 用于环形缓冲区的数组，环形缓冲区的大小可以在.h文件中定义VALUEPACK_BUFFER_SIZE
unsigned char vp_rxbuff[VALUEPACK_BUFFER_SIZE];

// 用于暂存发送数据的数组
unsigned char vp_txbuff[TXPACK_BYTE_SIZE+3];

// 初始化数据包  使用的是USART6 波特率可配置
void initValuePack(int baudrate)
{
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
    USART_InitStruct.USART_BaudRate = baudrate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART6, &USART_InitStruct);


	USART_ClearITPendingBit(USART6,USART_IT_RXNE);
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


// 数据包环形缓冲区计数
unsigned int vp_circle_rx_index;

// 串口接收中断 服务函数， 每次接收到数据后将字节存入环形缓冲区中，从头存到尾。所谓的环形缓冲区就是当接收环形缓冲区计数大于等于缓冲区的大小时（即数据到达缓冲区的尾部时）
// 数据会在缓冲区的头部继续存储，覆盖掉最早的数据。
void USART6_IRQHandler(void)
{
	// 判断是否是USART6接收了数据
	if(USART_GetITStatus(USART6, USART_IT_RXNE)) 
	{
	        // 读取数据到缓冲区中
		vp_rxbuff[vp_circle_rx_index] = USART6->DR;
	        
		// 将环形缓冲接收计数加一
		vp_circle_rx_index++;
	        // 数据到达缓冲区尾部后，转移到头部
		if(vp_circle_rx_index>=VALUEPACK_BUFFER_SIZE)
			vp_circle_rx_index=0;
	        
		// 将全局接收计数加一
		rxIndex++;
	}
	USART_ClearITPendingBit(USART6,USART_IT_RXNE);
}


// 数据读取涉及到的变量
unsigned short rdi,rdii,idl,idi,bool_index,bool_bit;
// 变量地址
uint32_t  idc;
// 记录读取的错误字节的次数
unsigned int err=0;
// 用于和校验
unsigned char sum=0;

// 存放数据包读取的结果
unsigned char isok;


//------------------------------------------------------------------------------------------------------------------------
// unsigned char readValuePack(RxPack *rx_pack_ptr)
// 尝试从缓冲区中读取数据包
// 参数   - RxPack *rx_pack_ptr： 传入接收数据结构体的指针，从环形缓冲区中读取出数据包，并将各类数据存储到rx_pack_ptr指向的结构体中
// 返回值 - 如果成功读取到数据包，则返回1，否则返回0
// 

unsigned char readValuePack(RxPack *rx_pack_ptr)
{
	
	isok = 0;
	
	// 确保读取计数和接收计数之间的距离小于2个数据包的长度
	while(rdIndex<(rxIndex-((rx_pack_length)*2)))
          rdIndex+=rx_pack_length;	
	
	// 如果读取计数落后于接收计数超过 1个 数据包的长度，则尝试读取
	while(rdIndex<=(rxIndex-rx_pack_length))
	{
		rdi = rdIndex % VALUEPACK_BUFFER_SIZE;
		rdii=rdi+1;
		if( vp_rxbuff[rdi]==PACK_HEAD) // 比较包头
		{
			if(vp_rxbuff[(rdi+RXPACK_BYTE_SIZE+2)%VALUEPACK_BUFFER_SIZE]==PACK_TAIL) // 比较包尾 确定包尾后，再计算校验和
			{
				//  计算校验和
				sum=0;
			      for(short s=0;s<RXPACK_BYTE_SIZE;s++)
				{
					rdi++;
					if(rdi>=VALUEPACK_BUFFER_SIZE)
					  rdi -= VALUEPACK_BUFFER_SIZE;
					sum += vp_rxbuff[rdi];
				}	
						rdi++;
					if(rdi>=VALUEPACK_BUFFER_SIZE)
					  rdi -= VALUEPACK_BUFFER_SIZE;
					
                                if(sum==vp_rxbuff[rdi]) // 校验和正确，则开始将缓冲区中的数据读取出来
				{
					//  提取数据包数据 一共有五步， bool byte short int float
					// 1. bool
					#if  RX_BOOL_NUM>0
						
					  idc = (uint32_t)rx_pack_ptr->bools;
					  idl = (RX_BOOL_NUM+7)>>3;
					
					bool_bit = 0;
					for(bool_index=0;bool_index<RX_BOOL_NUM;bool_index++)
					{
						*((unsigned char *)(idc+bool_index)) = (vp_rxbuff[rdii]&(0x01<<bool_bit))?1:0;
						bool_bit++;
						if(bool_bit>=8)
						{
						  bool_bit = 0;
							rdii ++;
						}
					}
					if(bool_bit)
						rdii ++;
					
				        #endif
					// 2.byte
					#if RX_BYTE_NUM>0
						idc = (uint32_t)(rx_pack_ptr->bytes);
					  idl = RX_BYTE_NUM;
					  for(idi=0;idi<idl;idi++)
					  {
					    if(rdii>=VALUEPACK_BUFFER_SIZE)
					      rdii -= VALUEPACK_BUFFER_SIZE;
					    (*((unsigned char *)idc))= vp_rxbuff[rdii];
							rdii++;
							idc++;
					  }
				        #endif
					// 3.short
					#if RX_SHORT_NUM>0
						idc = (uint32_t)(rx_pack_ptr->shorts);
					  idl = RX_SHORT_NUM<<1;
					  for(idi=0;idi<idl;idi++)
					  {
					    if(rdii>=VALUEPACK_BUFFER_SIZE)
					      rdii -= VALUEPACK_BUFFER_SIZE;
					    (*((unsigned char *)idc))= vp_rxbuff[rdii];
							rdii++;
							idc++;
					  }
				        #endif
					// 4.int
					#if RX_INT_NUM>0
						idc = (uint32_t)(&(rx_pack_ptr->integers[0]));
					  idl = RX_INT_NUM<<2;
					  for(idi=0;idi<idl;idi++)
					  {
					    if(rdii>=VALUEPACK_BUFFER_SIZE)
					      rdii -= VALUEPACK_BUFFER_SIZE;
					    (*((unsigned char *)idc))= vp_rxbuff[rdii];
							rdii++;
							idc++;
					  }
				        #endif
					// 5.float
					#if RX_FLOAT_NUM>0
						idc = (uint32_t)(&(rx_pack_ptr->floats[0]));
					  idl = RX_FLOAT_NUM<<2;
					  for(idi=0;idi<idl;idi++)
					  {
					    if(rdii>=VALUEPACK_BUFFER_SIZE)
					      rdii -= VALUEPACK_BUFFER_SIZE;
					    (*((unsigned char *)idc))= vp_rxbuff[rdii];
							rdii++;
							idc++;
					  }
				  #endif
				      
				        // 更新读取计数
					rdIndex+=rx_pack_length;
					isok = 1;
				}else
				{ 
				// 校验值错误 则 err+1 且 更新读取计数
				  rdIndex++;
			          err++;
				}
			}else
			{
				// 包尾错误 则 err+1 且 更新读取计数
				rdIndex++;
				err++;
			}		
		}else
		{ 
			// 包头错误 则 err+1 且 更新读取计数
			rdIndex++;
			err++;
		}		
	}
	return isok;
}

//-------------------------------------------------------------------------------------------------------------------------
// void sendBuffer(unsigned char *p,unsigned short length)
// 发送数据包
// 传入指针 和 字节长度

void sendBuffer(unsigned char *p,unsigned short length)
{
	  for(int i=0;i<length;i++)
   { 
      USART_SendData(USART6, *p++); 
      while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET) 
      {} 
    }
}

// 数据包发送涉及的变量
unsigned short loop;
unsigned char valuepack_tx_bit_index;
unsigned char valuepack_tx_index;

//---------------------------------------------------------------------------------------------------------
//  void sendValuePack(TxPack *tx_pack_ptr)
//  将发送数据结构体中的变量打包，并发送出去
//  传入参数- TxPack *tx_pack_ptr 待发送数据包的指针 
//  
//  先将待发送数据包结构体的变量转移到“发送数据缓冲区”中，然后将发送数据缓冲区中的数据发送
//

void sendValuePack(TxPack *tx_pack_ptr)
{
  int i;
  vp_txbuff[0]=0xa5;
  sum=0;
  //  由于结构体中不同类型的变量在内存空间的排布不是严格对齐的，中间嵌有无效字节，因此需要特殊处理	 
  valuepack_tx_bit_index = 0;
  valuepack_tx_index = 1;
	
	#if TX_BOOL_NUM>0 	  
	  for(loop=0;loop<TX_BOOL_NUM;loop++)
	  {
		  if(tx_pack_ptr->bools[loop])
			vp_txbuff[valuepack_tx_index] |= 0x01<<valuepack_tx_bit_index;
		  else
			vp_txbuff[valuepack_tx_index] &= ~(0x01<<valuepack_tx_bit_index);
		  valuepack_tx_bit_index++;
	  
		  if(valuepack_tx_bit_index>=8)
		  {
			  valuepack_tx_bit_index = 0;
			  valuepack_tx_index++;
		  }	
	  }
	  if(valuepack_tx_bit_index!=0)
		  valuepack_tx_index++;			
	#endif
	#if TX_BYTE_NUM>0 
	  
	  for(loop=0;loop<TX_BYTE_NUM;loop++)
	  {
		  vp_txbuff[valuepack_tx_index] = tx_pack_ptr->bytes[loop];
		  valuepack_tx_index++;			
	  }
	#endif
	
	#if TX_SHORT_NUM>0 
	  for(loop=0;loop<TX_SHORT_NUM;loop++)
	  {
		  vp_txbuff[valuepack_tx_index] = tx_pack_ptr->shorts[loop]&0xff;
		  vp_txbuff[valuepack_tx_index+1] = tx_pack_ptr->shorts[loop]>>8;
		  valuepack_tx_index+=2;			
	  }
	#endif
		
	#if TX_INT_NUM>0   
	  for(loop=0;loop<TX_INT_NUM;loop++)
	  {
		  i = tx_pack_ptr->integers[loop];	
		  vp_txbuff[valuepack_tx_index] = i&0xff;	
		  vp_txbuff[valuepack_tx_index+1] = (i>>8)&0xff;
		  vp_txbuff[valuepack_tx_index+2] =(i>>16)&0xff;
		  vp_txbuff[valuepack_tx_index+3] = (i>>24)&0xff;
		  valuepack_tx_index+=4;			
		}
	#endif
	
	#if TX_FLOAT_NUM>0   
	  for(loop=0;loop<TX_FLOAT_NUM;loop++)
	  {
		  i = *(int *)(&(tx_pack_ptr->floats[loop]));		
		  vp_txbuff[valuepack_tx_index] = i&0xff;
		  vp_txbuff[valuepack_tx_index+1] = (i>>8)&0xff;
		  vp_txbuff[valuepack_tx_index+2] =(i>>16)&0xff;
		  vp_txbuff[valuepack_tx_index+3] = (i>>24)&0xff;
		  valuepack_tx_index+=4;				
	  }
	#endif	

	for(unsigned short d=1;d<=TXPACK_BYTE_SIZE;d++)
		sum+=vp_txbuff[d];
		
	vp_txbuff[TXPACK_BYTE_SIZE+1] = sum;
	vp_txbuff[TXPACK_BYTE_SIZE+2] = 0x5a;
	sendBuffer(vp_txbuff,TXPACK_BYTE_SIZE+3);
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
