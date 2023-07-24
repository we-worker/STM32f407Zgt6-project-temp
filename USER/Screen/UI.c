#include "UI.h"

// 定义一个button_init函数，传入参数，返回button值
Button button_init(u16 x, u16 y, u16 width, u16 height, u16 color, char *text, u8 size, u8 state, void (*action)(u16 id), u16 id)
{
  Button btn;          // 定义一个按钮变量
  btn.x = x;           // 设置按钮左上角的x坐标
  btn.y = y;           // 设置按钮左上角的y坐标
  btn.width = width;   // 设置按钮的宽度
  btn.height = height; // 设置按钮的高度
  btn.color = color;   // 设置按钮的颜色
  btn.text = text;     // 设置按钮上显示的文字
  btn.size = size;     // 设置文字的大小
  btn.state = state;   // 设置按钮的状态
  btn.action = action; // 设置按钮按下时触发的函数指针
  btn.id = id;         // 设置参数地址的指针
  return btn;          // 返回按钮值
}

// 定义一个画按钮的函数，根据按钮结构体的属性绘制按钮
void LCD_DrawButton(Button *btn)
{
  // 先填充按钮的背景颜色
  LCD_Fill_onecolor(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, btn->color);
  // 再画按钮的边框，根据状态选择不同的颜色
  if (btn->state == 0)
  {                                                                                                           // 未按下时，边框为白色
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, WHITE);                                     // 上边框
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, WHITE);                                    // 左边框
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK);  // 右边框
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK); // 下边框
  }
  else
  {                                                                                                           // 按下时，边框为黑色
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, BLACK);                                     // 上边框
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, BLACK);                                    // 左边框
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, WHITE);  // 右边框
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn->height - 1, WHITE); // 下边框
  }
  // 最后显示按钮上的文字，文字颜色为黑色，背景颜色为透明
  LCD_DisplayString_color(btn->x + (btn->width - strlen(btn->text) * (btn->size / 2)) / 2, btn->y + (-3 * btn->state) + (btn->height - btn->size) / 2, btn->size, btn->text, BLACK, btn->color);
}

// 定义一个检测按钮是否被按下的函数，根据触摸屏的坐标判断是否在按钮区域内，并改变按钮状态和触发函数
void LCD_CheckButton(Button *btn)
{

  if (Xdown >= btn->x && Xdown <= btn->x + btn->width && Ydown >= btn->y && Ydown <= btn->y + btn->height)
  { // 如果在按钮区域内
    if (btn->state == 0)
    { // 如果之前未按下，则改变状态为按下，并重绘按钮
      btn->state = 1;
      LCD_DrawButton(btn);

      if (btn->action != NULL)
      {                       // 按下触发程序
        btn->action(btn->id); // 传入param指针作为参数
      }
    }
  }
  else
  { // 如果不在按钮区域内
    if (btn->state == 1)
    { // 如果之前按下，则改变状态为未按下，并重绘按钮
      btn->state = 0;
      LCD_DrawButton(btn);
    }
  }
}

// 定义一个函数，用来画一个标签框
void LCD_Draw_Label(Label *label)
{
  // 先画边框
  LCD_DrawLine(label->x, label->y, label->x + label->width, label->y, label->border_color);                                 // 上边
  LCD_DrawLine(label->x, label->y + label->height, label->x + label->width, label->y + label->height, label->border_color); // 下边
  LCD_DrawLine(label->x, label->y, label->x, label->y + label->height, label->border_color);                                // 左边
  LCD_DrawLine(label->x + label->width, label->y, label->x + label->width, label->y + label->height, label->border_color);  // 右边

  // 再填充颜色
  LCD_Fill_onecolor(label->x + 1, label->y + 1, label->x + label->width - 1, label->y + label->height - 1, label->fill_color);

  // 最后显示文本
  LCD_DisplayString_color(label->x + (label->width - strlen(label->text) * (label->text_size / 2)) / 2,
                          label->y + (label->height - (label->text_size)) / 2,
                          label->text_size,
                          label->text,
                          label->text_color,
                          label->fill_color);
}

// 定义一个函数，用来修改标签框的属性
void LCD_Update_Label(Label *label)
{
  // 先清除原来的标签框
  LCD_Fill_onecolor(label->x - 1, label->y - 1, label->x + label->width + 1, label->y + label->height + 1, WHITE);

  // 最后重新画出新的标签框
  LCD_Draw_Label(label);
}

// 绘制波形的函数,Xpoint:x轴要绘制的点数,yValues:数据,isfloat,输入数据是否是浮点型，仅支持浮点型或u16
// Height绘制屏幕范围，y_offset:绘制高度偏移，Maxvalue：数据最大值，方面画图大小
void drawWaveform(u16 Xpoint, void *yValues, uint8_t isfloat, u16 Height, u16 y_offset, float Maxvalue, u16 color)
{
  // 先画边框
  // LCD_DrawLine(0, label->y, label->x + label->width, label->y, label->border_color); //上边
  // LCD_DrawLine(label->x, label->y + label->height, label->x + label->width, label->y + label->height, label->border_color); //下边
  // LCD_DrawLine(label->x, label->y, label->x, label->y + label->height, label->border_color); //左边
  // LCD_DrawLine(label->x + label->width, label->y, label->x + label->width, label->y + label->height, label->border_color); //右边
  LCD_Fill_onecolor(0, y_offset, lcd_width, Height + y_offset, 0x9f31);

  u16 *yValuesU16 = (u16 *)yValues;
  float *yValuesFloat = (float *)yValues;

  float valueRange = 1.0f * Height / Maxvalue;
  // 绘制波形
  for (u16 i = 0; i < Xpoint; i++)
  {
    u16 x = i * (lcd_width * 1.0f / Xpoint);
    u16 y;
    if (isfloat)
      y = Height - ((yValuesFloat[i]) * valueRange) + y_offset;
    else
      y = Height - ((yValuesU16[i]) * valueRange) + y_offset;

    // 绘制当前ADC值的波形点
    LCD_Color_DrawPoint(x, y, color);
    // 绘制连接上一个ADC值的波形线段
    if (i > 0)
    {
      u16 prev_x = (i - 1) * (lcd_width * 1.0f / Xpoint);
      u16 prev_y;
      if (isfloat)
        prev_y = Height - ((yValuesFloat[i - 1]) * valueRange) + y_offset;
      else
        prev_y = Height - ((yValuesU16[i - 1]) * valueRange) + y_offset;
      LCD_DrawLine(prev_x, prev_y, x, y, color); // 使用指定颜色画线
    }
  }
}
