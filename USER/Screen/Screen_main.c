#include "Screen_main.h"

#include "05D_UI.h"
#include "Wave_page.h"
#include "Screen3.h"

uint16_t Screen_flash_cnt = 0;

#define page_num 3
Button btn_page[page_num];
uint8_t page_id = 0;

char *texts9[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"}; // 定义一个字符串数组

// 假设所有页面刷新函数和处理函数都有相同的类型和参数
typedef void (*PageFunc)(void);                                                             // 定义一个函数指针类型
PageFunc page_init_funcs[] = {Screen_page0_init, Screen_page1_init, Screen_page3_init};     // 定义一个存储页面处理函数地址的数组
PageFunc page_flash_funcs[] = {Screen_page0_flash, Screen_page1_flash, Screen_page3_flash}; // 定义一个存储页面刷新函数地址的数组
typedef void (*PageHandle)(int);
PageHandle page_handle_funcs[] = {Page0_handle, Page1_handle, Page3_handle};                  // 定义一个存储页面处理函数地址的数组


void Button_press_handle(u16);

void Page_Change(int id)
{
    for (int i = 0; i < page_num; i++)
    {
        if (i == page_id)
        {
            // 页面按钮刷新初始化
            btn_page[page_id].color = 0x3cf2;
        }
        else
        {
            btn_page[i].color = 0x3e47;
        }

        LCD_DrawButton(&btn_page[i]); // 画出每个按钮
    }
    page_init_funcs[page_id]();
    // switch (page_id)
    // {
    // case 0:
    //     Screen_page0_init();
    //     break;
    // case 1:
    //     Screen_page1_init();
    //     break;
    // case 2:
    //     Screen_page3_init();
    //     break;
    // default:
    //     break;
    // }
}

void Screen_init()
{
    LCD_Fill_onecolor(0, 0, lcd_width - 1, lcd_height - 1, 0xffff);
    for (char i = 0; i < page_num; i++)
    {                                                            // 遍历数组
        btn_page[i].x = (i % page_num) * (lcd_width / page_num); // 计算每个按钮的x坐标
        btn_page[i].y = (i / 3) * (30);                          // 计算每个按钮的y坐标
        btn_page[i].width = (lcd_width / page_num);              // 设置每个按钮的宽度为40
        btn_page[i].height = 30;                                 // 设置每个按钮的高度为40
        btn_page[i].color = 0x3e47;                              // 设置每个按钮的颜色为蓝色
        btn_page[i].text = texts9[i];                            // 把字符串数组的元素赋值给btn9
        btn_page[i].size = 16;                                   // 设置每个按钮的文字大小为16
        btn_page[i].state = 0;                                   // 设置每个按钮的初始状态为未按下
        btn_page[i].id = i + 100;                                // 设置每个按钮的id为数字
        btn_page[i].action = Button_press_handle;                // 设置每个按钮的action为Button_press_handle
        LCD_DrawButton(&btn_page[i]);                            // 画出每个按钮
    }
    page_id = 0;
    Page_Change(page_id);
}

void Screen_flash(void)
{
    for (int i = 0; i < page_num; i++)
    {
        LCD_CheckButton(&btn_page[i]);
    }
    page_flash_funcs[page_id]();
    // switch (page_id)
    // {
    // case 0:
    //     Screen_page0_flash();
    //     break;
    // case 1:
    //     Screen_page1_flash();
    //     break;
    // case 2:
    //     Screen_page3_flash();
    //     break;
    // default:
    //     break;
    // }
}

void Button_press_handle(u16 id)
{
    if (100 <= id && id <= 100 + page_num)
    {
        page_id = id - 100;
        Page_Change(page_id);
    }
    else
    {
        page_handle_funcs[page_id](id);
        // switch (page_id)
        // {
        // case 0:
        //     Page0_handle(id);
        //     break;
        // case 1:
        //     Page1_handle(id);
        //     break;
        // case 2:
        //     Page3_handle(id);
        //     break;
        // default:
        //     break;
        // }
    }
}
