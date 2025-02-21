#ifndef DRAW_H
#define DRAW_H

#include "main.h"

void draw_state();
void draw_shot(Shot* shot);
void draw_shots(Shot** current_shots);
void draw_hit(Alien* alien);
void draw_ship(Ship* ship);
void draw_alien(Alien* alien);
void draw_w();
void draw_l();
void draw_end(char finished);

#endif
