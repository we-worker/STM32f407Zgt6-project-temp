#include "05D_UI_progress.h"

#include "input_event.h"


void useless_main(void);


uint16_t Screen_flash_cnt=0;

Button btn_start; //定义一个按钮结构体

Button btn9[9];
char *texts[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"}; //定义一个字符串数组

uint32_t input_num;

		//定义一个示例标签框
Label label1 = {18, 12, 200, 50, BLACK, 0xE73F , BLACK, 24, NULL};

void Button_press_handle(u16);

// 定义PD6输出高电平
#define PD6_HIGH() GPIOD->BSRRL = GPIO_Pin_6;
// 定义PD6输出低电平
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

// 定义发送矩形波函数
void send_rect_wave(uint32_t input_num)
{
  uint32_t i;
  for (i = 0; i < 32; i++)
  {
    if (input_num & (1 << (31 - i))) // 判断第i位是否为1
    {
      PD6_HIGH()// 输出高电平
    }
    else
    {
      PD6_LOW() // 输出低电平
    }
    delay_us(10); // 延时100us
  }
	PD6_LOW()
}

void Screen_main2_init(void){
		sendio_init();
	
		for (char i = 0; i < 9; i++) { //遍历数组
			btn9[i].x =  (i % 3) * (70 + 10); //计算每个按钮的x坐标
			btn9[i].y = 70 + (i / 3) * (40 + 10); //计算每个按钮的y坐标
			btn9[i].width = 70; //设置每个按钮的宽度为40
			btn9[i].height = 40; //设置每个按钮的高度为40
			btn9[i].color = 0xAF7D ; //设置每个按钮的颜色为蓝色
			btn9[i].text = texts[i]; //把字符串数组的元素赋值给btn9
			btn9[i].size = 16; //设置每个按钮的文字大小为16
			btn9[i].state = 0; //设置每个按钮的初始状态为未按下
			btn9[i].id = i + 1; //设置每个按钮的id为数字
			btn9[i].action = Button_press_handle; //设置每个按钮的action为Button_press_handle
			LCD_DrawButton(&btn9[i]); //画出每个按钮
		}
	

    btn_start.x = 120; //设置按钮左上角的x坐标为100
    btn_start.y = 250; //设置按钮左上角的y坐标为200
    btn_start.width = 80; //设置按钮的宽度为80
    btn_start.height = 40; //设置按钮的高度为40
    btn_start.color = 0xEC1D ; //设置按钮的颜色为红色
    btn_start.text = "Start"; //设置按钮上显示的文字为“开始”
    btn_start.size = 16; //设置文字的大小为16
    btn_start.state = 0; //设置按钮的初始状态为未按下
		btn_start.id = 10; //按键ip
	  btn_start.action = Button_press_handle; //设置按钮按下时触发的函数为start

		LCD_DrawButton(&btn_start); //画出开始按钮  

		
		
		label1.text = malloc(15); //给text分配10个字节的空间
		strcpy(label1.text, "Hello Darkarc"); //初始化text
		LCD_Draw_Label(&label1); //画出标签框
}



void Screen_main2(void)
{
    
	
    Screen_flash_cnt++;
    //delay_ms(1);
    if (Screen_flash_cnt >= 100)
        Screen_flash_cnt = 0;

    char display_str[30];
    BRUSH_COLOR = RED; // 显示颜色变为红色

    if (Screen_flash_cnt == 0)
    {

//        // 高速时需要开关tim2实现采集，不卡顿
//        if (TIM2->CR1 == 0)		   // 等待被关闭  说明FTT计算完毕
//            TIM_Cmd(TIM2, ENABLE); // 使能定时器2
    }
    if (is_show__wave == 1)
    {
        if (Screen_flash_cnt == 0)
        {
            // 清空屏幕
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
    sprintf((char *)str, "%d", input_num); // 浮点型数据  e-01  就是除于10      /10
		strcpy(label1.text, str);
		LCD_Update_Label(&label1);
}

