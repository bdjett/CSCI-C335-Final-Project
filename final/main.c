/**********************************************
 * main.c
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Created on: April 18 2014
 * Last updated on: May 1 2014
 * Part of: Final Project
 */

#include "common.h"
#include "draw.h"
#include "image.h"
#include "audio.h"
#include "nunchuk.h"

int16_t curr_x; // crosshair x
int16_t curr_y; // crosshair y

uint8_t target_x; // target x
uint8_t target_y; // target y

int score = 0; // score counter

int counter = 0; // counter for moving bird

int level = 1; // keeps track of level

int move_time = 800; // how long until the bird moves automatically

int MOVE_TARGET_FLAG = 0; // flag to check whether the target should be moved

int GAME_OVER_FLAG = 0; // flag is set when game is over

// random_number generates a random number
int random_number(void) {
	return rand() % 100;
}

// move_target erases the current target and draws a new one at a random location
void move_target(void) {
	erase_target(target_x, target_y);

	target_x = random_number();
	target_y = random_number();

	printf("%d, %d\n", target_x, target_y);

	displayImage("bird2.bmp", target_x, target_y);
}

// end_game shows the end game screen and sets the GAME_OVER flag
void end_game(void) {
	GAME_OVER_FLAG = 1;
	erase_target(target_x, target_y);
	erase_crosshair(curr_x, curr_y);
	f3d_lcd_fillScreen(BLACK);
	f3d_lcd_drawString(20, 80, "Congratulations!", WHITE, BLACK);
}

// level_up increases the level and sets the move_time to the correct speed
void level_up(void) {
	level++;
	f3d_lcd_drawString(45, 45, "Level 1", BLACK, 15871); 
	switch (level) {
		case 2:
			score = 0;
			move_time = 600;
			f3d_lcd_drawString(5, 146, "Ducks: 0", BLACK, 34434);
			f3d_lcd_drawString(70, 146, "Level: 2", BLACK, 34434);
			f3d_lcd_drawString(45, 45, "Level 2", BLACK, 15871);
			delay(400);
			f3d_lcd_drawString(45, 45, "       ", BLACK, 15871);
			break;
		case 3:
			score = 0;
			move_time = 400;
			f3d_lcd_drawString(5, 146, "Ducks: 0", BLACK, 34434);
			f3d_lcd_drawString(70, 146, "Level: 3", BLACK, 34434);
			f3d_lcd_drawString(45, 45, "Level 3", BLACK, 15871);
                        delay(400);
                        f3d_lcd_drawString(45, 45, "       ", BLACK, 15871);
			break;
		case 4:
			score = 0;
			move_time = 200;
			f3d_lcd_drawString(5, 146, "Ducks: 0", BLACK, 34434);
                        f3d_lcd_drawString(70, 146, "Level: 4", BLACK, 34434);
			f3d_lcd_drawString(45, 45, "Level 4", BLACK, 15871);
                        delay(400);
                        f3d_lcd_drawString(45, 45, "       ", BLACK, 15871);
			break;
		case 5:
			score = 0;
			move_time = 100;
			f3d_lcd_drawString(5, 146, "Ducks: 0", BLACK, 34434);
                        f3d_lcd_drawString(70, 146, "Level: 5", BLACK, 34434);
			f3d_lcd_drawString(45, 45, "Level 5", BLACK, 15871);
                        delay(400);
                        f3d_lcd_drawString(45, 45, "       ", BLACK, 15871);
			break;			
		case 6:
			end_game();
			break;
		default:
			break;
	}
}

// SysTick_Handler is used with a counter to determine when to move the target
void SysTick_Handler(void) {	
	if (!queue_empty(&txbuf)) {
		flush_uart();
	}
	if (counter >= move_time) {
		MOVE_TARGET_FLAG = 1;
		counter = 0;
	}
	counter++;
}

// init initializes all of the components
void init(void) {
	f3d_uart_init();
	delay(100);
	f3d_timer2_init();
	delay(100);
	f3d_dac_init();
	delay(100);
	f3d_delay_init();
	delay(100);
	f3d_rtc_init();
	delay(100);
	f3d_systick_init(50);
	delay(100);
	f3d_lcd_init();
	delay(100);
	f3d_i2c1_init();
	delay(100);
	f3d_nunchuk_init();
	delay(100);

	srand(get_fattime());

	printf("Reset\n");
}

int main(void) {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	// initialize everything
	init();

	// mount file system
	f_mount(0, &Fatfs);

	// display start screen
	displayImage("start.bmp", 0, -1);
	f3d_lcd_drawString(1, 130, "Pull Trigger To Start", WHITE, BLACK);

	// unless the user presses the button, don't allow game to start
	while(!n.z) {
		f3d_nunchuk_read(&n);
		delay(30);
	}

	// background color
	f3d_lcd_fillScreen(15871);

	// set initial crosshair position
	curr_x = 5;
	curr_y = 5;

	// set initial target position
	target_x = random_number();
	target_y = random_number();

	// draw crosshair
	draw_crosshair(curr_x, curr_y);

	// draw grass for UI
	displayImage("grass.bmp", 0, 137);

	// scoring and level display
	f3d_lcd_drawString(5, 146, "Ducks: 0", BLACK, 34434);
	f3d_lcd_drawString(70, 146, "Level: 1", BLACK, 34434);

	// draw target
	displayImage("bird2.bmp", target_x, target_y);

	n.z = 0;

	// continually check nunchuk while the game is not over
	while (1) {
		if (!GAME_OVER_FLAG) {
			if (MOVE_TARGET_FLAG) {
				move_target();
				MOVE_TARGET_FLAG = 0;
			}
			check_nunchuk();
			delay(15);
		}
	}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
