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
    hit* punch;
    hit* kick;
    hit* special_attack;
};

typedef struct hit_set_s hit_set;

/*struct personnage{
};

enum perso{p1,p2};
*/

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
};


typedef struct sprite_p sprite_perso ;
struct world_s{
    enum game_state state;
    enum game_map choosed_map;
    bool terminer; /*!< Champ indiquant si l'on est à la fin du jeu */
    sprite_perso p1;
    sprite_perso p2;
    set_map map;
    SDL_GameController** joysticks;
};

typedef struct world_s jeu;

#endif