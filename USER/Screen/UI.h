#ifndef __UI_H
#define __UI_H
#include "common.h"
#include "stdlib.h"
#include "string.h"

#include "lcd.h"
#include "xpt2046.h"


//定义一个按钮结构体，包含位置，大小，颜色，文字，状态和触发函数
typedef struct {
    u16 x; //按钮左上角的x坐标
    u16 y; //按钮左上角的y坐标
    u16 width; //按钮的宽度
    u16 height; //按钮的高度
    u16 color; //按钮的颜色
    char* text; //按钮上显示的文字
    u8 size; //文字的大小
    u8 state; //按钮的状态，0表示未按下，1表示按下
		void (*action)(u16 id); //按钮按下时触发的函数指针，增加一个void*类型的参数
		u16 id; //用来存储参数地址的指针
} Button;


void LCD_DrawButton(Button* btn);
void LCD_CheckButton(Button* btn) ;




//定义一个结构体类型，用来存储标签框的属性
typedef struct {
  u16 x; //标签框左上角的x坐标
  u16 y; //标签框左上角的y坐标
  u16 width; //标签框的宽度
  u16 height; //标签框的高度
  u16 border_color; //标签框的边框颜色
  u16 fill_color; //标签框的填充颜色
  u16 text_color; //标签框的文本颜色
  u8 text_size; //标签框的文本大小
  char *text; //标签框的文本内容
} Label;


void LCD_Draw_Label(Label *label);
void LCD_Update_Label(Label *label) ;

void drawWaveform(u16 Xpoint, void *yValues, uint8_t isfloat, u16 Height, u16 y_offset,float Maxvalue, u16 color);


#endif
