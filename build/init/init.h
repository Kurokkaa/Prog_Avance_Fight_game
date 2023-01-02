
#ifndef init_h
#define init_h


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
#include "sys/stat.h"
#include "../lootbox/lootbox.h"

/*FONCTIONS D'INITIALISATION*/
void init_controller(jeu* world);
void init_map(jeu* world,SDL_Renderer* renderer);
void init_miniature(jeu* world,SDL_Renderer* renderer);
void init_jeu(jeu *world, SDL_Renderer* renderer);
void init_perso(SDL_Renderer *renderer, sprite_perso *perso, int x, int y, int w, int h, int speed, bool mirror,int choice);
void init_hits(sprite_perso* perso);
void init_combos(sprite_perso *player);
void init_combo(int pos,int nb_coups,int frame_between,int required,sprite_perso *player);
void init(SDL_Window** window, SDL_Renderer** renderer, jeu* world);
void init_state_animation(SDL_Renderer* renderer, sprite_perso* perso, enum character_state state, char* path, int nbFrame, int width);
void init_chara_state(SDL_Renderer* renderer, sprite_perso* perso);
void init_timer(jeu* world);

#endif