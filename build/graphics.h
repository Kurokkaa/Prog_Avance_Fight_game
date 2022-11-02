#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structure/structure.h"

SDL_Texture* load_image( char path[],SDL_Renderer *renderer);
void init_texture(SDL_Renderer *renderer, sprite_perso *perso);
void apply_sprite(SDL_Renderer *renderer, SDL_Texture *texture, sprite_perso* sprite);
void refresh_graphics(SDL_Renderer *renderer, jeu *world);
//void apply_texture();