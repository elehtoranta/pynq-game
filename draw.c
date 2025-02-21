#include "draw.h"
#include "Pixel.h"

#include <stdint.h>

void draw_state()
{
	for (uint8_t y = 1; y <= 6; ++y) {
		if (((float)ship.ammo / (float)AMMO) * 6 >= y) {
			SetPixel(7, y, 255, 0, 0);
		}
		if (((float)alien.hp / (float)ALIEN_HP) * 6 >= y) {
			SetPixel(0, y, 0, 255, 100);
		}
	}
}

void draw_shot(Shot* shot)
{
	SetPixel(shot->x, shot->y, 255, 77, 0);
}

void draw_shots(Shot** current_shots)
{
	for (uint8_t i = 0; i < CURRENT_SHOTS_MAX; ++i) {
		if (current_shots[i] == NULL) {
			continue;
		}
		draw_shot(current_shots[i]);
	}
}

// Drawing a vertical line that flashes on hit
void draw_hit(Alien* alien)
{
	for (uint8_t x = 0; x <= 7; ++x) {
		SetPixel(x, 0, 255, 255, 255);
	}
}

void draw_ship(Ship* ship)
{
	SetPixel(ship->x-1, 7, 0, 0, 255);
	SetPixel(ship->x, 7, 0, 0, 255);
	SetPixel(ship->x, 6, 0, 0, 255);
	SetPixel(ship->x+1, 7, 0, 0, 255);
}

void draw_alien(Alien* alien) {
	SetPixel(alien->x, 0, 0, 255, 0);
}

// Draws a W for WINNER
void draw_w()
{
	SetPixel(1, 2, 255, 255, 255);
	SetPixel(1, 3, 255, 255, 255);
	SetPixel(2, 4, 255, 255, 255);
	SetPixel(3, 3, 255, 255, 255);
	SetPixel(3, 2, 255, 255, 255);
	SetPixel(4, 4, 255, 255, 255);
	SetPixel(5, 3, 255, 255, 255);
	SetPixel(5, 2, 255, 255, 255);
}

// Draws an L for LOSER
void draw_l()
{
	SetPixel(2, 2, 255, 255, 255);
	SetPixel(2, 3, 255, 255, 255);
	SetPixel(2, 4, 255, 255, 255);
	SetPixel(3, 4, 255, 255, 255);
	SetPixel(4, 4, 255, 255, 255);
}

void draw_end(char finished) {
	if (finished == WIN) {
		draw_w();
	} else if (finished == LOSE) {
		draw_l();
	}
}
