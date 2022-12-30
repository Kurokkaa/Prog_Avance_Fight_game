
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
void render_aura(SDL_Renderer *renderer, sprite_perso* sprite, int numaura){
    SDL_Rect dst = {0, 0, 0, 0};
    SDL_Rect src = {0, 0, 0, 0};

    dst.x = sprite->x - 10;
    dst.y = sprite->y;
    dst.w = sprite->anim[numaura].width;
    dst.h = 250;

    int width;
    SDL_QueryTexture(sprite->anim[numaura].anim_text, NULL, NULL, &width, &src.h);
    src.h -=10;
    src.x = sprite->anim[numaura].frame * sprite->anim[numaura].width;
    src.y = 0;
    src.w = sprite->anim[numaura].width;
    SDL_RenderCopy(renderer, sprite->anim[numaura].anim_text, &src, &dst);
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
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
    SDL_Rect rect;
    switch(world->state){
        case combat:
            case pause:
            display_map(renderer,world);
            display_dynamic_texture(renderer, world->map.map_structure, world->map.plateformes);
            render_bonuses(renderer, &world->lootbox);
             if(world->p1.anim[15].aura || world->p1.damage_bonus){
                render_aura(renderer, &(world->p1),15);
            }
            if(world->p2.anim[15].aura || world->p2.damage_bonus){
                render_aura(renderer, &(world->p2),15);
            }
            play_animations(renderer, &(world->p1), world->p1.chara_state);
            play_animations(renderer, &(world->p2), world->p2.chara_state);
             if(world->p1.anim[15].aura || world->p1.damage_bonus){
                render_aura(renderer, &(world->p1),16);
            }
            if(world->p2.anim[15].aura || world->p2.damage_bonus){
                render_aura(renderer, &(world->p2),16);
            }
            display_life(renderer, world);
            display_fireball(renderer,world->p1);
            display_fireball(renderer,world->p2);
            display_timer(world,renderer);
            display_special(renderer,world);
            if(world->p1.guard){
                display_guard(renderer,world->p1);
            }
            if(world->p2.guard){
                display_guard(renderer,world->p2);
            }
            if(world->state == pause){
                //blend mode pour gérer la transparence
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                //Couleur noire avec une opacité de 50%
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
                SDL_RenderFillRect(renderer,NULL);
                 SDL_Color color = {180,180,180};
                SDL_Surface* surface_compteur = TTF_RenderText_Solid(world->font.police_compteur, "RESUME", color);
                SDL_Texture* texture_compteur = SDL_CreateTextureFromSurface(renderer,surface_compteur);
                apply_textures(texture_compteur,renderer,580, 300);
                SDL_Surface* quit_surface = TTF_RenderText_Solid(world->font.police_compteur, "quit", color);
                SDL_Texture* quit_texture = SDL_CreateTextureFromSurface(renderer,quit_surface);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                apply_textures(quit_texture,renderer,600,350 );
                rect.x = 140;
                rect.y = 300 + 50 * world->menu_set.index_menu ;
                rect.h = 50 ;
                rect.w = 950; 
                SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
            break;
        case main_menu:
            apply_textures(world->menu_set.menu_fond, renderer, 0, 0);
            
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
        case options:
            apply_textures(world->menu_set.menu_fond,renderer,0,0);
        
            rect.x = 60 + 300 * world->menu_set.index_menu;
            rect.y = 250;
            rect.h = 300 ;
            rect.w = 290; 
            SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 1);
            SDL_RenderDrawRect(renderer, &rect);
        }
        //apply_texture(world->p1.texture_perso, renderer, world->p1.x , world->p1.y);
        SDL_RenderPresent(renderer);
}
void display_timer(jeu* world,SDL_Renderer* renderer){
            char* timer;
            if(world->timer.inf){
               timer = "inf";
            } 
            else{
                 timer= malloc(sizeof(char)*3);
                sprintf(timer, "%d",world->timer.timer);
            }
     //Fond du timer
            SDL_Rect timer_background;
            timer_background.x = 500;
            timer_background.y = 0;
            timer_background.h = 30;
            timer_background.w = 280;
            SDL_SetRenderDrawColor(renderer, 215, 220, 230, 1);
            SDL_RenderFillRect(renderer, &timer_background);
            
            //Affichage du timer
            SDL_Color color = {0,0,0};
            SDL_Surface* surface_compteur = TTF_RenderText_Solid(world->font.police_compteur, timer, color);
            SDL_Texture* texture_compteur = SDL_CreateTextureFromSurface(renderer,surface_compteur);
            apply_textures(texture_compteur,renderer,620, -5);
            free(surface_compteur);
           
}
void display_fireball(SDL_Renderer* renderer,sprite_perso perso){
   if(perso.fireball.launched_fireball){
        apply_textures(perso.fireball.fireball,renderer,perso.fireball.x,perso.fireball.y);
    }
}
char* barlvl(sprite_perso perso){
     char* lvl;
     if(perso.special_bar>=0){
        lvl = "0";
    }
    if(perso.special_bar>=100){
        lvl = "1";
    }
    if(perso.special_bar>=200){
        lvl =  "2";
    }
    if(perso.special_bar>=300){
        lvl = "3";
    }
    return lvl;
}
void display_special(SDL_Renderer* renderer, jeu* world){
    char* lvlP1 = barlvl(world->p1);
    char* lvlP2 = barlvl(world->p2);
    
    SDL_Rect rect;
    SDL_Rect rect_fond;
    rect.x = 500 ;
    rect.y = 80;
    rect.h = 30;

    rect_fond.x = 500 ;
    rect_fond.h = 30;
    rect_fond.y = 80;
    rect_fond.w = -500;

    if(world->p1.special_bar < 300){
        rect.w = (world->p1.special_bar % 100) * -5;
    }
    else{
        rect.w = -500;
    }
    SDL_Color color = {255,0,0};
    SDL_Surface* lvl1 = TTF_RenderText_Solid(world->font.police_compteur,lvlP1 , color);
    SDL_Texture* texture_lvl1 = SDL_CreateTextureFromSurface(renderer,lvl1);
    apply_textures(texture_lvl1,renderer,520, 75);
    SDL_Surface* lvl2 = TTF_RenderText_Solid(world->font.police_compteur,lvlP2 , color);
    SDL_Texture* texture_lvl2 = SDL_CreateTextureFromSurface(renderer,lvl2);
    apply_textures(texture_lvl2,renderer,750, 75);
    
    //free(surface_compteur);
    SDL_Rect rect2;
    SDL_Rect rect_fond2;
    rect2.x = SCREEN_WIDTH - 500 ;
    rect2.y = 80;
    rect2.h = 30;
    
    
    rect_fond2.x = SCREEN_WIDTH - 500;
    rect_fond2.h = 30;
    rect_fond2.y = 80;
    rect_fond2.w =  500 ;

    if(world->p2.special_bar <300){
        rect2.w = (world->p2.special_bar %100) * 5 ;
    }
    else{
        rect2.w = 500;
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
void display_guard(SDL_Renderer* renderer,sprite_perso perso){
   SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if(perso.life_guard >0){
       SDL_SetRenderDrawColor(renderer,255,0,0,50);
    }
     if(perso.life_guard >=25){
        SDL_SetRenderDrawColor(renderer,255,128,0,50);
    }
    if(perso.life_guard >=50){
        SDL_SetRenderDrawColor(renderer,0,255,0,50);
    }
    disque(perso.x+perso.w - 20 ,perso.y+perso.h/2,perso.life_guard*1.5,renderer);
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
        rect.w = world->p1.life * -500 / MAX_LIFE;
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
        rect2.w = world->p2.life * 500 / MAX_LIFE ;
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
        case city_night:
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
    int width_factor,height_factor;
    width_factor = CELL_WIDTH;
    height_factor = CELL_HEIGHT;
    for(int i = 1; i<40 ; i++){
        for(int j = 1 ; j<20 ; j++){
            
            if(map_struct[i][j] == '5'){
                printf("i: %d j:%d \n",i*height_factor,j*width_factor);
                apply_textures(texture, renderer, (j+1) * width_factor, i * height_factor);
            }
        }
       
    }
}

void render_bonuses(SDL_Renderer * renderer, lootbox * lootbox){
    if(lootbox->active){
        apply_textures(lootbox->texture[lootbox->bonus],renderer,lootbox->x,lootbox->y);
    }
}

void disque(int cx,int cy,int rayon,SDL_Renderer* renderer){
   int d, y, x;
 
  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;
 
  while (y >= x) {
    ligneHorizontale(cx - x, cy - y, 2 * x + 1,renderer);
    ligneHorizontale(cx - x, cy + y, 2 * x + 1,renderer);
    ligneHorizontale(cx - y, cy - x, 2 * y + 1,renderer);
    ligneHorizontale(cx - y, cy + x, 2 * y + 1,renderer);
 
    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
 
    x++;
  }
    }

void ligneHorizontale(int x, int y, int w, SDL_Renderer* renderer)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;

  SDL_RenderFillRect(renderer,&r);
}

