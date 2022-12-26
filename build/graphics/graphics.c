
#include "graphics.h"



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

void play_animations(SDL_Renderer *renderer, sprite_perso* sprite, int chara_state){
    SDL_Rect dst = {0, 0, 0, 0};
    SDL_Rect src = {0, 0, 0, 0};

    dst.x = sprite->x;
    dst.y = sprite->y;
    dst.w = sprite->anim[chara_state].width;
    dst.h = 250;

    int width;
    SDL_QueryTexture(sprite->anim[chara_state].anim_text, NULL, NULL, &width, &src.h);
    src.h -=10;
    src.x = sprite->anim[chara_state].frame * 500;
    src.y = 0;
    src.w = sprite->anim[chara_state].width;
 if(!sprite->mirror){
    SDL_RenderCopy(renderer, sprite->anim[chara_state].anim_text, &src, &dst);
 }
 else{
    SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
        SDL_Point center = {150/2,250/2};
        SDL_RenderCopyEx(renderer,  sprite->anim[chara_state].anim_text, &src, &dst,180, &center, flip);
 }
}

void refresh_graphics(SDL_Renderer *renderer, jeu *world){
    SDL_RenderClear(renderer);
    switch(world->state){
        case combat:
            display_map(renderer,world);
            display_dynamic_texture(renderer, world->map.map_structure, world->map.plateformes);
            play_animations(renderer, &(world->p1), world->p1.chara_state);
            play_animations(renderer, &(world->p2), world->p2.chara_state);
            display_life(renderer, world);
            char* compteur = malloc(sizeof(char)*3);
            sprintf(compteur, "%d",world->timer.timer);
            SDL_Color color = {0,0,0};
            SDL_Surface* surface_compteur = TTF_RenderText_Solid(world->font.police_compteur, compteur, color);
            SDL_Texture* texture_compteur = SDL_CreateTextureFromSurface(renderer,surface_compteur);
            apply_textures(texture_compteur,renderer,640,360);
        break;
        case main_menu:
            apply_textures(world->menu_set.menu_fond, renderer, 0, 0);
            SDL_Rect rect;
            rect.x = 140;
            rect.y = 250 + 150 * world->menu_set.index_menu ;
            rect.h = 150 ;
            rect.w = 950; 
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
    /*VIE JOUEUR 1*/
    SDL_Rect rect;
    SDL_Rect rect_fond;
    rect.x = 500 ;
    rect.y = 0;
    rect.h = 30;

    rect_fond.x = 500 ;
    rect_fond.h = 30;
    rect_fond.y = 0;
    rect_fond.w = -500;

    if(world->p1.life >= 0){
        rect.w = world->p1.life * -25;
    }
    else{
        rect.w = 0;
    }

    /*VIE JOUEUR 2*/
    SDL_Rect rect2;
    SDL_Rect rect_fond2;
    rect2.x = SCREEN_WIDTH - 500 ;
    rect2.y = 0;
    rect2.h = 30;
    
    
    rect_fond2.x = SCREEN_WIDTH - 500;
    rect_fond2.h = 30;
    rect_fond2.y = 0;
    rect_fond2.w =  500 ;

    if(world->p2.life >= 0){
        rect2.w = world->p2.life * 25 ;
    }
    else{
        rect2.w = 0;
    } 

    /*RENDERING*/
    SDL_SetRenderDrawColor(renderer, 255, 20 , 15, 1);
    SDL_RenderFillRect(renderer, &rect_fond);
    SDL_RenderFillRect(renderer, &rect_fond2);

    SDL_SetRenderDrawColor(renderer, 255, 179 , 92, 1);
    SDL_SetRenderDrawColor(renderer, 0, 190, 25, 0);
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
        case street_art:
            apply_textures(world->map.image_fond,renderer,0,0);
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