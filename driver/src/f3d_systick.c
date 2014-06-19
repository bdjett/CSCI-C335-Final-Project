/*****************************
 * f3d_systick.c
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Created on: 4 April 2014
 * Part of: Lab 11
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_uart.h>

volatile int systick_flag = 0;

void f3d_systick_init(int times) {
	SysTick_Config(SystemCoreClock / times);
}

/* f3d_systick.c ends here */
