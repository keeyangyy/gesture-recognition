/*
 * gui.h
 *
 *  Created on: 2018年7月13日
 *      Author: wjy
 */

#ifndef SRC_GUI_H_
#define SRC_GUI_H_

#include <xil_types.h>

void gui_gesture(void);
void gui_num(void);
void gui_mode(u8 choose);
void clc_modekey(void);
void draw_grid(u16 x, u16 y, u8 lenx, u8 leny, u8 numx, u8 numy);
void print_grid(u16 x, u16 y, u8 lenx, u8 leny, u8 numx, u8 numy, u8 size, const u8 str[][20], u8 choose);
void lcd_freq(float freq, u16 x, u16 y);
void gui_arrow(u16 x, u16 y, u8 big);
void gui_stone(void);
void gui_cloth(void);
void gui_scissors(void);
void gui_gameimg(u8 img);
void gui_game(void);
void show_num(u16 xx, u16 yy, u8 num);
void show_num2(u16 xx, u16 yy, u8 num);

#endif /* SRC_GUI_H_ */
