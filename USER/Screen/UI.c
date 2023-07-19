#include "UI.h"



//����һ������ť�ĺ��������ݰ�ť�ṹ������Ի��ư�ť
void LCD_DrawButton(Button* btn) {
  //����䰴ť�ı�����ɫ
  LCD_Fill_onecolor(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, btn->color);
  //�ٻ���ť�ı߿򣬸���״̬ѡ��ͬ����ɫ
  if (btn->state == 0) { //δ����ʱ���߿�Ϊ��ɫ
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, WHITE); //�ϱ߿�
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, WHITE); //��߿�
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK); //�ұ߿�
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn->height - 1, BLACK); //�±߿�
  } else { //����ʱ���߿�Ϊ��ɫ
    LCD_DrawLine(btn->x, btn->y, btn->x + btn->width - 1, btn->y, BLACK); //�ϱ߿�
    LCD_DrawLine(btn->x, btn->y, btn->x, btn->y + btn->height - 1, BLACK); //��߿�
    LCD_DrawLine(btn->x + btn->width - 1, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, WHITE); //�ұ߿�
    LCD_DrawLine(btn->x, btn->y + btn->height - 1, btn->x + btn->width - 1, btn->y + btn -> height - 1, WHITE); //�±߿�
  }
  //�����ʾ��ť�ϵ����֣�������ɫΪ��ɫ��������ɫΪ͸��
  LCD_DisplayString_color(btn -> x +(btn -> width -strlen(btn -> text) *(btn -> size /2)) /2 ,btn -> y+(-3*btn -> state) +(btn -> height -btn -> size)/2 ,btn -> size ,btn -> text ,BLACK ,btn->color);
}

//����һ����ⰴť�Ƿ񱻰��µĺ��������ݴ������������ж��Ƿ��ڰ�ť�����ڣ����ı䰴ť״̬�ʹ�������
void LCD_CheckButton(Button* btn) {

  if (Xdown >=btn -> x && Xdown <=btn -> x+btn -> width && Ydown >=btn -> y && Ydown <=btn -> y+btn -> height) { //����ڰ�ť������
    if (btn -> state ==0) { //���֮ǰδ���£���ı�״̬Ϊ���£����ػ水ť
      btn -> state =1;
      LCD_DrawButton(btn);
			
			if (btn -> action !=NULL) { //���´�������
				btn -> action(btn -> id); //����paramָ����Ϊ����
			}
    }
  } else { //������ڰ�ť������
    if (btn -> state ==1) { //���֮ǰ���£���ı�״̬Ϊδ���£����ػ水ť
      btn -> state =0;
      LCD_DrawButton(btn);		
    }
  }
}






//����һ��������������һ����ǩ��
void LCD_Draw_Label(Label *label) {
  //�Ȼ��߿�
  LCD_DrawLine(label->x, label->y, label->x + label->width, label->y, label->border_color); //�ϱ�
  LCD_DrawLine(label->x, label->y + label->height, label->x + label->width, label->y + label->height, label->border_color); //�±�
  LCD_DrawLine(label->x, label->y, label->x, label->y + label->height, label->border_color); //���
  LCD_DrawLine(label->x + label->width, label->y, label->x + label->width, label->y + label->height, label->border_color); //�ұ�

  //�������ɫ
  LCD_Fill_onecolor(label->x + 1, label->y + 1, label->x + label->width - 1, label->y + label->height - 1, label->fill_color);

  //�����ʾ�ı�
  LCD_DisplayString_color(label->x + (label->width - strlen(label->text) * (label->text_size / 2)) / 2, 
                          label->y + (label->height - (label->text_size)) / 2,
                          label->text_size,
                          label->text,
                          label->text_color,
                          label->fill_color);
}



//����һ�������������޸ı�ǩ�������
void LCD_Update_Label(Label *label) {
  //�����ԭ���ı�ǩ��
  LCD_Fill_onecolor(label->x - 1, label->y - 1, label->x + label->width + 1, label->y + label->height + 1, WHITE);

  //������»����µı�ǩ��
  LCD_Draw_Label(label);
}





