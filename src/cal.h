/*
 * cal.h
 *
 *  Created on: 2018年7月21日
 *      Author: wjy
 */

#ifndef SRC_CAL_H_
#define SRC_CAL_H_

#include "main.h"
#include "matrix.h"

Matrix_t calc(void);
void res_show(void);
void check_cap(void);
void cal_X(u8 train_times[], u8 num);
void Y_show(void);

#endif /* SRC_CAL_H_ */
