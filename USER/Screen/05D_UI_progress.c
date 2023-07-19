#include "05D_UI_progress.h"

#include "input_event.h"


void useless_main(void);


uint16_t Screen_flash_cnt=0;

Button btn_start; //����һ����ť�ṹ��

Button btn9[9];
char *texts[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"}; //����һ���ַ�������

uint32_t input_num;

		//����һ��ʾ����ǩ��
Label label1 = {18, 12, 200, 50, BLACK, 0xE73F , BLACK, 24, NULL};

void Button_press_handle(u16);

// ����PD6����ߵ�ƽ
#define PD6_HIGH() GPIOD->BSRRL = GPIO_Pin_6;
// ����PD6����͵�ƽ
#define PD6_LOW() GPIOD->BSRRH = GPIO_Pin_6;
void sendio_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

// ���巢�;��β�����
void send_rect_wave(uint32_t input_num)
{
  uint32_t i;
  for (i = 0; i < 32; i++)
  {
    if (input_num & (1 << (31 - i))) // �жϵ�iλ�Ƿ�Ϊ1
    {
      PD6_HIGH()// ����ߵ�ƽ
    }
    else
    {
      PD6_LOW() // ����͵�ƽ
    }
    delay_us(10); // ��ʱ100us
  }
	PD6_LOW()
}

void Screen_main2_init(void){
		sendio_init();
	
		for (char i = 0; i < 9; i++) { //��������
			btn9[i].x =  (i % 3) * (70 + 10); //����ÿ����ť��x����
			btn9[i].y = 70 + (i / 3) * (40 + 10); //����ÿ����ť��y����
			btn9[i].width = 70; //����ÿ����ť�Ŀ��Ϊ40
			btn9[i].height = 40; //����ÿ����ť�ĸ߶�Ϊ40
			btn9[i].color = 0xAF7D ; //����ÿ����ť����ɫΪ��ɫ
			btn9[i].text = texts[i]; //���ַ��������Ԫ�ظ�ֵ��btn9
			btn9[i].size = 16; //����ÿ����ť�����ִ�СΪ16
			btn9[i].state = 0; //����ÿ����ť�ĳ�ʼ״̬Ϊδ����
			btn9[i].id = i + 1; //����ÿ����ť��idΪ����
			btn9[i].action = Button_press_handle; //����ÿ����ť��actionΪButton_press_handle
			LCD_DrawButton(&btn9[i]); //����ÿ����ť
		}
	

    btn_start.x = 120; //���ð�ť���Ͻǵ�x����Ϊ100
    btn_start.y = 250; //���ð�ť���Ͻǵ�y����Ϊ200
    btn_start.width = 80; //���ð�ť�Ŀ��Ϊ80
    btn_start.height = 40; //���ð�ť�ĸ߶�Ϊ40
    btn_start.color = 0xEC1D ; //���ð�ť����ɫΪ��ɫ
    btn_start.text = "Start"; //���ð�ť����ʾ������Ϊ����ʼ��
    btn_start.size = 16; //�������ֵĴ�СΪ16
    btn_start.state = 0; //���ð�ť�ĳ�ʼ״̬Ϊδ����
		btn_start.id = 10; //����ip
	  btn_start.action = Button_press_handle; //���ð�ť����ʱ�����ĺ���Ϊstart

		LCD_DrawButton(&btn_start); //������ʼ��ť  

		
		
		label1.text = malloc(15); //��text����10���ֽڵĿռ�
		strcpy(label1.text, "Hello Darkarc"); //��ʼ��text
		LCD_Draw_Label(&label1); //������ǩ��
}



void Screen_main2(void)
{
    
	
    Screen_flash_cnt++;
    //delay_ms(1);
    if (Screen_flash_cnt >= 100)
        Screen_flash_cnt = 0;

    char display_str[30];
    BRUSH_COLOR = RED; // ��ʾ��ɫ��Ϊ��ɫ

    if (Screen_flash_cnt == 0)
    {

//        // ����ʱ��Ҫ����tim2ʵ�ֲɼ���������
//        if (TIM2->CR1 == 0)		   // �ȴ����ر�  ˵��FTT�������
//            TIM_Cmd(TIM2, ENABLE); // ʹ�ܶ�ʱ��2
    }
    if (is_show__wave == 1)
    {
        if (Screen_flash_cnt == 0)
        {
            // �����Ļ
            //LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height - 1, 0xffff);
						for (char i = 0; i < 9; i++)
							LCD_CheckButton(&btn9[i]);
						LCD_CheckButton(&btn_start);
            
        }
    }
    else if (is_show__wave == 2)
    {

    }
    else
    {

    }
}



void useless_main(void)
{
}




void Button_press_handle(u16 id){
	if(id!=10){
		input_num=input_num*10+id;
	}else{
		send_rect_wave(input_num);
		input_num=0;
	}
		char str[10];
    sprintf((char *)str, "%d", input_num); // ����������  e-01  ���ǳ���10      /10
		strcpy(label1.text, str);
		LCD_Update_Label(&label1);
}

