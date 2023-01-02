#ifndef controller_h
#define controller_h

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
#include "../init/init.h"
#include "../data/data.h"
#include "../lootbox/lootbox.h"

/*FONCTIONS INPUTS USERS*/
void handle_menu_inputs(SDL_Event *event, jeu *world, SDL_Renderer* renderer);
void gameplay_inputs(SDL_Event *event, jeu *world);

#endif