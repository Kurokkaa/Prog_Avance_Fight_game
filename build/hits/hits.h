#ifndef hits_h
#define hits_h

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

void hits_anim(int character_state, int counter, sprite_perso * attacker, sprite_perso * receiver, jeu * world);
void light_punch(sprite_perso* attacker, sprite_perso* receiver,jeu* world);
void heavy_punch(sprite_perso *attacker, sprite_perso *receiver,jeu* world);
void kick_hit(sprite_perso *attacker, sprite_perso *receiver,jeu* world);
void check_fireball(sprite_perso *attacker, sprite_perso *receiver, jeu * world);
void check_gravityball(sprite_perso *attacker, sprite_perso *receiver, jeu * world);
void move_fireball(sprite_perso *perso);
void move_gravityball(sprite_perso *perso);

#endif