/*
 * peripher.h
 *
 *  Created on: 2017��11��3��
 *      Author: WJY
 */

#ifndef SRC_PERIPHER_PERIPHER_H_
#define SRC_PERIPHER_PERIPHER_H_

#include <xil_types.h>

#include "xgpio.h"
#include <xintc.h>
#include <xtmrctr.h>

extern volatile int KeyAction;
extern volatile int KeyValue;
extern XGpio gpio_led;
extern XGpio gpio_touch_rst;
extern XGpio gpio_touch_int;
extern XGpio gpio_touch_sck;
extern XGpio gpio_touch_sda;
extern XGpio gpio_fdc2214_sck0, gpio_fdc2214_sck1;
extern XGpio gpio_fdc2214_sda0, gpio_fdc2214_sda1;

void fdc2214_gpio_Init(void);
void Keys_Init(void);
void Keys_Tick(void);
void leds_Init(void);
void touch_gpio_Init(void);
void Int_Init(void);
void Timer0_Init(void);
void Timer0_ISR(void *arg, u8 num);
void Timer1_Init(void);
void Timer1_ISR(void *arg, u8 num);

void Peripher_Init(void);

#endif /* SRC_PERIPHER_PERIPHER_H_ */
