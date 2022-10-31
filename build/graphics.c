#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "structure/structure.h"

SDL_Texture* load_image( char path[],SDL_Renderer *renderer){
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ){
        printf( "Erreur initialisation de SDL_Image: %s\n", IMG_GetError() );
        SDL_Quit();
    }
    SDL_Surface* surfaceCharge = IMG_Load(path);
    return SDL_CreateTextureFromSurface(renderer, surfaceCharge);
}

void init_texture(SDL_Renderer *renderer, sprite_perso *perso){
    perso->text_perso = load_image( "ressources/stickman.png",renderer);
}

void apply_sprite(SDL_Renderer *renderer, SDL_Texture *texture, sprite_perso* sprite){
    SDL_Rect dst = {0, 0, 0, 0};
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = sprite->x; dst.y = sprite->y;
    
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void refresh_graphics(SDL_Renderer *renderer, jeu *world){
        SDL_RenderClear(renderer);
        apply_sprite(renderer, world->pers1.text_perso, &(world->pers1));
        //apply_texture(world->pers1.text_perso, renderer, world->pers1.x , world->pers1.y);
        SDL_RenderPresent(renderer);
}

/*void apply_textures(SDL_Texture * texture, SDL_Renderer *renderer, sprite_perso *perso){
    SDL_Rect dst = {0, 0, 0, 0};

    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}*/