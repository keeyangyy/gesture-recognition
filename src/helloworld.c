/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdlib.h>
#include <xil_printf.h>
#include "platform.h"

#include "sys/delay.h"
#include "peripher.h"
#include "lcd/lcd_ph.h"
#include "fdc2214.h"
#include "main.h"
#include "gui.h"
#include "matrix.h"
#include "cal.h"

void stepin(void);
void stepwait(void);

//timer interrupt
u8 tim_key, tim_getdata, tim_game;
//status
u8 show_cnt = 0;
u8 mode, gesture_op;
u8 game, game_run;
u8 fin_getdata, fin_calcdata;
u8 gesture_sta;
u16 cnt_gesture;
short int menu;

//touch
u8 key, keysta, last_key;
u16 touch_x, touch_y;

//data
u32 cap_data[8];		//增益GAIN,减去环境量
float cap_change[8];
float cap_fdata[8];
float init_cap[8];
//result
u8 res_number;

//tmp
u8 strtmp[20];
u16 res;
float matrix_tmp_ges[3][3][6], matrix_tmp_num[5][3][6];
u8 train_times_ges[3], train_times_num[5];

Matrix_t X_ges, cap_ges, res_ges;
Matrix_t X_num, cap_num, res_num;
Matrix_t Xges[9], Xnum[9];
Matrix_t Yges[9], Ynum[9];

int main()
{
    init_platform();

    print("Hello World!!!\n\r");
    Peripher_Init();
    Lcd_Ph_Init(1);	//1:横屏 0:竖屏
    print("Initialization is complete!!!\n\r");

    u16 delaytime = 0;
    while(FDC2214_Init(0))
    {
    	delaytime++;
    	if(delaytime > 1000)
    	{
    		printf("fdc2214_init channel 0 err\n");
    		break;
    	}
    }
    delaytime = 0;
    while(FDC2214_Init(1))
    {
    	delaytime++;
    	if(delaytime > 1000)
    	{
    		printf("fdc2214_init channel 1 err\n");
    		break;
    	}
    }

    res_ges = create_mat(COLUMN, 1);
    cap_ges = create_mat(ROW, 1);
    X_ges = create_mat(ROW, COLUMN);
    res_num = create_mat(COLUMN_NUM, 1);
    cap_num = create_mat(ROW, 1);
    X_num = create_mat(ROW, COLUMN_NUM);
    for(u8 i=0; i<3; i++)
    {
    	Xges[i] = create_mat(ROW, COLUMN);
    	Xnum[i] = create_mat(ROW, COLUMN_NUM);
    	Yges[i] = create_mat(COLUMN, 1);
		Ynum[i] = create_mat(COLUMN, 1);
    }

    menu = -1;

    mode = IDLE;
    gui_mode(8);

    check_cap();

    printf("initial 2214! \r\n");

    while(1)
    {
    	//查询按键***********************************************
    	if(tim_key)
    	{
    		key = get_sta_touch();
    		tim_key = 0;
    	}

		//手势操作********************************************************
		if(gesture_op)
		{
			if(gesture_op == RIGHT)
			{
				if(menu < 5)
					menu++;
#ifdef DEBUG
				printf("right\n");
#endif
			}
			else if(gesture_op == LEFT)
			{
				if(menu > 0)
					menu--;
#ifdef DEBUG
				printf("left\n");
#endif
			}
			else if(gesture_op == OK)
			{
				if(menu > -1){
					key = 'A' + menu;
					keysta = 1;
				}

#ifdef DEBUG
				printf("ok\n");
#endif
			}
			gesture_op = IDLE;

			if(menu > -1){
//				LCD_Fill(X_MODEKEY+30, 0, X_MODEKEY+60, 479, BACK_COLOR);
//				gui_arrow(X_MODEKEY+30, Y_MODEKEY/2 + menu*Y_MODEKEY, 10);
//				LCD_Fill(1, Y_MODEKEY*menu+1,X_MODEKEY-1, Y_MODEKEY*(menu+1)-1, LGRAY);
				gui_mode(menu);
			}
		}

    	//绘制GUI,更改相应状态,按键处理******************************
		if(key)
		{

			if(keysta == 1)			//按键为模式按键
			{
				LCD_Fill( X_MODEKEY, 0, 799, 479, BACK_COLOR);
				if(mode==IDLE)
				{
					switch(key)
					{
					case 'A':
						for (u8 i=0; i<6; i++)
							if((i!=0)&&(i!=5))
								LCD_Fill(20, 30+i*Y_MODEKEY, X_MODEKEY-2, 30+24+i*Y_MODEKEY, BACK_COLOR);
						mode = GESTURE;
						LCD_ShowString(20, 30+Y_MODEKEY, 12*4, 24, 24, "game");
						LCD_ShowString(20, 30+2*Y_MODEKEY, 12*5, 24, 24, "again");
						break;
					case 'B':
						for (u8 i=0; i<6; i++)
							if((i!=1)&&(i!=5))
								LCD_Fill(20, 30+i*Y_MODEKEY, X_MODEKEY-2, 30+24+i*Y_MODEKEY, BACK_COLOR);
						mode = NUM;
						break;
					case 'C':
						for (u8 i=0; i<6; i++)
							if((i!=2)&&(i!=5))
								LCD_Fill(20, 30+i*Y_MODEKEY, X_MODEKEY-2, 30+24+i*Y_MODEKEY, BACK_COLOR);
						gui_gesture();
						mode = TRAIN_GESTURE;
						break;
					case 'D':
						for (u8 i=0; i<6; i++)
							if((i!=3)&&(i!=5))
								LCD_Fill(20, 30+i*Y_MODEKEY, X_MODEKEY-2, 30+24+i*Y_MODEKEY, BACK_COLOR);
						gui_num();
						mode = TRAIN_NUM;
						break;
					case 'E':
						delaytime = 0;
					    while(FDC2214_Init(0))
					    {
					    	delaytime++;
					    	if(delaytime > 1000)
					    	{
					    		printf("fdc2214_init channel 0 err\n");
					    		break;
					    	}
					    }
					    delaytime = 0;
					    while(FDC2214_Init(1))
					    {
							delaytime++;
							if(delaytime > 1000)
							{
								printf("fdc2214_init channel 1 err\n");
								break;
							}
					    }
					    gui_mode(8);
						break;
					case 'F':
//						for(u8 i = 0; i < 4; i++)
//							init_cap[i] = Cap_Calculate0(i);
//						for(u8 i = 0; i < 4; i++)
//							init_cap[i+4] = Cap_Calculate1(i);
//						break;
						gui_mode(8);
						check_cap();
					default:
						break;
					}

				}
				else if((mode==GESTURE)||(mode==NUM)||(mode==TRAIN_GESTURE)||(mode==TRAIN_NUM))
				{
					switch(key)
					{
					case 'B':
						if(mode == GESTURE)
						{
							game = 1;		//进入游戏模式
							game_run = 1;	//图像开始变动
							gui_game();
						}
						break;
					case 'C':
						if((mode == GESTURE) && game)
						{
//							LCD_Fill(IMG_X, IMG_Y, IMG_X+150, IMG_Y+150, BACK_COLOR);		//清除图像
//							LCD_Fill(400, 360, 400+12*9, 360+24, BACK_COLOR);				//清除文字
//							game_run = 1;		//图像开始变动
							game = 1;		//进入游戏模式
							game_run = 1;	//图像开始变动
							gui_game();
						}
						break;
					case 'F':
						LCD_Fill( X_MODEKEY, 0, 799, 479, BACK_COLOR);
						gui_mode(8);
						game = 0;			//退出游戏模式
						mode = IDLE;
					default:
						break;
					}
				}
			}
			else if(keysta==2)
			{
				u8 j = 0;
				if(mode==TRAIN_GESTURE)
				{
					u8 cnttmp = key - 'a';
					switch(key)
					{
					case 'a':
					case 'b':
					case 'c':
						if(train_times_ges[cnttmp] == 3)
							break;
						else
						{
							for(u8 i=0; i < 6; i++)
								matrix_tmp_ges[cnttmp][train_times_ges[cnttmp]][i] = cap_fdata[i];
							LCD_ShowNum(2*X_MODEKEY+10, 30+cnttmp*Y_MODEKEY, ++train_times_ges[cnttmp],  12, 24);
							LCD_ShowString(2*X_MODEKEY+25, 30+cnttmp*Y_MODEKEY, 12*5, 24, 24, "times");
						}
						break;
					case 'd':
						break;
					case 'e':
						break;
					case 'f':	//OK
						cal_X(train_times_ges, 3);
						//清零变量
						for(u8 i=0; i<3; i++)
							train_times_ges[i] = 0;
						LCD_Fill( X_MODEKEY, 0, 799, 479, BACK_COLOR);
						gui_mode(8);
						game = 0;			//退出游戏模式
						mode = IDLE;
						break;
					default:
						break;
					}
#ifdef	DEBUG
					MatDump(X_ges);
#endif

				}
				else if(mode == TRAIN_NUM)
				{
					u8 cnttmp = key - 'a';
					switch(key)
					{
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
						if(train_times_num[cnttmp] == 3)
							break;
						else
						{
							for(u8 i=0; i < 6; i++)
								matrix_tmp_num[cnttmp][train_times_num[cnttmp]][i] = cap_fdata[i];
							LCD_ShowNum(2*X_MODEKEY+10, 30+cnttmp*Y_MODEKEY, ++train_times_num[cnttmp],  12, 24);
							LCD_ShowString(2*X_MODEKEY+25, 30+cnttmp*Y_MODEKEY, 12*5, 24, 24, "times");
						}
						break;
					case 'f':	//OK
						cal_X(train_times_num, 5);
						//清零变量
						for(u8 i=0; i<5; i++)
							train_times_num[i] = 0;
						LCD_Fill( X_MODEKEY, 0, 799, 479, BACK_COLOR);
						gui_mode(8);
						mode = IDLE;
						break;
					default:
						break;
					}
#ifdef	DEBUG
					MatDump(X_ges);
#endif
				}
			}

			key = 0;
		}//按键处理结束


		//图标显示************************************************************************
		if(game && tim_game)
		{
			tim_game = 0;


			if(game_run)
			{
				if(++show_cnt>2)
					show_cnt = 0;

				gui_gameimg(show_cnt);
			}
		}

		//fdc2214数据显示***********************************************************************
		if(fin_calcdata)
		{
			fin_calcdata = 0;

//			res_show();
			Y_show();
		}

		//fdc2214数据处理***********************************************************************
		if(fin_getdata)
		{
			fin_getdata = 0;

			if(mode==GESTURE)
			{
				for(u8 i=0; i < 6; i++)
					cap_ges.data[i][0] = cap_fdata[i];

//				res_ges = calc();
				calcY();

				fin_calcdata = 1;
			}
			else if(mode==NUM)
			{
				for(u8 i=0; i < 6; i++)
					cap_num.data[i][0] = cap_fdata[i];

//				res_num = calc();
				calcY();

				fin_calcdata = 1;
			}
		}

		//读取fdc2214数据***********************************************************************
    	if(tim_getdata)
    	{
    		tim_getdata = 0;

			for(u8 i = 0; i < 4; i++)
			{
#ifdef	DEBUG
				cap_data[i] = (Cap_Calculate0(i) - init_cap[i])*GAIN + 100;
#endif
				cap_data[i] = (Cap_Calculate0(i) - init_cap[i])*GAIN + 100;


				cap_change[i] = cap_fdata[i];
				cap_fdata[i] = (Cap_Calculate0(i) - init_cap[i])*GAIN;
				cap_change[i] = cap_fdata[i] - cap_change[i];
			}
			for(u8 i = 0; i < 2; i++)
			{
#ifdef	DEBUG
				cap_data[i+4] = (Cap_Calculate1(i+2) - init_cap[i+4])*GAIN + 100;
#endif
				cap_data[i+4] = (Cap_Calculate1(i+2) - init_cap[i+4])*GAIN + 100;

				cap_change[i+4] = cap_fdata[i+4];
				cap_fdata[i+4] = (Cap_Calculate1(i+2) - init_cap[i+4])*GAIN;
				cap_change[i+4] = cap_fdata[i+4] - cap_change[i+4];
			}


			if(mode == IDLE)
			{
				switch(gesture_sta)
				{
				case 0:				//手势操作入口
					if((cap_change[0] > GATE_GES)&&(cap_change[1] > GATE_GES)&&(cap_change[4] > GATE_GES)&&(cap_change[3] > GATE_GES))
					{
							gesture_op = OK;
							break;
					}
					else{
						if((cap_change[2]>GATE_GES)&&(cap_change[5] < GATE_GES))
							stepin();
						else if((cap_change[5] > GATE_GES)&&(cap_change[2] < GATE_GES))
						{
							gesture_sta = 4;		//跳转下一个状态
							cnt_gesture = 0;
						}
					}
					break;
					//RIGHT
				case 1:
					if(cap_change[2]<-1*GATE_GES)
						stepin();
					else
						stepwait();
				case 2:
					if(cap_change[5]>GATE_GES)
						stepin();
					else
						stepwait();
					break;
				case 3:
					if(cap_change[5]<-1*GATE_GES){
						gesture_op = RIGHT;
						gesture_sta = 0;
					}
					else
						stepwait();
					//LEFT
				case 4:
					if(cap_change[5] < -1*GATE_GES)
						stepin();
					else
						stepwait();
					break;
				case 5:
					if(cap_change[2] > GATE_GES)
						stepin();
					else
						stepwait();
					break;
				case 6:
					if(cap_change[2] < -1*GATE_GES){
						gesture_op = LEFT;
						gesture_sta = 0;
					}
					else
						stepwait();
					break;
				default:
					break;
				}
			}

#ifdef DEBUG
		    for(u8 i = 0; i < 6; i++)				//8通道数据显示
		    	LCD_ShowxNum(650, i*XGRID, cap_data[i], 10, 24, 0);
		    if(mode==TRAIN_GESTURE)
		    	MatDump(X_ges);								//矩阵显示
		    else if(mode==TRAIN_NUM)
		    	MatDump(X_num);								//矩阵显示

#endif
		    for(u8 i = 0; i < 6; i++)				//8通道数据显示
		    	LCD_ShowxNum(650, i*XGRID, cap_data[i], 10, 24, 0);


		    fin_getdata = 1;
    	}


    }

    cleanup_platform();
    return 0;
}

void stepin(void)
{
	gesture_sta++;		//跳转下一个状态
	cnt_gesture = 0;
}
void stepwait(void)
{
	cnt_gesture++;
	if(cnt_gesture > DELAY_GES)
		gesture_sta = 0;	//跳转回默认状态
}
