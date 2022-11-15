#ifndef data_h
#define data_h


#include "../structure/structure.h"
#include <stdio.h>
#include "../constante.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include <math.h>


SDL_Texture* load_image( char path[],SDL_Renderer *renderer);
char** read_file_map(char* nom);
void init_map(jeu* world,SDL_Renderer* renderer);
void movements(jeu* world, sprite_perso* perso, int * pos_init_x);
bool equals(int x, int y, char** map_point, char test);
void punch(sprite_perso* attacker, sprite_perso* receiver);
void change_directions(sprite_perso * p1, sprite_perso * p2);
void init_controller(jeu* world);
void collision_perso(sprite_perso * p1, sprite_perso * p2, int pos_initP1);
#endif