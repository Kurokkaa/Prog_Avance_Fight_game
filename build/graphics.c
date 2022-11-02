#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "structure/structure.h"

/*SDL_Texture* load_image( char path[],SDL_Renderer *renderer){
    
    SDL_Surface* surfaceCharge = IMG_Load(path);
    return SDL_CreateTextureFromSurface(renderer, surfaceCharge);
}
*/

SDL_Texture* load_image( char path[],SDL_Renderer *renderer){
    SDL_Surface* tmp = NULL;
    SDL_Texture* texture = NULL;
    tmp = IMG_Load(path);
    if(NULL == tmp){
        fprintf(stderr, "Erreur pendant chargement image BMP: %s", SDL_GetError());
        return NULL;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture){
        fprintf(stderr, "Erreur pendant creation de la texture liee a l'image chargee: %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

void init_texture(SDL_Renderer *renderer, sprite_perso *perso){
    perso->texture_perso = load_image( "build/ressources/stickman.png",renderer);
}

void apply_sprite(SDL_Renderer *renderer, SDL_Texture *texture, sprite_perso* sprite){
    SDL_Rect dst = {0, 0, 0, 0};
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = sprite->x; dst.y = sprite->y;
    
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void refresh_graphics(SDL_Renderer *renderer, jeu *world){
        SDL_RenderClear(renderer);
        apply_sprite(renderer, world->p1.texture_perso, &(world->p1));
        //apply_texture(world->p1.text_perso, renderer, world->p1.x , world->p1.y);
        SDL_RenderPresent(renderer);
}

/*void apply_textures(SDL_Texture * texture, SDL_Renderer *renderer, sprite_perso *perso){
    SDL_Rect dst = {0, 0, 0, 0};

    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}*/