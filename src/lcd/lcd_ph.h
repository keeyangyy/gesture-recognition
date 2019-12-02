
#ifndef SRC_LCD_LCD_PH_H_
#define SRC_LCD_LCD_PH_H_

#include "lcd.h"
#include "touch/touch.h"
#include "touch/gt9147.h"

void Lcd_Ph_Init(u8 dir);
u8 LCD_TOUCH_Init(void);

u8 get_sta_touch(void);
void single_touch(void);
void get_touch(void);
void Load_Drow_Dialog(void);
u16 my_abs(u16 x1,u16 x2);

#endif /* SRC_LCD_LCD_PH_H_ */
