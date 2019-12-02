/*
 * cal.c
 *
 *  Created on: 2018年7月21日
 *      Author: wjy
 */

#include "cal.h"
#include "main.h"
#include "lcd/lcd.h"
#include "fdc2214.h"
#include "gui.h"


Matrix_t calc(void)
{
	Matrix_t tmp0, tmp1, trans;
	Matrix_t X, cap, res;
	if(mode==GESTURE)
	{
		X = X_ges;
		cap = cap_ges;
	}
	else if(mode==NUM)
	{
		X = X_num;
		cap = cap_num;
	}

	trans = transpose_mat(&X);
//	MatDump(trans);
	tmp0 = mult_mat(&trans, &X);
//	MatDump(tmp);
	tmp1 = inverse_mat(&tmp0);
	free_mat(&tmp0);
//	MatDump(tmp);
	tmp0 = mult_mat(&tmp1,&trans);
	free_mat(&tmp1);
	free_mat(&trans);
//	MatDump(tmp);
	tmp1 = mult_mat(&tmp0, &cap);
	free_mat(&tmp0);
//	MatDump(tmp);

	res = tmp1;
	return res;
}

void res_show(void)
{

	u8 max = 0;
	float maxval = 0;
	float maxval_delay = 0;

	if(mode == GESTURE)
	{
		static u8 maxptr_ges = 0;		//max指针
//		static u8 max_delay_ges[16];
		static u8 maxval_tmp = 0;
		for(u8 i = 0; i < COLUMN; i++)		//估算手势
		{
			if(maxval < res_ges.data[i][0])
			{
				max = i;
				maxval_delay = maxval;
				maxval = res_ges.data[i][0];
			}
#ifdef DEBUG
			sprintf(strtmp, "%3.4f", res_ges.data[i][0]);
			LCD_ShowString(550, i*XGRID, 12*6, 24, 24, strtmp);
#endif
		}
		if((maxval - maxval_delay) > VALID)			//判断数据是否有效
		{
//			max_delay_ges[maxptr_ges++] = max;
			if(max == maxval_tmp)
			{
				maxptr_ges++;
				if(maxptr_ges > CORRECT)			//指针轮回
					maxptr_ges = 0;
			}
			else
			{
				maxval_tmp = max;
				maxptr_ges = 0;
			}


			if(maxval < GATE)
			{
				if(!game)
				{
					LCD_Fill(IMG_X, IMG_Y, IMG_X+150, IMG_Y+150, BACK_COLOR);		//清除图像
					LCD_Fill(400, 360, 400+12*9, 360+24, BACK_COLOR);				//清除文字
//					game_run = 1;		//图像开始变动
				}
			}
			else
			{
				//多次数据采集,提高稳定度
//				u8 tmp = max_delay_ges[0];
				u8 correct = 0;
				if(maxptr_ges == CORRECT)			//15次稳定数据触发一次显示
					correct = 1;
//					for(u8 i=1; i < 15; i++)
//						if(max_delay_ges[i] == tmp)
//							correct = 1;
////	debug
//				printf("\n%d ges result:\n", maxptr_ges);
//				MatDump(res_ges);
////	debug
				if((correct & (~game)) || game_run)
				{
					game_run = 0;		//图像静止
					switch(max)
					{
					case 0:
						gui_stone();
						if(game)
						{
							if(show_cnt == 0)
								LCD_ShowString(400, 360, 12*9, 24, 24, "again    ");
							else if(show_cnt == 1)
								LCD_ShowString(400, 360, 12*9, 24, 24, "you win!!");
							else if(show_cnt == 2)
								LCD_ShowString(400, 360, 12*9, 24, 24, "you lose!");
						}
						break;
					case 1:
						gui_scissors();
						if(game)
						{
							if(show_cnt == 0)
								LCD_ShowString(400, 360, 12*9, 24, 24, "you lose!");
							else if(show_cnt == 1)
								LCD_ShowString(400, 360, 12*9, 24, 24, "again    ");
							else if(show_cnt == 2)
								LCD_ShowString(400, 360, 12*9, 24, 24, "you win!!");
						}
						break;
					case 2:
						gui_cloth();
						if(game)
						{
							if(show_cnt == 0)
								LCD_ShowString(400, 360, 12*9, 24, 24, "you win!!");
							else if(show_cnt == 1)
								LCD_ShowString(400, 360, 12*9, 24, 24, "you lose!");
							else if(show_cnt == 2)
								LCD_ShowString(400, 360, 12*9, 24, 24, "again    ");
						}
						break;
					default:
						break;
					}
				}
			}
		}
		else
		{
			if(!game)
			{
				LCD_Fill(IMG_X, IMG_Y, IMG_X+150, IMG_Y+150, BACK_COLOR);		//清除图像
				LCD_Fill(400, 360, 400+12*9, 360+24, BACK_COLOR);				//清除文字
			}
		}
	}
	else if(mode == NUM)
	{
		static u8 maxptr_num = 0;		//max指针
//		static u8 max_delay_num[16];
		static u8 maxval_tmp = 0;
		for(u8 i = 0; i < COLUMN_NUM; i++)
		{
			if(maxval < res_num.data[i][0])
			{
				max = i;
				maxval_delay = maxval;
				maxval = res_num.data[i][0];
			}
#ifdef DEBUG
			sprintf(strtmp, "%3.4f", res_num.data[i][0]);
			LCD_ShowString(550, i*XGRID, 12*6, 24, 24, strtmp);
#endif
		}

		if((maxval - maxval_delay) > VALID)
		{
//			max_delay_num[maxptr_num++] = max;

			if(max == maxval_tmp)
			{
				maxptr_num++;
				if(maxptr_num > CORRECT)			//指针轮回
					maxptr_num = 0;
			}
			else
			{
				maxval_tmp = max;
				maxptr_num = 0;
			}

			if(maxval < GATE)
			{
	//			LCD_Fill(400, 10*XGRID, 400+12*1, 10*XGRID+24, BACK_COLOR);
		//		LCD_ShowString(300, 300, 12*5, 24, 24, "none");
				LCD_Fill(350, 200, 428, 414, BACK_COLOR);
			}
			else
			{
				//多次数据采集,提高稳定度
//				u8 tmp = max_delay_num[0];
				u8 correct = 0;
				if(maxptr_num == CORRECT)			//15次稳定数据触发一次显示
					correct = 1;
//				for(u8 i=1; i < 15; i++)
//				{
//					if(max_delay_num[i] == tmp)
//						correct = 1;
//					else
//						correct = 0;
//				}

////	debug
//				printf("\n%d num result:\n", maxptr_num);
//				MatDump(res_num);
////	debug
				if(correct)
				{
					switch(max)
					{
					case 0:
		//				LCD_Fill(400, 10*XGRID, 400+12*5, 10*XGRID+24, BACK_COLOR);
		//				LCD_ShowString(400, 10*XGRID, 12*1, 24, 24, "1");
						show_num2(350, 200, 1);
						break;
					case 1:
		//				LCD_Fill(400, 10*XGRID, 400+12*5, 10*XGRID+24, BACK_COLOR);
		//				LCD_ShowString(400, 10*XGRID, 12*1, 24, 24, "2");
						show_num2(350, 200, 2);
						break;
					case 2:
		//				LCD_Fill(400, 10*XGRID, 400+12*5, 10*XGRID+24, BACK_COLOR);
		//				LCD_ShowString(400, 10*XGRID, 12*1, 24, 24, "3");
						show_num2(350, 200, 3);
						break;
					case 3:
		//				LCD_Fill(400, 10*XGRID, 400+12*5, 10*XGRID+24, BACK_COLOR);
		//				LCD_ShowString(400, 10*XGRID, 12*1, 24, 24, "4");
						show_num2(350, 200, 4);
						break;
					case 4:
		//				LCD_Fill(400, 10*XGRID, 400+12*5, 10*XGRID+24, BACK_COLOR);
		//				LCD_ShowString(400, 10*XGRID, 12*1, 24, 24, "5");
						show_num2(350, 200, 5);
						break;
					default:
						break;
					}
				}
			}
		}
		else
			LCD_Fill(350, 200, 428, 414, BACK_COLOR);
	}


}


void check_cap(void)
{
	u8 k, i;
	float tmp[8];

	for(i = 0; i < 4; i++)
		tmp[i] = Cap_Calculate0(i);
	for(i = 0; i < 2; i++)
		tmp[i+4] = Cap_Calculate1(i+2);
	delay_ms(20);
	for(k=0; k < AVELEN-1; k++)
	{
		for(i = 0; i < 4; i++)
			tmp[i] = tmp[i] + Cap_Calculate0(i);
		for(i = 0; i < 2; i++)
			tmp[i+4] = tmp[i+4] + Cap_Calculate1(i+2);
		delay_ms(20);
	}

	for(i = 0; i < 6; i++)
		init_cap[i] = tmp[i]/AVELEN;
}

void cal_X(u8 train_times[], u8 num)
{
//	u16 cnt = train_times[0];
//	for(u8 i=0; i < num; i++)
//		cnt = cnt * train_times[i];

//	Matrix_t Mat[cnt];
//	if(num==3)
//	{
//		u16 iii;
//		for(u8 i0=0; i0 < train_times[0]; i0++)
//		{
//			for(u8 i1 = 0; i1 < train_times[1]; i1++)
//				for(u8 i2=0; i2 < train_times[2]; i2++)
//				{
//					Mat[iii] = create_mat(6, num);
//					for(u8 raw=0; raw < 6; raw++)
//					{
//						Mat[iii].data[raw][0] = matrix_tmp_ges[0][0][raw];
//					}
//
//					iii++;
//				}
//		}
//	}


	Matrix_t Mat[27];

//	if(num == 3)
//	{
//		for(u8 i=0; i < 3; i++)
//			Mat[i] = create_mat(6, 3);
//		for(u8 i=0; i<3; i++)
//			for(u8 raw=0; raw < 6; raw++)
//				for(u8 col=0; col < num; col++)
//					Mat[i].data[raw][col] = matrix_tmp_ges[col][i][raw];
//	}
	if(num == 3)
	{
		for(u8 i=0; i < 27; i++)
			Mat[i] = create_mat(6, 3);
		for(u8 col=0; col<3; col++)
		{
			for(u8 raw=0; raw < 6; raw++)
			{
				Mat[0].data[raw][col] = matrix_tmp_ges[col][0][raw];
				Mat[1].data[raw][col] = matrix_tmp_ges[col][1][raw];
				Mat[2].data[raw][col] = matrix_tmp_ges[col][2][raw];
				Mat[3].data[raw][col] = matrix_tmp_ges[col][col][raw];
				Mat[4].data[raw][col] = matrix_tmp_ges[col][3-col][raw];
			}
		}
		for(u8 raw=0; raw < 6; raw++)
		{
			Mat[5].data[raw][0] = matrix_tmp_ges[0][0][raw];
			Mat[5].data[raw][1] = matrix_tmp_ges[1][1][raw];
			Mat[5].data[raw][2] = matrix_tmp_ges[2][1][raw];
			Mat[6].data[raw][0] = matrix_tmp_ges[0][1][raw];
			Mat[6].data[raw][1] = matrix_tmp_ges[1][1][raw];
			Mat[6].data[raw][2] = matrix_tmp_ges[2][0][raw];
			Mat[7].data[raw][0] = matrix_tmp_ges[0][1][raw];
			Mat[7].data[raw][1] = matrix_tmp_ges[1][0][raw];
			Mat[7].data[raw][2] = matrix_tmp_ges[2][1][raw];
			Mat[8].data[raw][0] = matrix_tmp_ges[0][0][raw];
			Mat[8].data[raw][1] = matrix_tmp_ges[1][1][raw];
			Mat[8].data[raw][2] = matrix_tmp_ges[2][0][raw];
		}
	}
	else if(num==5)
	{
		for(u8 i=0; i < 3; i++)
			Mat[i] = create_mat(6, 5);
		for(u8 i=0; i<3; i++)
			for(u8 raw=0; raw < 6; raw++)
				for(u8 col=0; col < num; col++)
					Mat[i].data[raw][col] = matrix_tmp_num[col][i][raw];
	}



	Matrix_t tmp0, tmp1, trans;

	for(u8 i=0; i<9; i++)
	{
		trans = transpose_mat(&Mat[i]);
		tmp0 = mult_mat(&trans, &Mat[i]);
		tmp1 = inverse_mat(&tmp0);
		free_mat(&tmp0);
		tmp0 = mult_mat(&tmp1,&trans);
		free_mat(&tmp1);
		free_mat(&trans);

		if(mode==TRAIN_GESTURE)
		{
			free_mat(&Xges[i]);
			Xges[i] = tmp0;
		}
		else if(mode==TRAIN_NUM)
		{
			free_mat(&Xnum[i]);
			Xnum[i] = tmp0;
		}
	}
//	return cnt;
}
void calcY(void)
{
	if(mode == GESTURE)
	{
		for(u8 i=0; i<9; i++)
		{
			free_mat(&Yges[i]);
			Yges[i] = mult_mat(&Xges[i], &cap_ges);
		}
	}

	else if(mode == NUM)
	{
		for(u8 i=0; i<3; i++)
		{
			free_mat(&Ynum[i]);
			Ynum[i] = mult_mat(&Xnum[i], &cap_num);
		}
	}
}

void get_ges(void)
{
	float minval[9];
	u8 min[9];
	u8 valid[9];
	u8 validtmp;
	for(u8 tran=0; tran < 9; tran++)
	{
		if(mode==GESTURE)
		{
			MatDump(Yges[tran]);
			float res[3];
			for(u8 i=0; i < 3; i++)
			{
				res[i] = 0;
				for(u8 row=0; row < 3; row++)
				{
					if(row==i)
						res[i] += (Yges[tran].data[row][0]-1.0f)*(Yges[tran].data[row][0]-1.0f);
					else
						res[i] += Yges[tran].data[row][0]*Yges[tran].data[row][0];
				}
			}
			minval[tran] = res[0];
			for(u8 i=0; i < 3; i++)
			{
				if(minval[tran] >= res[i])
				{
					minval[tran] = res[i];
					min[tran] = i;
				}
			}
			valid[tran] = (res[min[tran]] < GATEGATE) ? 1 : 0;
		}
		else if(mode==NUM)
		{
			float res[5];
			for(u8 i=0; i < 3; i++)
			{
				res[i] = 0;
				for(u8 row=0; row < 5; row++)
				{
					if(row==i)
						res[i] += (Ynum[tran].data[row][tran]-1.0f)*(Ynum[tran].data[row][tran]-1.0f);
					else
						res[i] += Ynum[tran].data[row][tran]*Ynum[tran].data[row][tran];
				}
			}
			minval[tran] = res[0];
			for(u8 i=0; i < 5; i++)
			{
				if(minval[tran] > res[i])
				{
					minval[tran] = res[i];
					min[tran] = i;
				}
			}
			valid[tran] = (res[min[tran]] < GATEGATE) ? 1 : 0;
		}
	}

	res_number = min[0];
	float minvaltmp = minval[0];
	validtmp = valid[0];
	for(u8 tran=0; tran < 9; tran++)
	{
		if(minvaltmp > minval[tran])
		{
			minvaltmp = minval[tran];
			res_number = min[tran];
			validtmp = valid[tran];
		}
	}

	if(!validtmp)
		res_number = 10;
}

void Y_show(void)
{
	get_ges();

	if(mode == GESTURE)
	{
		if(res_number <= 3)
		{
			game_run = 0;		//图像静止
			switch(res_number)
			{
			case 0:
				gui_stone();
				if(game)
				{
					if(show_cnt == 0)
						LCD_ShowString(400, 360, 12*9, 24, 24, "again    ");
					else if(show_cnt == 1)
						LCD_ShowString(400, 360, 12*9, 24, 24, "you win!!");
					else if(show_cnt == 2)
						LCD_ShowString(400, 360, 12*9, 24, 24, "you lose!");
				}
				break;
			case 1:
				gui_scissors();
				if(game)
				{
					if(show_cnt == 0)
						LCD_ShowString(400, 360, 12*9, 24, 24, "you lose!");
					else if(show_cnt == 1)
						LCD_ShowString(400, 360, 12*9, 24, 24, "again    ");
					else if(show_cnt == 2)
						LCD_ShowString(400, 360, 12*9, 24, 24, "you win!!");
				}
				break;
			case 2:
				gui_cloth();
				if(game)
				{
					if(show_cnt == 0)
						LCD_ShowString(400, 360, 12*9, 24, 24, "you win!!");
					else if(show_cnt == 1)
						LCD_ShowString(400, 360, 12*9, 24, 24, "you lose!");
					else if(show_cnt == 2)
						LCD_ShowString(400, 360, 12*9, 24, 24, "again    ");
				}
				break;
			default:
				break;
			}
		}
		else
		{
			if(!game)
			{
				LCD_Fill(IMG_X, IMG_Y, IMG_X+150, IMG_Y+150, BACK_COLOR);		//清除图像
				LCD_Fill(400, 360, 400+12*9, 360+24, BACK_COLOR);				//清除文字
			}
		}
	}
	else if(mode == NUM)
	{
		if(res_number <= 5)
		{

			switch(res_number)
			{
			case 0:
				show_num2(350, 200, 1);
				break;
			case 1:
				show_num2(350, 200, 2);
				break;
			case 2:
				show_num2(350, 200, 3);
				break;
			case 3:
				show_num2(350, 200, 4);
				break;
			case 4:
				show_num2(350, 200, 5);
				break;
			default:
				break;
			}
		}
		else
			LCD_Fill(350, 200, 428, 414, BACK_COLOR);
	}
}
