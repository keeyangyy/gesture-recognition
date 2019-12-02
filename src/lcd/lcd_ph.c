/*
 * lcd_ph.c
 *
 *  Created on: 2018年2月6日
 *      Author: WJY
 */

#include "lcd_ph.h"
#include "../sys/delay.h"
#include "../peripher.h"
#include "../main.h"


u8 key_table1[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
u8 key_table2[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
//u8 key_table2[4][4] = {	'7', '8', '9', 'o',
//						'4', '5', '6', 'k',
//						'1', '2', '3', 'm',
//						'b', '0', '.', 'a'};


//5个触控点的颜色(电容触摸屏用)
const u16 POINT_COLOR_TBL[5]={RED,GREEN,BLUE,BROWN,GRED};

//LCD外设初始化
void Lcd_Ph_Init(u8 dir)
{
    LCD_Init(dir);	//1:横屏 0:竖屏
    LCD_TOUCH_Init();
}

//电容触摸屏单点触摸读取函数
//定时器中断进入
//返回值: key值
u8 get_sta_touch(void)
{
	u8 i=0;
	u8 keyval = 0;	//默认为0
	keysta = 0;	//默认为0

	GT9147_Scan(0);
	if(tp_dev.sta&0x01)
	{
		if(tp_dev.x[0]<lcddev.width && tp_dev.y[0]<lcddev.height)
		{
			if(last_key==0)			//单次按键响应
			{
				touch_x = tp_dev.x[0];
				touch_y = tp_dev.y[0];
//				//显示单次触摸坐标
//				LCD_ShowxNum(180, 120, touch_x, 3, 24, 0);
//				LCD_ShowxNum(250, 120, touch_y, 3, 24, 0);
				last_key=1;		//标识本次按键按下


				//处理按键成key值
				if(touch_x < X_MODEKEY)
				{
					u8 temp = touch_y/80;
					keyval = key_table1[temp];
					keysta = 1;
				}
				else if(touch_x < 2*X_MODEKEY)
				{
					u8 temp = touch_y/80;
					keyval = key_table2[temp];
					keysta = 2;
				}
//				else if((touch_x > 480)&&(touch_y > 160))
//				{
//					u8 tempx = (touch_x-480)/80;
//					u8 tempy = (touch_y-160)/80;
//					keyval = key_table2[tempy][tempx];
//					keysta = 2;
//				}
			}
		}
	}
	else
		last_key=0;

//	//显示实时坐标
//	LCD_ShowxNum(180, 300, tp_dev.x[0], 5, 24, 0);
//	LCD_ShowxNum(250, 300, tp_dev.y[0], 5, 24, 0);

	return keyval;
}


//电容触摸屏单点触摸读取函数
void single_touch(void)
{
	u8 i=0;
 	u16 lastpos[2];		//最后一次的数据

 	u8 last_key = 0;
 	u16 touch_x, touch_y;

 	u32 led = 0;

	while(1)
	{
		GT9147_Scan(0);
//		GT9147_Scan_Single(0);
		if(tp_dev.sta&0x01)
		{
			if(tp_dev.x[0]<lcddev.width && tp_dev.y[0]<lcddev.height)
			{
				if(last_key==0)			//单次按键响应
				{
					touch_x = tp_dev.x[0];
					touch_y = tp_dev.y[0];
					//显示单次触摸坐标
					LCD_ShowxNum(20, 50, touch_x, 3, 24, 0);
					LCD_ShowxNum(100, 50, touch_y, 3, 24, 0);
					last_key=1;
				}

				if(lastpos[0]==0XFFFF)
				{
					lastpos[0] = tp_dev.x[0];
					lastpos[1] = tp_dev.y[0];
				}
				lcd_draw_bline(lastpos[0],lastpos[1],tp_dev.x[0],tp_dev.y[0],5, POINT_COLOR);//画线
				lastpos[0]=tp_dev.x[0];
				lastpos[1]=tp_dev.y[0];
				//处理按键
				if(tp_dev.x[0]>(lcddev.width-24) && tp_dev.y[0]<20)
				{
					Load_Drow_Dialog();//清除
				}
			}
		}
		else
		{
			lastpos[0]=0XFFFF;
			last_key=0;
		}


		//显示实时坐标
		LCD_ShowxNum(20, 20, tp_dev.x[0], 5, 24, 0);
		LCD_ShowxNum(100, 20, tp_dev.y[0], 5, 24, 0);

		delay_ms(5);
		i++;
		if(i%100==0)
		{
			i = 0;
			XGpio_DiscreteWrite(&gpio_led, 1, led++);
		}
	}
}


//电容触摸屏测试函数
void get_touch(void)
{
	u8 t=0;
	u8 i=0;
 	u16 lastpos[5][2];		//最后一次的数据

 	u32 led=0;

	while(1)
	{
		GT9147_Scan(0);
		for(t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				if(tp_dev.x[t]<lcddev.width && tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],5,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24) && tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		//显示实时坐标
		LCD_ShowxNum(20, 20, tp_dev.x[0], 5, 24, 0);
		LCD_ShowxNum(100, 20, tp_dev.y[0], 5, 24, 0);

		delay_ms(5);
		i++;
		if(i%100==0)
		{
			i = 0;
			XGpio_DiscreteWrite(&gpio_led, 1, led++);
		}
	}
}


//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(BACK_COLOR);//清屏
 	POINT_COLOR = BLUE;//设置字体为蓝色
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR = RED;//设置画笔蓝色
}

//两个数之差的绝对值
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{
	if(x1>x2)	return x1-x2;
	else 		return x2-x1;
}
