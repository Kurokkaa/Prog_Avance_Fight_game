#ifndef data_h
#define data_h


#include "../structure/structure.h"
#include <stdio.h>
#include "../constante.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../map/russia/map_russia.h"


SDL_Texture* load_image( char path[],SDL_Renderer *renderer);
char** read_file_map(char* nom);
void init_map(jeu* world,SDL_Renderer* renderer);
void limit_movements(sprite_perso* perso);
void is_empty(int x, int y, char** struct);

#endif