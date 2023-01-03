
#include "graphics.h"

/**
 * @brief Applique une Texture lorsque l'aura est activé
 *  
 * @param renderer Le moteur de rendu
 * @param sprite Image du personnage
 * @param numaura Numéro de l'aura affiché
 */
void render_aura(SDL_Renderer *renderer, sprite_perso* sprite, int numaura){
    //Configure l'animation de l'aura
    anim * aura = getAnimation(sprite->anim, numaura);
    SDL_Rect dst = {0, 0, 0, 0};
    SDL_Rect src = {0, 0, 0, 0};
    dst.x = sprite->x-sprite->w;
    dst.y = sprite->y;
    dst.w = aura->width;
    dst.h = 250;

    //Gère l'affichage de l'aura
    int width;
    SDL_QueryTexture(aura->anim_text, NULL, NULL, &width, &src.h);
    src.h -=10;
    src.x = aura->frame * aura->width;
    src.y = 0;
    src.w = aura->width;
    SDL_RenderCopy(renderer, aura->anim_text, &src, &dst);
}

/**
 * @brief Joue l'animation du personnage
 *  
 * @param renderer Le moteur de rendu
 * @param sprite Image du personnage
 * @param chara_state Etat du personnage 
 */
void play_animations(SDL_Renderer *renderer, sprite_perso* sprite, int chara_state){
    //Configure l'animation du personnage
    anim * animation = getAnimation(sprite->anim, chara_state) ;
    SDL_Rect dst = {0, 0, 0, 0};
    SDL_Rect src = {0, 0, 0, 0};
    dst.x = sprite->x;
    dst.y = sprite->y;
    dst.w = animation->width;
    dst.h = 250;

    //Affiche le personnage
    SDL_QueryTexture(animation->anim_text, NULL, NULL, NULL, &src.h);
    src.h -= 10;
    src.x = 40 + animation->frame * 500;
    src.y = 0;
    src.w = animation->width;

    //si le personnage est à droite de l'autre alors on inverse le sens du sprite
    if(!sprite->mirror){
        SDL_RenderCopy(renderer, animation->anim_text, &src, &dst);
    }
    else{
        SDL_RendererFlip flip = SDL_FLIP_VERTICAL;

            SDL_Point center = {animation->width/ 2, 250/2};
            SDL_RenderCopyEx(renderer, animation->anim_text, &src, &dst,180, &center, flip);
    }
}

/**
 * @brief Mets à jour les images du jeu
 *  
 * @param renderer Le moteur de rendu
 * @param world Les données du monde
 */
void refresh_graphics(SDL_Renderer *renderer, jeu *world){
    //efface l'ancienne image
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
    SDL_Rect rect;
    //selon l'état du jeu
    switch(world->state){
        //En partie
        case combat:
            //Si c'est en pause pendant combat
            case pause:
            display_map(renderer,world);
            if(world->state == combat){
                apply_textures(world->menu_set.cadreVie, renderer, 0 , 0); //Cadres contenant les vies et spécials
                display_dynamic_texture(renderer, world->map.map_structure, world->map.plateformes);
                render_bonuses(renderer, &world->lootbox);  //les bonus
                //si le personnage à un bonus de puissance on mets l'aura
                if(getAnimation(world->p1.anim, 19)->aura || world->p1.damage_bonus){
                    render_aura(renderer, &(world->p1),19);
                }
                if(getAnimation(world->p2.anim, 19)->aura || world->p2.damage_bonus){
                    render_aura(renderer, &(world->p2),19);
                }
                //animations des états des personnages
                play_animations(renderer, &(world->p1), world->p1.chara_state);
                play_animations(renderer, &(world->p2), world->p2.chara_state);
                if(getAnimation(world->p1.anim, 19)->aura || world->p1.damage_bonus){
                    render_aura(renderer, &(world->p1),20);
                }
                if(getAnimation(world->p2.anim, 19)->aura  || world->p2.damage_bonus){
                    render_aura(renderer, &(world->p2),20);
                }
                //HUD
                display_life(renderer, world);
                display_timer(world,renderer);
                //Les sorts 
                display_fireball(renderer,world->p1.fireball);
                display_fireball(renderer,world->p2.fireball);
                display_gravityball(renderer,world->p1);
                display_gravityball(renderer,world->p2);
                display_special(renderer,world);
                //la garde 
                if(world->p1.guard){
                    display_guard(renderer,world->p1);
                }
                if(world->p2.guard){
                    display_guard(renderer,world->p2);
                }
            }
            //si le monde est en pause
            if(world->state == pause){
                //blend mode pour gérer la transparence
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                //Couleur noire avec une opacité de 50%
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
                SDL_RenderFillRect(renderer,NULL);
                SDL_Color color = {180,180,180,0};
                SDL_Surface* surface_compteur = TTF_RenderText_Solid(world->font.police_compteur, "RESUME", color);
                SDL_Texture* texture_compteur = SDL_CreateTextureFromSurface(renderer,surface_compteur);
                apply_textures(texture_compteur,renderer,580, 300);
                SDL_Surface* quit_surface = TTF_RenderText_Solid(world->font.police_compteur, "QUIT", color);
                SDL_Texture* quit_texture = SDL_CreateTextureFromSurface(renderer,quit_surface);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                apply_textures(quit_texture,renderer,600,350 );
                rect.x = 140;
                rect.y = 300 + 50 * world->menu_set.index_menu ;
                rect.h = 50 ;
                rect.w = 950; 
                SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
                SDL_FreeSurface(surface_compteur);
                SDL_FreeSurface(quit_surface);
                SDL_DestroyTexture(texture_compteur);
                SDL_DestroyTexture(quit_texture);
            }
            break;

        //si la partie est finie   
        case endgame:
            display_map(renderer,world);
            play_animations(renderer, &(world->p1), world->p1.chara_state);
            play_animations(renderer, &(world->p2), world->p2.chara_state);
            SDL_Color color = {150,151,162,0};
            char* WhoWin;
            //Affichage du message de victoire pour le gagnant
            if(world->p1.chara_state == winner){
                WhoWin = "LE JOUEUR 1 GAGNE";
            }
            else{
                WhoWin = "LE JOUEUR 2 GAGNE";
            }
            SDL_Surface* surface_victoire = TTF_RenderText_Solid(world->font.police_victoire,WhoWin , color);
            SDL_Texture* texture_victoire = SDL_CreateTextureFromSurface(renderer,surface_victoire);
            apply_textures(texture_victoire,renderer,235, 200);
            free(surface_victoire);
            SDL_DestroyTexture(texture_victoire);
            break;
        //Quand on est dans le menu
        case main_menu:
            //le rectangle de selection du menu
            apply_textures(world->menu_set.menu_fond, renderer, 0, 0);
            rect.x = 140;
            rect.y = 250 + 150 * world->menu_set.index_menu ;
            rect.h = 150 ;
            rect.w = 950; 
            SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 1);
            SDL_RenderDrawRect(renderer, &rect);
            break;

        //selection de la carte pour jouer (les miniatures)
        case selection_map:
            apply_textures(world->menu_set.menu_fond, renderer, 0, 0);
            apply_textures(world->menu_set.tab_map[world->menu_set.index_menu],renderer,240, 135);
            break;

        //selection du du temps de la partie
        case options:
            apply_textures(world->menu_set.menu_fond,renderer,0,0);
        
            rect.x = 60 + 300 * world->menu_set.index_menu;
            rect.y = 250;
            rect.h = 300 ;
            rect.w = 290; 
            SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 1);
            SDL_RenderDrawRect(renderer, &rect);
        }
        SDL_RenderPresent(renderer);
}

/**
 * @brief Affiche le timer 
 *  
 * @param renderer Le moteur de rendu
 * @param world Les données du monde
 */
void display_timer(jeu* world,SDL_Renderer* renderer){
            char* timer = malloc(sizeof(char)*4);
            if(world->timer.inf){
               sprintf(timer, "%s", "inf");
            } 
            else{
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
            SDL_Color color = {0,0,0,0};
            SDL_Surface* surface_compteur = TTF_RenderText_Solid(world->font.police_compteur, timer, color);
            SDL_Texture* texture_compteur = SDL_CreateTextureFromSurface(renderer,surface_compteur);
            apply_textures(texture_compteur,renderer,610, -5);
            SDL_DestroyTexture(texture_compteur);
            SDL_FreeSurface(surface_compteur);
            free(timer);
}

/**
 * @brief Affiche la boule de feu
 *  
 * @param renderer Le moteur de rendu
 * @param throwable Structure des attaques à distance
 */
void display_fireball(SDL_Renderer* renderer,throwable projectile){
   if(projectile.launched_fireball){
        apply_textures(projectile.fireball,renderer,projectile.x,projectile.y);
    }
}

/**
 * @brief Affiche le trou noir
 *  
 * @param renderer Le moteur de rendu
 * @param world Les données du monde
 */
void display_gravityball(SDL_Renderer* renderer,sprite_perso perso){
    anim * animation = getAnimation(perso.anim, 21);
    if(perso.gravityball.launched_fireball){
        SDL_Rect dst = {0, 0, 0, 0};
        SDL_Rect src = {0, 0, 0, 0};
        dst.x = perso.gravityball.x;
        dst.y = perso.gravityball.y;
        dst.w = animation->width;
        dst.h = 250;

        SDL_QueryTexture(animation->anim_text, NULL, NULL, NULL, &src.h);
        src.x = animation->frame * animation->width;
        src.y = 0;
        src.w = 207;
    
        SDL_RenderCopy(renderer, animation->anim_text, &src, &dst);
    }
}

/**
 * @brief retourne le niveau de la barre de spécial
 *  
 * @param perso Structure de tout ce qui est relatif aux personnages
 */
char* barlvl(sprite_perso perso){
    char* lvl;
    //si il y a la bar chargé 0 fois
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

/**
 * @brief affichage de la barre de spécial
 *  
 * @param renderer Le moteur de rendu
 * @param world Les données du monde
 */
void display_special(SDL_Renderer* renderer, jeu* world){
    char* lvlP1 = barlvl(world->p1);
    char* lvlP2 = barlvl(world->p2);
    
    //la barre de spécial
    SDL_Rect rect;
    SDL_Rect rect_fond;
    rect.x = 500 ;
    rect.y = 30;
    rect.h = 20;

    rect_fond.x = 500 ;
    rect_fond.h = 20;
    rect_fond.y = 30;
    rect_fond.w = -500;

    //le niveau de la barre de spécial
    if(world->p1.special_bar < 300){
        rect.w = (world->p1.special_bar % 100) * -5;
    }
    else{
        rect.w = -500;
    }
    SDL_Color color = {192,191,201,0};
    SDL_Surface* lvl1 = TTF_RenderText_Solid(world->font.police_compteur,lvlP1 , color);
    SDL_Texture* texture_lvl1 = SDL_CreateTextureFromSurface(renderer,lvl1);
    apply_textures(texture_lvl1,renderer,525, 25);
    SDL_Surface* lvl2 = TTF_RenderText_Solid(world->font.police_compteur,lvlP2 , color);
    SDL_Texture* texture_lvl2 = SDL_CreateTextureFromSurface(renderer,lvl2);
    apply_textures(texture_lvl2,renderer,735, 25);

    //barre de spécial du 2nd joueur
    SDL_Rect rect2;
    SDL_Rect rect_fond2;
    rect2.x = SCREEN_WIDTH - 500 ;
    rect2.y = 30;
    rect2.h = 20;
    rect_fond2.x = SCREEN_WIDTH - 500;
    rect_fond2.h = 20;
    rect_fond2.y = 30;
    rect_fond2.w =  500 ;

    if(world->p2.special_bar <300){
        rect2.w = (world->p2.special_bar %100) * 5 ;
    }
    else{
        rect2.w = 500;
    }
     /*RENDERING*/
    SDL_SetRenderDrawColor(renderer, 113, 116 , 128, 1);
    SDL_RenderFillRect(renderer, &rect_fond);
    SDL_RenderFillRect(renderer, &rect_fond2);

    SDL_SetRenderDrawColor(renderer, 26, 4, 229, 0);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_FreeSurface(lvl1);
    SDL_FreeSurface(lvl2);
    SDL_DestroyTexture(texture_lvl1);
    SDL_DestroyTexture(texture_lvl2);
}

/**
 * @brief Affiche la bulle verte de la garde
 *  
 * @param renderer Le moteur de rendu
 * @param perso Les données des personnages
 */
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
    disque(perso.x+perso.w - 20 ,perso.y+perso.h/2,perso.life_guard*0.75,renderer);
}

/**
 * @brief Affiche la vie 
 *  
 * @param renderer Le moteur de rendu
 * @param world Les données du monde
 */
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

/**
 * @brief Applique une texture
 *  
 * @param renderer Le moteur de rendu
 * @param world Les données du monde
 * @param x abcisse
 * @param y ordonné
 */
void apply_textures(SDL_Texture * texture, SDL_Renderer *renderer, int x , int y){
    SDL_Rect dst = {0, 0, 0, 0};

    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);

    dst.x = x; dst.y=y;

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

/**
 * @brief Affiche la map
 *  
 * @param renderer Le moteur de rendu
 * @param world Les données du monde
 */
void display_map(SDL_Renderer* renderer,jeu* world){
    switch(world->choosed_map){
        //différentes maps
        case city_night:
            apply_textures(world->map.image_fond,renderer,0,0);
        break;

        case forest:
            apply_textures(world->map.image_fond,renderer,0,0);
        break;

        case japan:
            apply_textures(world->map.image_fond,renderer,0,0);
        break;
    }
}

/**
 * @brief Affiche les plateformes selon la position sur le quadrillage
 *  
 * @param renderer Le moteur de rendu
 * @param map_struct Structure de la carte
 * @param texture texture de la plateforme
 */
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

    //parcours de la structure de la map
    for(int i = 0; i<40 ; i++){
        for(int j = 0 ; j<20 ; j++){
            
            if(map_struct[i][j] == '5'){
                apply_textures(texture, renderer, j * width_factor, i * height_factor);
            }
        }
    }
}

/**
 * @brief Affiche la texture pour chaque bonus
 *  
 * @param renderer Le moteur de rendu
 * @param lootbox La lootbox à afficher
 */
void render_bonuses(SDL_Renderer * renderer, lootbox * lootbox){
    if(lootbox->active){
        apply_textures(lootbox->texture[lootbox->bonus],renderer,lootbox->x,lootbox->y);
    }
}

/**
 * @brief Affiche un disque
 *  
 * @param cx L'abcisse
 * @param cy L'ordonnée
 * @param rayon le rayon du disque
 * @param renderer Le moteur de rendu
 */
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

/**
 * @brief Affiche une ligne horizontale
 *  
 * @param x abcisse 
 * @param y ordonnée
 * @param w taille
 * @param renderer Le moteur de rendu
 */
void ligneHorizontale(int x, int y, int w, SDL_Renderer* renderer)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;

  SDL_RenderFillRect(renderer,&r);
}

