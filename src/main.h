/*
 * main.h
 *
 *  Created on: 2018年7月13日
 *      Author: wjy
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include <xil_types.h>
#include "matrix.h"

//#define		DEBUG	1

#define		X_MODEKEY	200		//模式按键坐标
#define		Y_MODEKEY	80		//模式按键坐标
#define		XGRID		40		//行距

#define 	IMG_X		400
#define		IMG_Y		240

//状态
#define IDLE	0
#define GESTURE		1
#define NUM		2
#define TRAIN_GESTURE	3
#define TRAIN_NUM		4

//手势操作
#define RIGHT	1
#define LEFT	2
#define OK		3
#define GATE_GES	40
#define DELAY_GES	8
#define VALID	0.1
#define CORRECT 	5

#define L		0.000018f
#define CAP		(33*0.000000000001f)
#define FREQ	40000000
#define GAIN	1000000000000000

#define AVELEN	5

#define GATE	0.15
#define GATEGATE 0.8

#define ROW				6
#define COLUMN			3
#define COLUMN_NUM		5


//timer interrupt
extern u8 tim_key, tim_getdata, tim_game;
//status
extern u8 show_cnt;
extern u8 mode, gesture_op;
extern u8 game, game_run;
extern u8 fin_getdata, fin_calcdata;
extern u8 gesture_sta;
extern u16 cnt_gesture;

//touch
extern u8 key, keysta, last_key;
extern u16 touch_x, touch_y;

extern u8 mode;

//data

extern u32 cap_data[8];		//增益GAIN,减去环境量
extern float init_cap[8];
extern u32 Data_FDC;
//result
extern u8 res_number;

//tmp
extern u8 strtmp[20];
extern u16 res;
extern float matrix_tmp_ges[3][3][6], matrix_tmp_num[5][3][6];

extern Matrix_t X_ges, cap_ges, res_ges;
extern Matrix_t X_num, cap_num, res_num;
extern Matrix_t tmp0, tmp1, trans;
extern Matrix_t Xges[9], Xnum[9];
extern Matrix_t Yges[9], Ynum[9];

#endif /* SRC_MAIN_H_ */
