#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../constante.h"
#include "../map/map_global.h"

struct hit_chara_s{
    int dmg;
    int range_x;
    int range_y;
    int speed;
    bool launch;
    int timer;
    int frame;
    SDL_Texture* animation;
};

typedef struct hit_chara_s hit;
struct hit_set_s{
    hit* light_punch;
    hit* heavy_punch;
    hit* flight_punch;
    hit* low_kick;
    hit* middle_kick;
    hit* flight_kick;
    hit* special_attack;
};

typedef struct hit_set_s hit_set;

/*struct personnage{
};

enum perso{p1,p2};
*/

struct inputs_s{
    enum combos_inputs input;
    int timestamp;
};

typedef struct inputs_s inputs;
struct combos_s{
    enum combos_inputs input[10];
    int frame_between;
    int nb_coups;
};

typedef struct combos_s combo;
struct sprite_p{
    SDL_Texture * texture_perso;
    int x;
    int y;
    int w; /*!<largeur du sprite */
    int h; /*!<hauteur du sprite */
    int speed;
    int perso;
    enum character_state chara_state;
    bool backwards;
    int jump_height;
    int jump_origin;
    int animation;
    bool mirror;
    int life;
    hit_set hits;
    inputs* buffer;
    int pos_tab_combo;
    combo tab_combo[2];
};

typedef struct sprite_p sprite_perso ;

struct menu_p{
    int index_menu;
    SDL_Texture* menu_fond;
    SDL_Texture* tab_map[3];
};

typedef struct menu_p menu;
struct world_s{
    enum game_state state;
    enum game_map choosed_map;
    bool terminer; /*!< Champ indiquant si l'on est à la fin du jeu */
    sprite_perso p1;
    sprite_perso p2;
    set_map map;
    SDL_GameController** joysticks;
    menu menu_set;
    int timestamp_w;
    int keystates_pre[123];
};

typedef struct world_s jeu;


#endif