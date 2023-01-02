#ifndef data_h
#define data_h


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

/*FONCTIONS DE LECTURES*/
char** read_file_map(char* nom);
SDL_Texture* load_image( char path[],SDL_Renderer *renderer);
bool read_combo(sprite_perso* player, int val, jeu * world);
void checkJoystick(SDL_Joystick** joysticks);
void read_counter(jeu *world);

/*FONCTIONS MOUVEMENTS ET ACTIONS*/
void update_data(jeu* world);
void movements(jeu* world, sprite_perso* perso, sprite_perso* adversaire);
bool equals(int x, int y, char** map_point, char test);
void light_punch(sprite_perso* attacker, sprite_perso* receiver,jeu* world);
void heavy_punch(sprite_perso *attacker, sprite_perso *receiver,jeu* world);
void kick_hit(sprite_perso *attacker, sprite_perso *receiver,jeu* world);
void change_directions(sprite_perso * p1, sprite_perso * p2);
bool canMove(sprite_perso * perso, sprite_perso * adversaire);
void sprites_collision(sprite_perso * p1, sprite_perso * p2, jeu * world);
void add_input_buffer(sprite_perso* player, enum combos_inputs touche_appui, int timestamp);

/*FONCTIONS FREE ESPACE ALLOUE*/
void quit_game(jeu* world,SDL_Window** fenetre,SDL_Renderer** renderer);
void close_Joystick(jeu* world);
void free_map_structure(char** map_structure);
void destroy_textures(jeu* world);
void free_menu(jeu* world);

/*FONCTIONS INPUTS USERS*/
void handle_menu_inputs(SDL_Event *event, jeu *world, SDL_Renderer* renderer);
void gameplay_inputs(SDL_Event *event, jeu *world);

/*FONCTION POUR LES LOOTBOXES*/
int generate_number (int a, int b);
void init_lootbox(lootbox * lootbox, SDL_Renderer * renderer);

void update_lootbox(jeu * world);
void check_lootbox_pickup(sprite_perso * player, lootbox * lootbox, int player_number);
void apply_bonus(lootbox * lootbox, sprite_perso *player);
void lootbox_loop(jeu *world);

/*FONCTION CHRONOMETRE*/
void unpause(compteur* chrono);
void pauseChrono(compteur* chrono);
void check_timer(jeu *world);

#endif