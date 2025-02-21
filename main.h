#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

// Limits
#define CURRENT_SHOTS_MAX 8*6
#define SHOT_DAMAGE 1
#define SPEED 1
#define AMMO 42
#define ALIEN_HP 10
#define ALIEN_PACE 6

// Gameplay bounds
#define LEFT_X_BOUND 1
#define RIGHT_X_BOUND 6

// End results
typedef enum Result {
    LOSE = 1,
    WIN = 2,
} Result;

typedef struct Shot {
    int8_t	x, y;
    uint8_t	damage;
} Shot;

typedef struct Ship {
    uint8_t x;
    int8_t	ammo;
} Ship;

typedef struct Alien
{
    uint8_t x;
    int8_t hp;
} Alien;

// Refactor to this to encapsulate
//typedef struct GameState {
//	Shot*	shots[8*6];
//	Ship	ship;
//	Alien	alien;
//	uint8_t	score;
//	uint8_t	finished; // used as bool
//} GameState;


// State
extern Shot*	current_shots[8*6];
extern Ship		ship;
extern Alien	alien;
extern char		finished;

#endif
