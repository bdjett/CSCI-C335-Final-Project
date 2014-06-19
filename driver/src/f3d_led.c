/* led.c --- 
 * 
 * Filename: led.c
 * Description: Driver for LED
 * Author: Brian Jett & Adam Combs
 * Maintainer: 
 * Created: Thu Jan 10 10:53:06 2013
 * Last-Updated: Fri Feb 07
 *           By: Brian Jett & Adam Combs
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Code: */
#include <stm32f30x.h>
#include <f3d_led.h>

void f3d_led_init(void) {
	// initialize the port and pins for the 8 leds
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void f3d_led_on(int led) {
	// enable the led specified by led parameter
	// led = 0-7 representing the 8 leds
	switch(led) {
	case 0:
		GPIOE->BSRR = GPIO_Pin_8;
		break;
	case 1:
		GPIOE->BSRR = GPIO_Pin_9;
		break;
	case 2:
		GPIOE->BSRR = GPIO_Pin_10;
		break;
	case 3:
		GPIOE->BSRR = GPIO_Pin_11;
		break;
	case 4:
		GPIOE->BSRR = GPIO_Pin_12;
		break;
	case 5:
		GPIOE->BSRR = GPIO_Pin_13;
		break;
	case 6:
		GPIOE->BSRR = GPIO_Pin_14;
		break;
	case 7:
		GPIOE->BSRR = GPIO_Pin_15;
		break;
	default:
		break;
	}
}
 
void f3d_led_off(int led) {
	// disable the led specified by led parameter
	// led = 0-7 representing the 8 leds
		switch(led) {
	case 0:
		GPIOE->BRR = GPIO_Pin_8;
		break;
	case 1:
		GPIOE->BRR = GPIO_Pin_9;
		break;
	case 2:
		GPIOE->BRR = GPIO_Pin_10;
		break;
	case 3:
		GPIOE->BRR = GPIO_Pin_11;
		break;
	case 4:
		GPIOE->BRR = GPIO_Pin_12;
		break;
	case 5:
		GPIOE->BRR = GPIO_Pin_13;
		break;
	case 6:
		GPIOE->BRR = GPIO_Pin_14;
		break;
	case 7:
		GPIOE->BRR = GPIO_Pin_15;
		break;
	default:
		break;
	}
} 

void f3d_led_all_on(void) {
	// turn on all 8 leds 
	GPIOE->BSRR = GPIO_Pin_All;
} 

void f3d_led_all_off(void) {
	// turn off all 8 leds
	GPIOE->BRR = GPIO_Pin_All;
} 

/* led.c ends here */

