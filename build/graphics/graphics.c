
#include "graphics.h"
/*SDL_Texture* load_image( char path[],SDL_Renderer *renderer){
    
    SDL_Surface* surfaceCharge = IMG_Load(path);
    return SDL_CreateTextureFromSurface(renderer, surfaceCharge);
}
*/



void init_texture(SDL_Renderer *renderer, sprite_perso *perso){
    perso->texture_perso = load_image( "build/ressources/stickman.png",renderer);
    
}

void apply_sprite(SDL_Renderer *renderer, SDL_Texture *texture, sprite_perso* sprite){
    SDL_Rect dst = {0, 0, 0, 0};
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = sprite->x; dst.y = sprite->y;
    if(!sprite->mirror){
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        }
    else{
        SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
        SDL_Point center = {sprite->w/2,sprite->h/2};
        SDL_RenderCopyEx(renderer, texture, NULL, &dst,180, &center, flip);
    }
    
}

void refresh_graphics(SDL_Renderer *renderer, jeu *world){
    SDL_RenderClear(renderer);
    switch(world->state){
        case combat:
            display_map(renderer,world);
            display_dynamic_texture(renderer, world->map.map_structure, world->map.plateformes);
            apply_sprite(renderer, world->p1.texture_perso, &(world->p1));
            apply_sprite(renderer, world->p2.texture_perso, &(world->p2));
            display_life(renderer, world);
        break;
        case main_menu:
            apply_textures(world->menu_set.menu_fond, renderer, 0, 0);
            SDL_Rect rect;
            rect.x = 400;
            rect.y = 200 + 100 * world->menu_set.index_menu ;
            rect.h = 100 ;
            rect.w = 400; 
            SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 1);
            SDL_RenderDrawRect(renderer, &rect);
        break;
        case selection_map:
            apply_textures(world->menu_set.menu_fond, renderer, 0, 0);
            apply_textures(world->menu_set.tab_map[world->menu_set.index_menu],renderer,240, 135);
        break;
        }
        //apply_texture(world->p1.texture_perso, renderer, world->p1.x , world->p1.y);
        SDL_RenderPresent(renderer);
}

void display_life(SDL_Renderer* renderer, jeu* world){
    SDL_Rect rect;
    SDL_Rect rect2;
    rect.x = 400 ;
    rect.h = 20;
    rect.y = SCREEN_HEIGHT- rect.h;
    if(world->p1.life >= 0){
        rect.w = world->p1.life * -20;
    }
    else{
        rect.w = 0;
    }
    if(world->p2.life >= 0){
        rect2.w = world->p2.life * 20 ;
    }
    else{
        rect2.w = 0;
    } 
    rect2.x = SCREEN_WIDTH - 400;
    rect2.y = SCREEN_HEIGHT - rect.h;
    rect2.h = 20;
    SDL_Rect rect_fond;
    SDL_Rect rect_fond2;
    rect_fond.x = 400 ;
    rect_fond.h = 20;
    rect_fond.y = SCREEN_HEIGHT- rect.h;
    rect_fond.w = -400;
    rect_fond2.w =  400 ;
    rect_fond2.x = SCREEN_WIDTH - 400;
    rect_fond2.y = SCREEN_HEIGHT - rect.h;
    rect_fond2.h = 20;
    SDL_SetRenderDrawColor(renderer, 255, 0 , 0, 1);
    SDL_RenderFillRect(renderer, &rect_fond);
    SDL_RenderFillRect(renderer, &rect_fond2);
    SDL_SetRenderDrawColor(renderer, 255, 179 , 92, 1);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect2);
}

void apply_textures(SDL_Texture * texture, SDL_Renderer *renderer, int x , int y){
    SDL_Rect dst = {0, 0, 0, 0};

    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void display_map(SDL_Renderer* renderer,jeu* world){
    switch(world->choosed_map){
        case russia:
            apply_textures(world->map.image_fond,renderer,0,0);
            //read_structure(world->map.map_russia.map_structure);
        break;
        case forest:
            apply_textures(world->map.image_fond,renderer,0,0);
        break;
    }
}

void display_dynamic_texture(SDL_Renderer* renderer, char** map_struct, SDL_Texture* texture){
    if(texture == NULL){
        printf("Probleme de la texture des plateformes");
    }
    if(map_struct == NULL){
        printf("Probleme de la map_struct");
    }
    for(int i = 0 ; i<40 ; i++){
        for(int j = 0 ; j<20 ; j++){
            if(map_struct[i][j] == '5'){
                apply_textures(texture, renderer, j * CELL_WIDTH, i * CELL_HEIGHT);
            }
        }
    }
}