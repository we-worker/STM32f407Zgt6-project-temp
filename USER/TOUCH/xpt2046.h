#ifndef __XPT2046_H__
#define __XPT2046_H__
#include "common.h"
   
//////////////////////////////////////////////////////////////////////////////////

extern u8  CMD_RDX;
extern u8  CMD_RDY;

//电阻屏芯片连接引脚	   
#define PEN  		PFin(11)  	//T_PEN
#define DOUT 		PBin(2)   	//T_MISO
#define TDIN 		PBout(1)  	//T_MOSI
#define TCLK 		PAout(5)  	//T_CLK
#define TCS  		PBout(0)  	//T_CS 
 
//触摸屏控制器参数	
extern	u16 Xdown; 		 
extern	u16 Ydown;	   //触摸屏被按下就返回的的坐标值
extern  u16 Xup;
extern  u16 Yup;       //触摸屏被按下之后抬起返回的的坐标值  

extern float xFactor;	 //触摸屏校准参数(电容屏不需要校准) 
extern float yFactor;
extern short xOffset;  
extern short yOffset;

   
//电阻屏函数
/*********电阻屏SPI通信与读取触摸点AD值*********************/
void SPI_Write_Byte(u8 num);				  //向控制芯片写入一个数据
u16 SPI_Read_AD(u8 CMD);							//读取AD转换值
u16 RTouch_Read_XorY(u8 xy);							//带滤波的坐标读取(X/Y)
u8 RTouch_Read_XY(u16 *x,u16 *y);				//双方向读取(X+Y)
u8 RTouch_Read_XY2(u16 *x,u16 *y);			//两次读取的双方向坐标读取

/*********电阻屏初始化函数*********************/
void XPT2046_Init(void);						 	//初始化

/*********电阻屏扫描按键函数*********************/
void XPT2046_Scan(u8 tp);	        //tp:0,屏幕坐标;1,物理坐标	

#endif

















