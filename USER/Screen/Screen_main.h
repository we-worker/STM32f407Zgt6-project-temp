#ifndef __Screen_main_H
#define __Screen_main_H
#include "common.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "UI.h"


void Screen_main2_init(void);
void Screen_main2(void);

extern int out_with_sin;

extern char *texts9[9] ;

void Screen_init(void);
void Screen_flash(void);

void Button_press_handle(u16);
extern uint8_t page_id ;
extern uint16_t Screen_flash_cnt;

#endif
