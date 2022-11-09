#ifndef MAP_GLOBAL_H
#define MAP_GLOBAL_H

#include "SDL2/SDL.h"

struct map_settings{
    enum game_map choose_map;
    char** map_structure;
    int taille_cellule_H;
    int taille_cellule_W;
    SDL_Texture* image_fond;
    SDL_Texture* plateformes;
};

typedef struct map_settings set_map;

#endif