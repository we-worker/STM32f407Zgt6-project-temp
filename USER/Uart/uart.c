#include "uart.h"
#define BUFFER_SIZE 128

__IO uint8_t nRx2Counter=0; //�����ֽ���
__IO uint8_t USART_Rx2Buff[FRAME_BYTE_LENGTH]; //���ջ�����
__IO uint8_t USART_FrameFlag = 0; //������������֡��־��1������0������

volatile uint8_t rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;

void USART6_Init(void) {
    // ʹ�� USART6 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    // GPIO ��ʼ���ṹ��
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;


    // ���� USART6 ����
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // ���� PC6 Ϊ USART6_TX ����
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // ���� PC7 Ϊ USART6_RX ����
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // �� PC6 �� PC7 ������ USART6 �Ĺ���ӳ������
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);  // USART6_TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);  // USART6_RX

    // ���� USART6
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART6, &USART_InitStruct);



    // ���ý����ж�
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

    // ʹ�� USART6
    USART_Cmd(USART6, ENABLE);

    // ���� NVIC �ж����ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


/******************************************************
		��ʽ�������������
        "\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
		"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
		"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
		"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
**********************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);	   
					Data++;
					break;
				case 'n':							          //���з�
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
				case 's':										  //�ַ���
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //ʮ����
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
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
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

//�ض���printf������
int fputc(int ch, FILE *f)
{
 	USART_SendData(USART6, (unsigned char) ch);
 	while( USART_GetFlagStatus(USART6,USART_FLAG_TC)!= SET);
	return (ch);
}

void USART_GetChar(uint8_t nChar) //���ڽ��յ�һ���ֽ�
{

	if(USART_FrameFlag == 1) return;   //����ϴε�����֡��û��������򷵻�
	
	if(nRx2Counter==0 && nChar == FRAME_START)
	{
		USART_Rx2Buff[nRx2Counter++]=nChar;  //���浽������
	}
	else if(nRx2Counter>0) //���յ�֡ͷ�Ժ�ż�������
	{
		USART_Rx2Buff[nRx2Counter++]=nChar;  //���浽������
		if(nRx2Counter>=FRAME_BYTE_LENGTH)  //���յ�һ֡����
		{
			nRx2Counter = 0;
			if(USART_Rx2Buff[FRAME_BYTE_LENGTH-1] == FRAME_END) //������һ���ֽ���֡β��������֡����
			{
				USART_FrameFlag=1;
			}
		}
	}	
}


//ch1,ch2,ch3����һ��
void Xuehui_app_send(uint8_t *string){
	uint8_t cmdf[2]={0x03,0xFC};//֡ͷ
	uint8_t cmdr[2]={0xFC,0x03};//֡ͷ
	
	USART_OUT(USART6,cmdf);
	USART_OUT(USART6,string);
	USART_OUT(USART6,cmdr);

}

void USART6_IRQHandler(void) {
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
					// �� USART6 ��������
        uint8_t data = USART_ReceiveData(USART6);

        // ��������н������ݵĴ���
        // ���磬�����ݴ洢��������
        rx_buffer[rx_index++] = data;

        // �������������������������
        if (rx_index >= BUFFER_SIZE) {
            rx_index = 0;
        }

        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
    }
}
