#include "led.h" 

/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: led.c                                                                *
* 文件简述：LED初始化                                                            *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：LED对应IO口初始化                                                    * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

//LED对应IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

  //PG13、PG14和PG15初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;         //LED0、LED1和LED2对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                  //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;             //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                   //上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);                         //初始化GPIO
	
	GPIO_SetBits(GPIOE, GPIO_Pin_3 | GPIO_Pin_4);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOG, GPIO_Pin_9);
	
}






