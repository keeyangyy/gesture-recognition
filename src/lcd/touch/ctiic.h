#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H

#include "xil_types.h"
#include "xgpio.h"

#include "../../sys/bit.h"

extern XGpio gpio_touch_rst;
extern XGpio gpio_touch_int;
extern XGpio gpio_touch_sck;
extern XGpio gpio_touch_sda;

#define TOUCH_INT	HwReg(XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_INT_BASEADDR)
#define TOUCH_RST	HwReg(XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_RST_BASEADDR)
#define TOUCH_SCK	HwReg(XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_SCK_BASEADDR)
#define TOUCH_SDA	HwReg(XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_SDA_BASEADDR)

//IO方向设置
#define CT_SCL_IN()   XGpio_SetDataDirection(&gpio_touch_sck, 1, 1)    //SCL输入模式
#define CT_SCL_OUT()  XGpio_SetDataDirection(&gpio_touch_sck, 1, 0)	   //SCL输出模式
#define CT_SDA_IN()   XGpio_SetDataDirection(&gpio_touch_sda, 1, 1)    //SDA输入模式
#define CT_SDA_OUT()  XGpio_SetDataDirection(&gpio_touch_sda, 1, 0)	   //SDA输出模式
#define CT_RST_IN()   XGpio_SetDataDirection(&gpio_touch_rst, 1, 1)    //RST输入模式
#define CT_RST_OUT()  XGpio_SetDataDirection(&gpio_touch_rst, 1, 0)	   //RST输出模式
#define CT_INT_IN()   XGpio_SetDataDirection(&gpio_touch_int, 1, 1)    //INT输入模式
#define CT_INT_OUT()  XGpio_SetDataDirection(&gpio_touch_int, 1, 0)	   //INT输出模式
//IO操作函数
#define CT_IIC_SCL    TOUCH_SCK 	//SCL_GPIO地址
#define CT_IIC_SDA    TOUCH_SDA 	//SDA_GPIO地址
#define CT_READ_SDA   TOUCH_SDA 	//SDA_GPIO地址
 

//IIC所有操作函数
void CT_IIC_Init(void);                	//初始化IIC的IO口
void CT_IIC_Start(void);				//发送IIC开始信号
void CT_IIC_Stop(void);	  				//发送IIC停止信号
void CT_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
u8 CT_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CT_IIC_Ack(void);					//IIC发送ACK信号
void CT_IIC_NAck(void);					//IIC不发送ACK信号

#endif






