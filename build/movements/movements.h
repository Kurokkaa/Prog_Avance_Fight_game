#ifndef movements_h
#define movements_h

#include "../structure/structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../constante.h"
#include "../graphics/graphics.h"
#include "SDL2/SDL.h"
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "sys/stat.h"
#include "../controller/controller.h"
#include "../lootbox/lootbox.h"
#include "../movements/movements.h"

void movements(jeu *world, sprite_perso *perso, sprite_perso *adversaire);
void change_directions(sprite_perso *p1, sprite_perso *p2);
bool canMove(sprite_perso *perso, sprite_perso *adversaire);
void sprites_collision(sprite_perso * p1, sprite_perso * p2, jeu * world);

#endif