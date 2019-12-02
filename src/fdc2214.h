/*
 * fdc2214.h
 *
 *  Created on: 2018年7月20日
 *      Author: wjy
 */

#ifndef SRC_FDC2214_H_
#define SRC_FDC2214_H_


#include <xil_types.h>
#include <xparameters.h>
#include "peripher.h"
#include "sys/bit.h"

//FPGA移植
#define GPIO_SCK0	HwReg(XPAR_GPIO_2_BASEADDR)
#define GPIO_SCK1 	HwReg(XPAR_GPIO_3_BASEADDR)
#define GPIO_SDA0	HwReg(XPAR_GPIO_4_BASEADDR)
#define GPIO_SDA1 	HwReg(XPAR_GPIO_5_BASEADDR)

////IO方向设置
//#define FDC_SDA_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=8<<4*5;}
//#define FDC_SDA_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=3<<4*5;}
//FPGA移植
#define FDC_SDA0_IN()  XGpio_SetDataDirection(&gpio_fdc2214_sda0, 1, 1)
#define FDC_SDA0_OUT() XGpio_SetDataDirection(&gpio_fdc2214_sda0, 1, 0)
#define FDC_SDA1_IN()  XGpio_SetDataDirection(&gpio_fdc2214_sda1, 1, 1)
#define FDC_SDA1_OUT() XGpio_SetDataDirection(&gpio_fdc2214_sda1, 1, 0)


////IO操作函数
//#define FDC_IIC_SCL    PCout(4) 	//SCL
//#define FDC_IIC_SDA    PCout(5)   //输出SDA
//#define FDC_READ_SDA   PCin(5) 		//输入SDA
//FPGA移植
#define FDC_IIC_SCL0    GPIO_SCK0 		//SCL
#define FDC_IIC_SDA0    GPIO_SDA0			//输出SDA
#define FDC_READ_SDA0   GPIO_SDA0 			//输入SDA
#define FDC_IIC_SCL1    GPIO_SCK1 		//SCL
#define FDC_IIC_SDA1    GPIO_SDA1   		//输出SDA
#define FDC_READ_SDA1	 GPIO_SDA1			//输入SDA


////IO方向设置
//
//#define FDC_SDA_IN()  {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=(u32)8<<0;}
//#define FDC_SDA_OUT() {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=(u32)3<<0;}

////IO操作函数
//#define FDC_IIC_SCL    PBout(1) //SCL
//#define FDC_IIC_SDA    PBout(0) //SDA
//#define FDC_READ_SDA   PBin(0)  //输入SDA


/*FDC2214    iic从地址
 *ADDR = L , I2C Address = 0x2A
 *ADDR = H , I2C Address = 0x2B*/
#define FDC2214_ADDR 0x2A

/*FDC2214各个寄存器地址*/
#define DATA_CH0 0x00     				//数据寄存器
#define DATA_LSB_CH0 0x01
#define DATA_CH1 0x02
#define DATA_LSB_CH1 0x03
#define DATA_CH2 0x04
#define DATA_LSB_CH2 0x05
#define DATA_CH3 0x06
#define DATA_LSB_CH3 0x07
#define RCOUNT_CH0 0x08    //
#define RCOUNT_CH1 0x09
#define RCOUNT_CH2 0x0A
#define RCOUNT_CH3 0x0B
//#define OFFSET_CH0 0x0C  //FDC2114
//#define OFFSET_CH1 0x0D
//#define OFFSET_CH2 0x0E
//#define OFFSET_CH3 0x0F
#define SETTLECOUNT_CH0 0x10
#define SETTLECOUNT_CH1 0x11
#define SETTLECOUNT_CH2 0x12
#define SETTLECOUNT_CH3 0x13
#define CLOCK_DIVIDERS_C_CH0 0x14       //时钟分频
#define CLOCK_DIVIDERS_C_CH1 0x15
#define CLOCK_DIVIDERS_C_CH2 0x16
#define CLOCK_DIVIDERS_C_CH3 0x17
#define STATUS 0x18                     //状态寄存器
#define ERROR_CONFIG 0x19 				//错误报告设置
#define CONFIG 0x1A
#define MUX_CONFIG 0x1B
#define RESET_DEV 0x1C
#define DRIVE_CURRENT_CH0 0x1E          //电流驱动
#define DRIVE_CURRENT_CH1 0x1F
#define DRIVE_CURRENT_CH2 0x20
#define DRIVE_CURRENT_CH3 0x21
#define MANUFACTURER_ID 0x7E      //读取值：0x5449
#define DEVICE_ID 0x7F            //读取值：0x3055

//extern u16 Data_FDC;

//相关函数申明
u8 Set_FDC22140(u8 reg,u8 MSB,u8 LSB);
 u8 Set_FDC22141(u8 reg,u8 MSB,u8 LSB);

u16 FDC_Read0(u8 reg);
u16 FDC_Read1(u8 reg);

//u16 FCD2214_ReadCH(u8 index);
u32 FCD2214_ReadCH0(u8 index);
u32 FCD2214_ReadCH1(u8 index);
u8 FDC2214_Init(u8 channel);

float Cap_Calculate0(u8 index);
float Cap_Calculate1(u8 index);


#endif /* SRC_FDC2214_H_ */
