/**********************************************
 * draw.c
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Created on: April 18 2014
 * Last updated on: May 1 2014
 * Part of: Final Project
 */

#include "common.h"
#include "draw.h"
#include "image.h"

// Draw_crosshair takes an x and y coordinate and draws a 2x2 red square
void draw_crosshair(int16_t x, int16_t y) {
	f3d_lcd_fillRect((uint8_t) x, (uint8_t) y, 2, 2, RED);
}

// Erase_crosshair takes an x and y coordinate and fills over the red square
// with the blue background color
void erase_crosshair(int16_t x, int16_t y) {
	f3d_lcd_fillRect((uint8_t) x, (uint8_t) y, 2, 2, 15871);
}

// Draw_target displays the bird target at the given x and y coordinates
void draw_target(uint8_t x, uint8_t y) {
	//f3d_lcd_fillRect(x, y, 17, 25, RED);
	displayImage("bird2.bmp", x, y);
}

// Erase_target takes an x and y coordinate and draws over the bird target
// with a blue background
void erase_target(uint8_t x, uint8_t y) {
	f3d_lcd_fillRect(x, y, 18, 26, 15871);
}
