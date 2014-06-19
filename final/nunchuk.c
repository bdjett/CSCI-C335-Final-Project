/**********************************************
 * nunchuk.c
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Created on: April 18 2014
 * Last updated on: May 1 2014
 * Part of: Final Project
 */

#include "common.h"
#include "nunchuk.h"
#include "draw.h"
#include "image.h"
#include "audio.h"

#define MAX_X 127
#define MAX_Y 135
#define MIN_X 0
#define MIN_Y 0

char score_buff[500];

void check_nunchuk(void) {
	n.z = 0;
	f3d_nunchuk_read(&n);
	//printf("c: %d\n", n.z);
	if (is_hit()) {
		// if crosshair is over targe,t continually redraw target
		displayImage("bird2.bmp", target_x, target_y);
	}
	// continually redraw crosshair
	draw_crosshair(curr_x, curr_y);
	if (n.z == 1) {
		// button pressed
		if (is_hit()) {
			// bird hit
			play("quack.wav");
			score += 1;
			sprintf(score_buff, "Ducks: %d", score);
			f3d_lcd_drawString(5, 146, score_buff, BLACK, 34434);
			erase_target(target_x, target_y);
			f3d_lcd_drawString(target_x, target_y, "  *", RED, 15871);
			delay(200);
			MOVE_TARGET_FLAG = 1;
			if (score >= 5) {
				level_up();
			}
		} else {
		//	delay(500);
		//	play("gun5.wav");
		}
	}
	if (n.jy < 110 && n.jx > 150) {
		erase_crosshair(curr_x, curr_y);
		(++curr_y > MAX_Y) ? curr_y = MAX_Y : curr_y;
		(++curr_x > MAX_X) ? curr_x = MAX_X : curr_x;
		draw_crosshair(curr_x, curr_y);
	} else if (n.jy < 110 && n.jx < 110) {
		erase_crosshair(curr_x, curr_y);
		(++curr_y > MAX_Y) ? curr_y = MAX_Y : curr_y;
		(--curr_x < MIN_X) ? curr_x = MIN_X : curr_x;
		draw_crosshair(curr_x, curr_y);
	} else if (n.jy > 150 && n.jx < 110) {
		erase_crosshair(curr_x, curr_y);
		(--curr_y < MIN_Y) ? curr_y = MIN_Y : curr_y;
		(--curr_x < MIN_X) ? curr_x = MIN_X : curr_x;
		draw_crosshair(curr_x, curr_y);
	} else if (n.jy > 150 && n.jx > 150) {
		erase_crosshair(curr_x, curr_y);
		(--curr_y < MIN_Y) ? curr_y = MIN_Y : curr_y;
		(++curr_x > MAX_X) ? curr_x = MAX_X : curr_x;
		draw_crosshair(curr_x, curr_y);
	} else if (n.jx > 200) {
		erase_crosshair(curr_x, curr_y);
		(++curr_x > MAX_X) ? curr_x = MAX_X : curr_x;
		draw_crosshair(curr_x, curr_y);
	} else if (n.jx < 100) {
		erase_crosshair(curr_x, curr_y);
		(--curr_x < MIN_X) ? curr_x = MIN_X : curr_x;
		draw_crosshair(curr_x, curr_y);
	} else if (n.jy > 200) {
		erase_crosshair(curr_x, curr_y);
		(--curr_y < MIN_Y) ? curr_y = MIN_Y : curr_y;
		draw_crosshair(curr_x, curr_y);
	} else if (n.jy < 100) {
		erase_crosshair(curr_x, curr_y);
		(++curr_y > MAX_Y) ? curr_y = MAX_Y : curr_y;
		draw_crosshair(curr_x, curr_y);
	} 
	//delay(15);
}

// is_hit checks if the corshair is over the target
int is_hit(void) {
	if ((curr_x >= target_x && curr_x < target_x + 24) && (curr_y >= target_y && curr_y < target_y + 17)) {
		return 1;
	} else {
		return 0;
	}
}
