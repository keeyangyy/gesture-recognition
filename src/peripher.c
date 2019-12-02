/*
 * peripher.c
 *
 *  Created on: 2017��11��3��
 *      Author: WJY
 */

#include <xparameters.h>

#include "peripher.h"
#include "main.h"

void Peripher_Init(void)
{
	fdc2214_gpio_Init();
	Keys_Init();
	leds_Init();
	touch_gpio_Init();
	Int_Init();
	Timer0_Init();
	Timer1_Init();
}



//gpio_fdc2214--------------------------------------
XGpio gpio_fdc2214_sck0, gpio_fdc2214_sck1;
XGpio gpio_fdc2214_sda0, gpio_fdc2214_sda1;
void fdc2214_gpio_Init(void)
{
    XGpio_Initialize (&gpio_fdc2214_sck0, XPAR_HIER_PERIPH_HIER_FCD2214_GPIO_SCK0_DEVICE_ID);
    XGpio_DiscreteWrite (&gpio_fdc2214_sck0, 1, 1);

    XGpio_Initialize (&gpio_fdc2214_sck1, XPAR_HIER_PERIPH_HIER_FCD2214_GPIO_SCK1_DEVICE_ID);
    XGpio_DiscreteWrite (&gpio_fdc2214_sck1, 1, 1);

    XGpio_Initialize (&gpio_fdc2214_sda0, XPAR_HIER_PERIPH_HIER_FCD2214_GPIO_SDA0_DEVICE_ID);
    XGpio_DiscreteWrite (&gpio_fdc2214_sda0, 1, 1);

    XGpio_Initialize (&gpio_fdc2214_sda1, XPAR_HIER_PERIPH_HIER_FCD2214_GPIO_SDA1_DEVICE_ID);
    XGpio_DiscreteWrite (&gpio_fdc2214_sda1, 1, 1);
}

//gpio_keys-----------------------------------------
XGpio gpio_key;
volatile int KeyAction;
volatile int KeyValue;

void Keys_Init(void)
{
	XGpio_Initialize(&gpio_key, XPAR_HIER_PERIPH_GPIO_KEYS_DEVICE_ID);
}
void Keys_Tick(void)
{
	static u8 KeyLast = 0;
	KeyValue = XGpio_DiscreteRead(&gpio_key, 1);
	if (KeyValue != KeyLast)
		KeyAction = 1;
	KeyLast = KeyValue;
}


//gpio_leds-------------------------------------------
XGpio gpio_led;

void leds_Init(void)
{
    XGpio_Initialize (&gpio_led, XPAR_HIER_PERIPH_GPIO_LEDS_DEVICE_ID);
    XGpio_DiscreteWrite (&gpio_led, 1, 0);
}


//gpio_touch--------------------------------------------
XGpio gpio_touch_rst;
XGpio gpio_touch_int;
XGpio gpio_touch_sck;
XGpio gpio_touch_sda;

void touch_gpio_Init(void)
{
    XGpio_Initialize (&gpio_touch_rst, XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_RST_DEVICE_ID);
    XGpio_Initialize (&gpio_touch_int, XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_INT_DEVICE_ID);
    XGpio_Initialize (&gpio_touch_sck, XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_SCK_DEVICE_ID);
    XGpio_Initialize (&gpio_touch_sda, XPAR_HIER_PERIPH_HIER_LCD_GPIO_TOUCH_SDA_DEVICE_ID);
    XGpio_DiscreteWrite (&gpio_touch_rst, 1, 1);
    XGpio_DiscreteWrite (&gpio_touch_int, 1, 1);
    XGpio_DiscreteWrite (&gpio_touch_sck, 1, 1);
    XGpio_DiscreteWrite (&gpio_touch_sda, 1, 1);
}


//interrupt-------------------------------------------
XIntc intc;

void Int_Init(void)
{
	XIntc_Initialize(&intc, XPAR_HIER_PERIPH_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	XIntc_Start(&intc, XIN_REAL_MODE);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(
		XIL_EXCEPTION_ID_INT,
		(Xil_ExceptionHandler)XIntc_InterruptHandler,
		&intc
	);
	Xil_ExceptionEnable();

	XIntc_Start(&intc, 0);
}

////timer------------------------------------------------
XTmrCtr tim0, tim1;
//timer0

void Timer0_ISR(void *arg, u8 num)
{
	if(num==0)
	{
		tim_key = 1;
	}
	else if(num==1)
	{
		tim_getdata = 1;
	}

}
void Timer0_Init(void)
{
	XTmrCtr_Initialize(&tim0, XPAR_HIER_PERIPH_AXI_TIMER_0_DEVICE_ID);
	//channel 0
	XTmrCtr_SetResetValue(&tim0, 0, XPAR_TMRCTR_0_CLOCK_FREQ_HZ / 100);	//定时器100Hz,10ms
	XTmrCtr_SetOptions(&tim0, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION);
	//channel 1
	XTmrCtr_SetResetValue(&tim0, 1, XPAR_TMRCTR_0_CLOCK_FREQ_HZ / 40);	//定时器40Hz 25ms
	XTmrCtr_SetOptions(&tim0, 1, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION);

	XTmrCtr_SetHandler(&tim0, Timer0_ISR, (void *)&tim0);

	XIntc_Connect(&intc, XPAR_INTC_0_TMRCTR_0_VEC_ID, XTmrCtr_InterruptHandler, (void *)&tim0);
	XIntc_Enable(&intc, XPAR_INTC_0_TMRCTR_0_VEC_ID);
	XIntc_Acknowledge(&intc, XPAR_INTC_0_TMRCTR_0_VEC_ID);

	XTmrCtr_Start(&tim0, 0);
	XTmrCtr_Start(&tim0, 1);
}
//timer1
void Timer1_ISR(void *arg, u8 num)
{
	if(num==0)
	{
		tim_game = 1;
	}
	else if(num==1)
	{

	}
}
void Timer1_Init(void)
{
	XTmrCtr_Initialize(&tim1, XPAR_HIER_PERIPH_AXI_TIMER_1_DEVICE_ID);
	//channel 0
	XTmrCtr_SetResetValue(&tim1, 0, XPAR_TMRCTR_0_CLOCK_FREQ_HZ / 10);	//定时器50Hz,100ms
	XTmrCtr_SetOptions(&tim1, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION);
	//channel 1
	XTmrCtr_SetResetValue(&tim1, 1, XPAR_TMRCTR_0_CLOCK_FREQ_HZ / 1000 - 1);	//定时器1kHz
	XTmrCtr_SetOptions(&tim1, 1, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION);
	XTmrCtr_SetHandler(&tim1, Timer1_ISR, (void *)&tim1);

	XIntc_Connect(&intc, XPAR_INTC_0_TMRCTR_1_VEC_ID, XTmrCtr_InterruptHandler, (void *)&tim1);
	XIntc_Enable(&intc, XPAR_INTC_0_TMRCTR_1_VEC_ID);
	XIntc_Acknowledge(&intc, XPAR_INTC_0_TMRCTR_1_VEC_ID);

	XTmrCtr_Start(&tim1, 0);
//	XTmrCtr_Start(&tim1, 1);
}
