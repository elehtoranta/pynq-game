/*
 * Pixel.c
 *
 *  Created on: 4.11.2024
 *      Author: Erkka Lehtoranta
 */

#include "Pixel.h"
/* FIXME remove in prod, comes from Pixel.h */
#include <stdint.h>
#include <unistd.h>

#define REG_BITWIDTH 8

#define CONTROL (*(uint32_t *) 0x41220008)
#define CHANNEL (*(uint32_t *) 0x41220000)
#define INPUT (*(uint32_t *) 0xE000A068)
#define LED (*(uint32_t *) 0x41200000)
#define RGB (*(uint32_t *) 0x41240000)


//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};


enum Colors
{
	RED,
	GREEN,
	BLUE,
};

//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{

	//TODO: invert Y-axis quite easily with 7-y
	dots[x][7-y][RED] = r;
	dots[x][7-y][GREEN] = g;
	dots[x][7-y][BLUE] = b;
}


//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch()
{
	CONTROL |= 0x02;
	CONTROL &= ~0x02;
}


//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
// FIXME Produces weird colors when there should be no colors at all :DD
void run(uint8_t x)
{
	uint8_t color_temp;
	/* Ensure LAT is low (0) */
	CONTROL &= ~0x02;

	//Write code that writes data to led matrix driver (8-bit data). Use values from dots array
	//Hint: use nested loops (loops inside loops)
	//Hint2: loop iterations are 8,3,8 (pixels,color,8-bitdata)
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = BLUE; j >= RED; --j) {
			color_temp = dots[x][i][j];
			for (uint8_t bit = 0; bit < 8; ++bit) {
				if ((color_temp & 0x80) == 1) {
					CONTROL |= 0x80;
				} else {
					CONTROL &= ~0x80;
				}
				color_temp <<= 1;
				CONTROL |= 0x2;
				CONTROL &= ~0x2;
			}
		}
	}
	latch();
}

//Set one line (channel) as active, one at a time.
void open_line(uint8_t x)
{
	/* Why not: */
//	if (0 <= x && x <= 7) {
//		CHANNEL |= 1<<x;
//	} else {
//		CHANNEL &= 0x0;
//	}
	/* Performance? */

	/* Instead of: */
	switch (x) {
		case 0: {
			CHANNEL |= 0x01;
			break;
		}
		case 1: {
			CHANNEL |= 0x02;
			break;
		}
		case 2: {
			CHANNEL |= 0x04;
			break;
		}
		case 3: {
			CHANNEL |= 0x08;
			break;
		}
		case 4: {
			CHANNEL |= 0x10;
			break;
		}
		case 5: {
			CHANNEL |= 0x20;
			break;
		}
		case 6: {
			CHANNEL |= 0x40;
			break;
		}
		case 7: {
			CHANNEL |= 0x80;
			break;
		}
		default: {
			CHANNEL &= 0x0;
		}
	}
}

// Here the setup operations for the LED matrix will be performed
void setup()
{

	/* Initialize */
	CONTROL = 0x0;
	CHANNEL = 0x0;

	/* Toggle rst 3 times, leave it at value 1 since it's active low */
	for (int i = 0; i < 3; ++i) {
		CONTROL ^= 0x01;
		usleep(500); /* Last call is redundant but whatever */
	}

	/* Set sda data to 1 */
	CONTROL |= 0x10;

	/* Fill LED driver for (max) brightness. */
	for (uint8_t i = 0; i < 144; ++i) {
		/* Toggle (tick) CLK on and off */
		CONTROL &= ~0x08;
		CONTROL |= 0x08;
	}

	/* Set sb to write to serial data */
	CONTROL |= 0x02;
}
