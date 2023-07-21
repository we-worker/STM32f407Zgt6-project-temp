#include "xpt2046.h" 
#include "lcd.h"
#include "stdlib.h"
 


/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: xpt2046.c                                                            *
* 文件简述：电阻屏触摸驱动程序                                                   *
* 创建日期：2018.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/	 
	
u16 Xdown=0; 		 
u16 Ydown=0;	     //触摸屏被按下就返回的的坐标值
u16 Xup=0;
u16 Yup=0; 			   //触摸屏被按下之后抬起返回的的坐标值 

/**************默认为竖屏参数***************/
u8  CMD_RDX=0XD0;
u8  CMD_RDY=0X90;

float xFactor=0.06671114;	 //竖屏校准参数 
float yFactor=0.09117551;
short xOffset=-11;       //xOffset和yOffset 可以根据实际手头的电阻屏适当调整到精准
short yOffset=-18;
/*******************************************/

/**************以下为普通IO模拟SPI通信XPT2046***************/
/****************************************************************************
* 名    称: void SPI_Write_Byte(u8 num) 
* 功    能：SPI写数据,向触摸屏IC写入1byte数据 
* 入口参数：num:要写入的数据
* 返回参数：无
* 说    明：       
****************************************************************************/
void SPI_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 
		delay_us(1);
		TCLK=1;		 //上升沿有效	        
	}		 			    
} 	

/****************************************************************************
* 名    称: u16 SPI_Read_AD(u8 CMD)
* 功    能：SPI读数据 ,从触摸屏IC读取adc值
* 入口参数：CMD:指令
* 返回参数: 读到的数据	   
* 说    明：       
****************************************************************************/  
u16 SPI_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//先拉低时钟 	 
	TDIN=0; 	//拉低数据线
	TCS=0; 		//选中触摸屏IC 2046
	SPI_Write_Byte(CMD);//发送命令字
	delay_us(6); 
	TCLK=0; 	     	    
	delay_us(1);    	   
	TCLK=1;		 
	delay_us(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	 //下降沿有效  	    	   
		delay_us(1);    
 		TCLK=1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//高12位才有效
	TCS=1;		  //释放片选	 
	return(Num);   
}
/**************以上为普通IO模拟SPI通信XPT2046***************/


/****************************************************************************
* 名    称: u16 RTouch_Read_XorY(u8 xy)
* 功    能：读取一个坐标值(x或者y)
* 入口参数：xoy:指令（CMD_RDX/CMD_RDY
* 返回参数: 读到的数据	   
* 说    明：连续读取5次数据,对这些数据升序排列, 然后去掉最低和最1个数,取平均值       
****************************************************************************/ 
u16 RTouch_Read_XorY(u8 xoy)
{
	u16 i, j;
	u16 buf[5];
	u16 sum=0;
	u16 temp;
	
	for(i=0;i<5;i++)buf[i] = SPI_Read_AD(xoy);		 		    
	for(i=0;i<5-1; i++)  //排序
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//升序 
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=1;i<5-1;i++)sum+=buf[i];
	temp=sum/(5-2*1);
	return temp;   
} 

/****************************************************************************
* 名    称: u8 RTouch_Read_XY(u16 *x,u16 *y)
* 功    能：读取x,y坐标
* 入口参数：x,y:读取到的坐标值
* 返回参数: 返回值:0,失败;1,成功   
* 说    明：最小值不能少于50.       
****************************************************************************/ 
u8 RTouch_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=RTouch_Read_XorY(CMD_RDX);
	ytemp=RTouch_Read_XorY(CMD_RDY);	  												   
	if(xtemp<50||ytemp<50)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}

/****************************************************************************
* 名    称: u8 RTouch_Read_XY2(u16 *x,u16 *y)
* 功    能：连续2次读取触摸屏IC
* 入口参数：x,y:读取到的坐标值
* 返回参数: 返回值:0,失败;1,成功   
* 说    明：        
****************************************************************************/
u8 RTouch_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=RTouch_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=RTouch_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+50)||(x1<=x2&&x2<x1+50))  //前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+50)||(y1<=y2&&y2<y1+50)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  


u16 x;
u16 y;
u8 time;
/****************************************************************************
* 名    称: void XPT2046_Scan(u8 tp)
* 功    能：触摸按键扫描	
* 入口参数：type:0,屏幕坐标;1,物理坐标 
* 返回参数: 无  
* 说    明：        
****************************************************************************/
void XPT2046_Scan(u8 type)
{			   
		Xup=0xffff;
    Yup=0xffff;	 
	if(PEN==0)//有按键按下
	{
		if(type)RTouch_Read_XY2(&x,&y);//读取物理坐标
		else if(RTouch_Read_XY2(&x,&y))//读取屏幕坐标
		{
	 		x=xFactor*x+xOffset;     //将结果转换为屏幕坐标
			y=yFactor*y+yOffset;  
	 	} 	
		Xdown=x;
		Ydown=y;
		
		time++;		   
	}else  //建抬起
	{    
		 if(time>2)
		 {
			 		Xup=x;
		      Yup=y;	 
		 }
		 time=0;
		 Xdown=0xffff;
     Ydown=0xffff;	 
	}
}	

//xpt2046初始化  		    
void XPT2046_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;	  
		
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOB,C,F时钟

	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2; //设置为上拉输入
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	 GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	 GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;//设置为推挽输出
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //输出模式
	 GPIO_Init(GPIOB, &GPIO_InitStructure); 

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	 GPIO_Init(GPIOA, &GPIO_InitStructure); 	
   
		if(dir_flag)  //如果竖屏调转 X Y
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	
      xFactor=-0.09195402;	//横屏校准参数 
      yFactor=0.06736275;
      xOffset=348;
      yOffset=-19;			
		}
		else				    
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;
      xFactor=0.06671114;	  //竖屏校准参数 
      yFactor=0.09117551;
      xOffset=-11;  
      yOffset=-18;				
		}										 
}

