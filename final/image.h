/**********************************************
 * image.h
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Created on: April 18 2014
 * Last updated on: May 1 2014
 * Part of: Final Project
 */

#define RGB16(red, green, blue) ( ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3) )

extern void displayImage(char *fileName, int x, int y);
extern FATFS Fatfs;
