#ifndef graphics_h
#define graphics_h

#include <SDL2/SDL.h>
#include <stdio.h>
#include "../structure/structure.h"
#include "../data/data.h"
#include "../constante.h"
#include "../liste/liste.h"
#include <SDL2/SDL_ttf.h>


void apply_sprite(SDL_Renderer *renderer, SDL_Texture *texture, sprite_perso* sprite);
void play_animations(SDL_Renderer *renderer, sprite_perso* sprite, int chara_state);
void refresh_graphics(SDL_Renderer *renderer, jeu *world);
void display_fireball(SDL_Renderer* renderer,throwable projectile);
void display_gravityball(SDL_Renderer* renderer,sprite_perso perso);
void display_timer(jeu* world,SDL_Renderer* renderer);
void display_special(SDL_Renderer* renderer, jeu* world);
void display_guard(SDL_Renderer* renderer,sprite_perso perso);
void apply_textures(SDL_Texture * texture, SDL_Renderer *renderer, int x , int y);
void display_map(SDL_Renderer* renderer,jeu* world);
void display_dynamic_texture(SDL_Renderer* renderer, char** map_struct, SDL_Texture* texture);
void display_life(SDL_Renderer* renderer, jeu* world);
void disque(int cx,int cy,int rayon,SDL_Renderer* renderer);
void ligneHorizontale(int x, int y, int w, SDL_Renderer* renderer);
void render_bonuses(SDL_Renderer * renderer, lootbox * lootbox);

#endif