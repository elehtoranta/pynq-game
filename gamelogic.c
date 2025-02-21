#include "gamelogic.h"

// NOTE direction: 1 == right, -1 == left
enum DIRECTION {
	LEFT = -1,
	NO_MOVE,
	RIGHT,
}
void move_ship(int8_t direction)
{
	ship.x += direction;
}

// Moves the alien on the X axis.
void move_alien(Alien* alien) {
	static uint8_t direction = 1;

	if (alien->x == 1) {
		direction = 1;
	} else if (alien->x == 6) {
		direction = -1;
	}

	alien->x += direction;
}

// Hit detection.
int detect_collision(Alien* alien, Shot* shot)
{
	if (shot->x == alien->x && shot->y == 0) {
		alien->hp -= shot->damage;
		draw_hit(alien);
		return 1;
	}
	return 0;
}

// Destroys a shot from the shots array.
// NOTE: *p_shot must be a valid, allocated pointer to a shot or removal attempt
// from shots[] this will crash like hell.
void destroy_shot(Shot** p_shot, uint8_t i)
{
	free(*p_shot);
	current_shots[i] = NULL;
}

// Allocates and initializes a shot in front of the ship.
Shot* spawn_shot()
{
	Shot* newshot = (Shot*) malloc(sizeof(Shot));
	if (!newshot) return NULL;
	newshot->x = ship.x;
	newshot->y = 5;
	newshot->damage = SHOT_DAMAGE;
	return newshot;
}

void shoot()
{
	Shot* newshot = spawn_shot();
	if (!newshot) return;

	for (uint8_t pos = 0; pos < CURRENT_SHOTS_MAX; ++pos) {
		if (current_shots[pos] == NULL) { // Free space
			current_shots[pos] = newshot;
			return;
		}
	}
	// Didn't find a free spot
	free(newshot);

	// Decrementing ship.ammo did not work here so it's handled directly in TickHandler1
}

void reset_game()
{
	// Clean shot buffer
	for (uint8_t n = 0; n < CURRENT_SHOTS_MAX; ++n) {
		if (current_shots[n] != NULL) {
			destroy_shot(&current_shots[n], n);
		}
	}

	ship.ammo = AMMO;
	ship.x = 3;

	alien.x = 1;
	alien.hp = ALIEN_HP;

	finished = 0;
}
