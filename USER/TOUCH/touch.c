#include "touch.h"
#include "xpt2046.h"
#include "lcd.h"
#include "led.h"

/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: touch.c                                                              *
* 文件简述：触摸屏测试程序                                                       *
* 创建日期：2018.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/

//触摸屏初始化
void Touch_Init(void)
{
	if(lcd_id==0x9341)
	{
	  XPT2046_Init();
	}

}

//清空屏幕
void Clear_Screen(void)
{
	LCD_Clear(WHITE);//清屏   
 	BRUSH_COLOR=BLUE;//设置字体为蓝色 
	LCD_DisplayString(lcd_width-40,lcd_height-18,16,"Clear");//显示清屏区域
  BRUSH_COLOR=RED;//设置画笔蓝色 
}

/****************************************************************************
* 名    称: u8 Draw_Point(u16 x,u16 y,u16 color)
* 功    能：画一个点(4*4的点)	
* 入口参数：x,y:坐标
            color:颜色
* 返回参数: 无  
* 说    明：        
****************************************************************************/
void Draw_Point(u16 x,u16 y,u16 color)
{	    	
	u8 i=0;

	BRUSH_COLOR=color;
	for(i=0;i<4;i++)
	{
	 LCD_DrawPoint(x,y+i);
	 LCD_DrawPoint(x+1,y+i);
	 LCD_DrawPoint(x+2,y+i);
	 LCD_DrawPoint(x+3,y+i);
	} 	  	
}	

//电阻触摸屏测试函数
void R_Touch_test(void)
{
	u8 i=0;	  
	while(1)
	{
		XPT2046_Scan(0); 		 
	
		 	if(Xdown<lcd_width&&Ydown<lcd_height)
			{	
				if(Xdown>(lcd_width-40)&&Ydown>lcd_height-18)Clear_Screen();  //清空屏幕
				else Draw_Point(Xdown,Ydown,RED);		//画图	  			   
			} 
    			
		i++;
		if(i%20==0)
			LED0=!LED0;
	}
}

