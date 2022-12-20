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
void movements(jeu* world, sprite_perso* perso, sprite_perso* adversaire);
bool equals(int x, int y, char** map_point, char test);
void light_punch(sprite_perso* attacker, sprite_perso* receiver);
void change_directions(sprite_perso * p1, sprite_perso * p2);
void init_controller(jeu* world);
bool canMove(sprite_perso * perso, sprite_perso * adversaire);
void sprites_collision(sprite_perso * p1, sprite_perso * p2, jeu * world);
void init_miniature(jeu* world,SDL_Renderer* renderer);
void add_input_buffer(sprite_perso* player, enum combos_inputs touche_appui, int timestamp);
bool read_combo(sprite_perso* player, int val);
void init_combo(sprite_perso* player);
#endif