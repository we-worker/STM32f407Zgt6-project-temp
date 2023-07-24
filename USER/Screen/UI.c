#include "UI.h"

// ����һ��button_init�������������������buttonֵ
Button button_init(u16 x, u16 y, u16 width, u16 height, u16 color, char *text, u8 size, u8 state, void (*action)(u16 id), u16 id)
{
  Button btn;          // ����һ����ť����
  btn.x = x;           // ���ð�ť���Ͻǵ�x����
  btn.y = y;           // ���ð�ť���Ͻǵ�y����
  btn.width = width;   // ���ð�ť�Ŀ��
  btn.height = height; // ���ð�ť�ĸ߶�
  btn.color = color;   // ���ð�ť����ɫ
  btn.text = text;     // ���ð�ť����ʾ������
  btn.size = size;     // �������ֵĴ�С
  btn.state = state;   // ���ð�ť��״̬
  btn.action = action; // ���ð�ť����ʱ�����ĺ���ָ��
  btn.id = id;         // ���ò�����ַ��ָ��
  return btn;          // ���ذ�ťֵ
}

// ����һ������ť�ĺ��������ݰ�ť�ṹ������Ի��ư�ť
void LCD_DrawButton(Button *btn)
{
  // ����䰴ť�ı�����ɫ
  LCD_Fill_onecolor(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, btn->color);
  // �ٻ���ť�ı߿򣬸���״̬ѡ��ͬ����ɫ
  if (btn->state == 0)
  {                                                                                                           // δ����ʱ���߿�Ϊ��ɫ
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, WHITE);                                     // �ϱ߿�
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, WHITE);                                    // ��߿�
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK);  // �ұ߿�
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK); // �±߿�
  }
  else
  {                                                                                                           // ����ʱ���߿�Ϊ��ɫ
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, BLACK);                                     // �ϱ߿�
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, BLACK);                                    // ��߿�
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, WHITE);  // �ұ߿�
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn->height - 1, WHITE); // �±߿�
  }
  // �����ʾ��ť�ϵ����֣�������ɫΪ��ɫ��������ɫΪ͸��
  LCD_DisplayString_color(btn->x + (btn->width - strlen(btn->text) * (btn->size / 2)) / 2, btn->y + (-3 * btn->state) + (btn->height - btn->size) / 2, btn->size, btn->text, BLACK, btn->color);
}

// ����һ����ⰴť�Ƿ񱻰��µĺ��������ݴ������������ж��Ƿ��ڰ�ť�����ڣ����ı䰴ť״̬�ʹ�������
void LCD_CheckButton(Button *btn)
{

  if (Xdown >= btn->x && Xdown <= btn->x + btn->width && Ydown >= btn->y && Ydown <= btn->y + btn->height)
  { // ����ڰ�ť������
    if (btn->state == 0)
    { // ���֮ǰδ���£���ı�״̬Ϊ���£����ػ水ť
      btn->state = 1;
      LCD_DrawButton(btn);

      if (btn->action != NULL)
      {                       // ���´�������
        btn->action(btn->id); // ����paramָ����Ϊ����
      }
    }
  }
  else
  { // ������ڰ�ť������
    if (btn->state == 1)
    { // ���֮ǰ���£���ı�״̬Ϊδ���£����ػ水ť
      btn->state = 0;
      LCD_DrawButton(btn);
    }
  }
}

// ����һ��������������һ����ǩ��
void LCD_Draw_Label(Label *label)
{
  // �Ȼ��߿�
  LCD_DrawLine(label->x, label->y, label->x + label->width, label->y, label->border_color);                                 // �ϱ�
  LCD_DrawLine(label->x, label->y + label->height, label->x + label->width, label->y + label->height, label->border_color); // �±�
  LCD_DrawLine(label->x, label->y, label->x, label->y + label->height, label->border_color);                                // ���
  LCD_DrawLine(label->x + label->width, label->y, label->x + label->width, label->y + label->height, label->border_color);  // �ұ�

  // �������ɫ
  LCD_Fill_onecolor(label->x + 1, label->y + 1, label->x + label->width - 1, label->y + label->height - 1, label->fill_color);

  // �����ʾ�ı�
  LCD_DisplayString_color(label->x + (label->width - strlen(label->text) * (label->text_size / 2)) / 2,
                          label->y + (label->height - (label->text_size)) / 2,
                          label->text_size,
                          label->text,
                          label->text_color,
                          label->fill_color);
}

// ����һ�������������޸ı�ǩ�������
void LCD_Update_Label(Label *label)
{
  // �����ԭ���ı�ǩ��
  LCD_Fill_onecolor(label->x - 1, label->y - 1, label->x + label->width + 1, label->y + label->height + 1, WHITE);

  // ������»����µı�ǩ��
  LCD_Draw_Label(label);
}

// ���Ʋ��εĺ���,Xpoint:x��Ҫ���Ƶĵ���,yValues:����,isfloat,���������Ƿ��Ǹ����ͣ���֧�ָ����ͻ�u16
// Height������Ļ��Χ��y_offset:���Ƹ߶�ƫ�ƣ�Maxvalue���������ֵ�����滭ͼ��С
void drawWaveform(u16 Xpoint, void *yValues, uint8_t isfloat, u16 Height, u16 y_offset, float Maxvalue, u16 color)
{
  // �Ȼ��߿�
  // LCD_DrawLine(0, label->y, label->x + label->width, label->y, label->border_color); //�ϱ�
  // LCD_DrawLine(label->x, label->y + label->height, label->x + label->width, label->y + label->height, label->border_color); //�±�
  // LCD_DrawLine(label->x, label->y, label->x, label->y + label->height, label->border_color); //���
  // LCD_DrawLine(label->x + label->width, label->y, label->x + label->width, label->y + label->height, label->border_color); //�ұ�
  LCD_Fill_onecolor(0, y_offset, lcd_width, Height + y_offset, 0x9f31);

  u16 *yValuesU16 = (u16 *)yValues;
  float *yValuesFloat = (float *)yValues;

  float valueRange = 1.0f * Height / Maxvalue;
  // ���Ʋ���
  for (u16 i = 0; i < Xpoint; i++)
  {
    u16 x = i * (lcd_width * 1.0f / Xpoint);
    u16 y;
    if (isfloat)
      y = Height - ((yValuesFloat[i]) * valueRange) + y_offset;
    else
      y = Height - ((yValuesU16[i]) * valueRange) + y_offset;

    // ���Ƶ�ǰADCֵ�Ĳ��ε�
    LCD_Color_DrawPoint(x, y, color);
    // ����������һ��ADCֵ�Ĳ����߶�
    if (i > 0)
    {
      u16 prev_x = (i - 1) * (lcd_width * 1.0f / Xpoint);
      u16 prev_y;
      if (isfloat)
        prev_y = Height - ((yValuesFloat[i - 1]) * valueRange) + y_offset;
      else
        prev_y = Height - ((yValuesU16[i - 1]) * valueRange) + y_offset;
      LCD_DrawLine(prev_x, prev_y, x, y, color); // ʹ��ָ����ɫ����
    }
  }
}
