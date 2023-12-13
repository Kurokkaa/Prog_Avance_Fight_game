#ifndef data_h
#define data_h

#include "../structure/structure.h"
#include <stdio.h>
#include <stdlib.h>
#include "../constante.h"
#include "SDL2/SDL.h"
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>
#include "sys/stat.h"
#include "../controller/controller.h"
#include "../lootbox/lootbox.h"
#include "../movements/movements.h"
#include "../hits/hits.h"

/*FONCTIONS DE LECTURES*/
char** read_file_map(char* nom);
SDL_Texture* load_image( char* path,SDL_Renderer *renderer);
bool read_combo(sprite_perso* player, int val, jeu * world);
void checkJoystick(SDL_Joystick** joysticks);
void read_counter(jeu *world);
void save_victory(int player);
bool file_exists (char* filename);
bool equals(int x, int y, char** map_point, char test);

/*FONCTIONS D'ECRITURE*/

void write_victory(int nb1, int nb2);

/*FONCTIONS MOUVEMENTS ET ACTIONS*/
void update_data(jeu* world);
void add_input_buffer(sprite_perso* player, enum combos_inputs touche_appui, int timestamp);
void compute_game(jeu *world);
void endgame_data(jeu *world);
void decision_ia(jeu* world, sprite_perso *p1, sprite_perso *p2);


/*FONCTIONS FREE ESPACE ALLOUE*/
void quit_game(jeu* world,SDL_Window** fenetre,SDL_Renderer** renderer);
void close_Joystick(jeu* world);
void free_map_structure(char** map_structure);
void destroy_textures(jeu* world);
void free_menu(jeu* world);

/*FONCTION CHRONOMETRE*/
void unpause(compteur* chrono);
void pauseChrono(compteur* chrono);
void check_timer(jeu *world);

#endif