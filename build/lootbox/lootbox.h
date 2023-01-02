#ifndef lootbox_h
#define lootbox_h

#include "../structure/structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../constante.h"
#include "../graphics/graphics.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../init/init.h"


/*FONCTION POUR LES LOOTBOXES*/
int generate_number (int a, int b);
void update_lootbox(jeu * world);
void check_lootbox_pickup(sprite_perso * player, lootbox * lootbox, int player_number);
void apply_bonus(lootbox * lootbox, sprite_perso *player);
void lootbox_loop(jeu *world);

#endif