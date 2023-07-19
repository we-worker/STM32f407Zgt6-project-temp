#include "UI.h"



//定义一个画按钮的函数，根据按钮结构体的属性绘制按钮
void LCD_DrawButton(Button* btn) {
  //先填充按钮的背景颜色
  LCD_Fill_onecolor(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, btn->color);
  //再画按钮的边框，根据状态选择不同的颜色
  if (btn->state == 0) { //未按下时，边框为白色
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, WHITE); //上边框
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, WHITE); //左边框
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK); //右边框
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK); //下边框
  } else { //按下时，边框为黑色
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, BLACK); //上边框
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, BLACK); //左边框
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, WHITE); //右边框
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn -> height - 1, WHITE); //下边框
  }
  //最后显示按钮上的文字，文字颜色为黑色，背景颜色为透明
  LCD_DisplayString_color(btn -> x +(btn -> width -strlen(btn -> text) *(btn -> size /2)) /2 ,btn -> y+(-3*btn -> state) +(btn -> height -btn -> size)/2 ,btn -> size ,btn -> text ,BLACK ,btn->color);
}

//定义一个检测按钮是否被按下的函数，根据触摸屏的坐标判断是否在按钮区域内，并改变按钮状态和触发函数
void LCD_CheckButton(Button* btn) {

  if (Xdown >=btn -> x && Xdown <=btn -> x+btn -> width && Ydown >=btn -> y && Ydown <=btn -> y+btn -> height) { //如果在按钮区域内
    if (btn -> state ==0) { //如果之前未按下，则改变状态为按下，并重绘按钮
      btn -> state =1;
      LCD_DrawButton(btn);
			
			if (btn -> action !=NULL) { //按下触发程序
				btn -> action(btn -> id); //传入param指针作为参数
			}
    }
  } else { //如果不在按钮区域内
    if (btn -> state ==1) { //如果之前按下，则改变状态为未按下，并重绘按钮
      btn -> state =0;
      LCD_DrawButton(btn);		
    }
  }
}






//定义一个函数，用来画一个标签框
void LCD_Draw_Label(Label *label) {
  //先画边框
  LCD_DrawLine(label->x, label->y, label->x + label->width, label->y, label->border_color); //上边
  LCD_DrawLine(label->x, label->y + label->height, label->x + label->width, label->y + label->height, label->border_color); //下边
  LCD_DrawLine(label->x, label->y, label->x, label->y + label->height, label->border_color); //左边
  LCD_DrawLine(label->x + label->width, label->y, label->x + label->width, label->y + label->height, label->border_color); //右边

  //再填充颜色
  LCD_Fill_onecolor(label->x + 1, label->y + 1, label->x + label->width - 1, label->y + label->height - 1, label->fill_color);

  //最后显示文本
  LCD_DisplayString_color(label->x + (label->width - strlen(label->text) * (label->text_size / 2)) / 2, 
                          label->y + (label->height - (label->text_size)) / 2,
                          label->text_size,
                          label->text,
                          label->text_color,
                          label->fill_color);
}



//定义一个函数，用来修改标签框的属性
void LCD_Update_Label(Label *label) {
  //先清除原来的标签框
  LCD_Fill_onecolor(label->x - 1, label->y - 1, label->x + label->width + 1, label->y + label->height + 1, WHITE);

  //最后重新画出新的标签框
  LCD_Draw_Label(label);
}





