
#include "graphics.h"
/*SDL_Texture* load_image( char path[],SDL_Renderer *renderer){
    
    SDL_Surface* surfaceCharge = IMG_Load(path);
    return SDL_CreateTextureFromSurface(renderer, surfaceCharge);
}
*/



void init_texture(SDL_Renderer *renderer, sprite_perso *perso,jeu* world){
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
      
        if(world->state==combat){
            display_map(renderer,world);
        }
        apply_sprite(renderer, world->p1.texture_perso, &(world->p1));
        //apply_texture(world->p1.texture_perso, renderer, world->p1.x , world->p1.y);
        SDL_RenderPresent(renderer);
}

void apply_textures(SDL_Texture * texture, SDL_Renderer *renderer, int x , int y){
    SDL_Rect dst = {0, 0, 0, 0};

    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void display_map(SDL_Renderer* renderer,jeu* world){
    switch(world->choice){
        case russia:
            apply_textures(world->maps.map_r.image_fond,renderer,0,0);
            break;
    }
}