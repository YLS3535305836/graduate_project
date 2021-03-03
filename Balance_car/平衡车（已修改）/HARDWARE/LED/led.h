#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED 	PAout(15) 	// PA15
#define LED2 	PBout(3) 		// PB3

void LED_Init(void);  		//≥ı ºªØ
void Led_Flash(u16 time);	//LED1∆µ…¡

#endif
