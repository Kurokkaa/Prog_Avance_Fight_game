#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../constante.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

struct hit_chara_s
{
    int dmg;
    int range_x;
    int range_y;
    int speed;
    bool launch;
    int timer;
    int effective_frame;
    int delay;
};
typedef struct hit_chara_s hit;

struct map_settings{
    enum game_map choosed_map;
    char** map_structure;
    SDL_Texture* image_fond;
    SDL_Texture* plateformes;
};
typedef struct map_settings set_map;

struct animation_s{
    int aura;
    int frame;
    int nbFrame;
    int width;
    int counter;
    SDL_Texture *anim_text;
};
typedef struct animation_s anim_t;

struct hit_set_s
{
    hit *light_punch;
    hit *heavy_punch;
    hit *kick;
    hit *special_attack;
};
typedef struct hit_set_s hit_set;

struct inputs_s
{
    enum combos_inputs input;
    int timestamp;
};

typedef struct inputs_s inputs;

struct combos_s
{
    enum combos_inputs input[10];
    int frame_between;
    int nb_coups;
    int required;
};
typedef struct combos_s combo;

struct compteur_s{
    int startTime;
    int pauseTime;
    bool pause;
    bool start;
    int timer; 
    bool inf;
};
typedef struct compteur_s compteur;

struct throwable_s
{
    SDL_Texture * fireball;
    bool launched_fireball;
    int x;
    int y;
    int multiplicateur;
    compteur timer_throw;
};
typedef struct throwable_s throwable;

struct sprite_p{
    SDL_Texture * aura;
    int perso_choisi; 
    int x;
    int y;
    int w; /*!<largeur du sprite */
    int h; /*!<hauteur du sprite */
    int speed;
    int perso;
    int stun_time;
    enum character_state chara_state;
    bool backwards;
    int jump_height;
    int jump_origin;
    bool mirror;
    bool double_jump;
    int life;
    hit_set hits;
    anim_t anim[17];
    inputs *buffer;
    int pos_tab_combo;
    combo tab_combo[NB_COMBOS];
    bool attack_launched;
    bool guard;
    int life_guard;
    bool broken_guard;
    bool damage_bonus;
    bool berserk;
    int bonus_timer;
    int special_bar;
    compteur chrono_guard;
    compteur chrono_special;
    compteur dmg_bonus_timer;
    throwable fireball;
    throwable gravityball;
};
typedef struct sprite_p sprite_perso;

struct menu_p
{
    int index_menu;
    SDL_Texture *menu_fond;
    SDL_Texture *tab_map[3];
    SDL_Texture *cadreVie;
};
typedef struct menu_p menu;

struct ressources_s
{
    TTF_Font *police_compteur;
};
typedef struct ressources_s ressources;

struct lootbox_s{
    SDL_Texture * texture[3];
    int x;
    int y;
    int w;
    int h;
    int fallspeed;
    enum bonus_type bonus;
    bool active;
    int collided;
    bool falling;
};
typedef struct lootbox_s lootbox;

struct Sound_s{
    Mix_Chunk* menu;
    Mix_Chunk* light_punch;
    Mix_Chunk* heavy_punch;
    Mix_Chunk* kick;
    Mix_Chunk* fireball;
    Mix_Chunk* frappe;
    Mix_Chunk* change;
    Mix_Chunk* guard;
    bool menuPlayed;
};
typedef struct Sound_s Sound;

struct world_s{
    enum game_state state;
    enum game_map choosed_map;
    bool terminer; /*!< Champ indiquant si l'on est à la fin du jeu */
    sprite_perso p1;
    sprite_perso p2;
    set_map map;
    SDL_GameController **joysticks;
    menu menu_set;
    int timestamp_w;
    int keystates_pre[123];
    ressources font;
    compteur timer;
    lootbox lootbox;
    Sound music;
    bool game_over;
};
typedef struct world_s jeu;

#endif