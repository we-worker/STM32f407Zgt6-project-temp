#ifndef __UI_H
#define __UI_H
#include "common.h"
#include "stdlib.h"
#include "string.h"

#include "lcd.h"
#include "xpt2046.h"


//����һ����ť�ṹ�壬����λ�ã���С����ɫ�����֣�״̬�ʹ�������
typedef struct {
    u16 x; //��ť���Ͻǵ�x����
    u16 y; //��ť���Ͻǵ�y����
    u16 width; //��ť�Ŀ��
    u16 height; //��ť�ĸ߶�
    u16 color; //��ť����ɫ
    char* text; //��ť����ʾ������
    u8 size; //���ֵĴ�С
    u8 state; //��ť��״̬��0��ʾδ���£�1��ʾ����
		void (*action)(u16 id); //��ť����ʱ�����ĺ���ָ�룬����һ��void*���͵Ĳ���
		u16 id; //�����洢������ַ��ָ��
} Button;


void LCD_DrawButton(Button* btn);
void LCD_CheckButton(Button* btn) ;




//����һ���ṹ�����ͣ������洢��ǩ�������
typedef struct {
  u16 x; //��ǩ�����Ͻǵ�x����
  u16 y; //��ǩ�����Ͻǵ�y����
  u16 width; //��ǩ��Ŀ��
  u16 height; //��ǩ��ĸ߶�
  u16 border_color; //��ǩ��ı߿���ɫ
  u16 fill_color; //��ǩ��������ɫ
  u16 text_color; //��ǩ����ı���ɫ
  u8 text_size; //��ǩ����ı���С
  char *text; //��ǩ����ı�����
} Label;


void LCD_Draw_Label(Label *label);
void LCD_Update_Label(Label *label) ;

void drawWaveform(u16 Xpoint, void *yValues, uint8_t isfloat, u16 Height, u16 y_offset,float Maxvalue, u16 color);


#endif
