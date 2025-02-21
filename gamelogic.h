#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include "main.h"

typedef enum {
	LEFT = -1,
	NO_MOVE,
	RIGHT,
} Direction ;

// Moves the player ship one step to the direction.
void move_ship(Direction direction);

// Moves the alien on the X axis. The movement of the alien is
// fully determined by its current position, as it only
// moves end-to-end on its axis with constant speed.
void move_alien(Alien* alien);

// Hit detection. Returns 1 on hit, 0 on miss.
int detect_collision(Alien* alien, Shot* shot);

// Destroys a shot pointed to by p_shot from index i of the shots array.
// NOTE: the caller must ensure that *p_shot is a valid, allocated
// pointer to a shot, or removal from shots[] will crash like hell.
void destroy_shot(Shot** p_shot, uint8_t i);

// Allocates and initializes a shot in front of the ship. Returns
// a pointer to that shot, or NULL if the allocation failed.
Shot* spawn_shot();

// Spawns a new shot with spawn_shot() and adds it to the shot matrix. If
// the shot matrix is full, frees the shot and does nothing.
void shoot();

// Resets the game to its initial state. (See main.h for this state)
void reset_game();

#endif
