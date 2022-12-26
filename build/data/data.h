#ifndef data_h
#define data_h


#include "../structure/structure.h"
#include <stdio.h>
#include "../constante.h"
#include "../graphics/graphics.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>

/*FONCTIONS D'INITIALISATION*/
void init_controller(jeu* world);
void init_map(jeu* world,SDL_Renderer* renderer);
void init_miniature(jeu* world,SDL_Renderer* renderer);
void init_jeu(jeu *world, SDL_Renderer* renderer);
void init_perso(SDL_Renderer* renderer, sprite_perso* perso, int x, int y, int w, int h, int speed,bool mirror);
void init_hits(sprite_perso* perso);
void init_combo(sprite_perso* player);
void init(SDL_Window** window, SDL_Renderer** renderer, jeu* world);
void init_state_animation(SDL_Renderer* renderer, sprite_perso* perso, enum character_state state, char* path, int nbFrame, int width);
void init_chara_state(SDL_Renderer* renderer, sprite_perso* perso);
void init_timer(jeu* world);

/*FONCTIONS DE LECTURES*/
char** read_file_map(char* nom);
SDL_Texture* load_image( char path[],SDL_Renderer *renderer);
bool read_combo(sprite_perso* player, int val);
void checkJoystick(SDL_Joystick** joysticks);

/*FONCTIONS MOUVEMENTS ET ACTIONS*/
void movements(jeu* world, sprite_perso* perso, sprite_perso* adversaire);
bool equals(int x, int y, char** map_point, char test);
void light_punch(sprite_perso* attacker, sprite_perso* receiver);
void reset_hit(sprite_perso* perso);
void change_directions(sprite_perso * p1, sprite_perso * p2);
bool canMove(sprite_perso * perso, sprite_perso * adversaire);
void sprites_collision(sprite_perso * p1, sprite_perso * p2, jeu * world);
void add_input_buffer(sprite_perso* player, enum combos_inputs touche_appui, int timestamp);

/*FONCTIONS FREE ESPACE ALLOUE*/
void quit_game(jeu* world,SDL_Window** fenetre,SDL_Renderer** renderer);
void close_Joystick(jeu* world);
void free_map_structure(char** map_structure);
void destroy_textures(jeu* world);

/*FONCTIONS INPUTS USERS*/
void handle_menu_inputs(SDL_Event *event, jeu *world, SDL_Renderer* renderer);
void gameplay_inputs(SDL_Event *event, jeu *world);

#endif