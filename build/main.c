#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "structure/structure.h"
#include "graphics/graphics.h"
#include "data/data.h"
#include "constante.h"
void quit_game(jeu* world,SDL_Window** fenetre,SDL_Renderer** renderer){
    SDL_DestroyWindow(*fenetre);
    SDL_DestroyRenderer(*renderer);
    destroy_textures(world);
    free_map_structure(world->map.map_structure);
    IMG_Quit;
    SDL_Quit();
}
void free_map_structure(char** map_structure){
    for(int i; i<20;i++){
        free(map_structure[i]);
    }
    free(map_structure);
}
void destroy_textures(jeu* world){
    SDL_DestroyTexture(world->p1.texture_perso);
    SDL_DestroyTexture(world->map.image_fond);
    SDL_DestroyTexture(world->map.plateformes);
}

void init_jeu(jeu *world){
    world->state = combat;
    world->choosed_map = russia;
    world->terminer = false;
    
}
void init_map(jeu* world,SDL_Renderer* renderer){
    switch(world->choosed_map){
        case russia:
            world->map.image_fond = load_image("build/map/russia/RussiaMap.png",renderer);
            world->map.map_structure = read_file_map("build/map/russia/russia_structure");
            world->map.plateformes = load_image("build/map/russia/plateforme.png",renderer);
            world->map.taille_cellule_H = sizeof(world->map.map_structure);
            world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
            break;        
    }
}
void init_perso(SDL_Renderer* renderer, sprite_perso* perso, int x, int y, int w, int h, int speed){ //voir moyen pour charger texture spécifique
    
    perso->x = x;
    perso->y = y;
    perso->w = w;
    perso->h = h;
    perso->speed = CHARA_SPEED;
    perso->jump_height = 300;
    perso->chara_state = idle;
    perso->backwards = false;
    perso->jump_origin = y;
    perso->animation = 0;
    init_texture(renderer, perso);
}

void init(SDL_Window** window, SDL_Renderer** renderer, jeu* world){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){ // Initialisation de la SDL
        printf("Erreur d'initialisation de la SDL: %s",SDL_GetError());
        SDL_Quit();
    }

    *window = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
    if( window == NULL){ // En cas d’erreur
        printf("Erreur de la creation d'une fenetre: %s",SDL_GetError());
        SDL_Quit();
    }
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ){
        printf( "Erreur initialisation de SDL_Image: %s\n", IMG_GetError() );
        SDL_Quit();
    }
    init_jeu(world);
    init_perso(*renderer,&world->p1,65, 465 ,125,232,CHARA_SPEED);
    init_perso(*renderer,&world->p2,1000, 465 ,125,232,CHARA_SPEED);
    init_map(world,*renderer);
}

void gameplay_inputs(SDL_Event *event, jeu *world){
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);;
    while(SDL_PollEvent(event)){
        if(event->type == SDL_QUIT){
            world->terminer = true;
        }
    }   
        if((!keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D]||keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D]) && world->p1.chara_state == walk){
            world->p1.chara_state = idle;
        }
        if((!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]) && world->p2.chara_state == walk){
            world->p2.chara_state = idle;
        }
            //deplacement gauche
        if(keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D]){
            //world->p1.speed = -CHARA_SPEED;
            if(world->p1.chara_state == idle){
                world->p1.chara_state = walk;
                world->p1.backwards = true;
            }
            if(world->p1.chara_state == flight){
                world->p1.chara_state = flight_control;
                world->p1.backwards = true;
            }
            if(world->p1.chara_state == fall){
                world->p1.chara_state = fall_control;
                world->p1.backwards = true;
            }
        }
        if(keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT]){
            //world->p1.speed = -CHARA_SPEED;
            if(world->p2.chara_state == idle){
                world->p2.chara_state = walk;
                world->p2.backwards = true;
            }
            if(world->p2.chara_state == flight){
                world->p2.chara_state = flight_control;
                world->p2.backwards = true;
            }
            if(world->p2.chara_state == fall){
                world->p2.chara_state = fall_control;
                world->p2.backwards = true;
            }
        }

        if(!keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]){
            //world->p1.speed = CHARA_SPEED;
            if(world->p2.chara_state == idle){
                world->p2.chara_state = walk;
                world->p2.backwards = false;
            }
            if(world->p2.chara_state == flight){
                world->p2.chara_state = flight_control;
                world->p2.backwards = false;
            }
            if(world->p2.chara_state == fall){
                world->p2.chara_state = fall_control;
                world->p2.backwards = false;
            }
        }

        //deplacement droite
        if(!keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D]){
            //world->p1.speed = CHARA_SPEED;
            if(world->p1.chara_state == idle){
                world->p1.chara_state = walk;
                world->p1.backwards = false;
            }
            if(world->p1.chara_state == flight){
                world->p1.chara_state = flight_control;
                world->p1.backwards = false;
            }
            if(world->p1.chara_state == fall){
                world->p1.chara_state = fall_control;
                world->p1.backwards = false;
            }
        }

        //sauts
        if(keystates[SDL_SCANCODE_W] && (world->p1.chara_state == idle || world->p1.chara_state == walk)){
            world->p1.chara_state = jump;
        }
        if(keystates[SDL_SCANCODE_UP] && (world->p2.chara_state == idle || world->p2.chara_state == walk)){
            world->p2.chara_state = jump;
        }
        //coups
        if(keystates[SDL_SCANCODE_G] && !keystates[SDL_SCANCODE_U] && !keystates[SDL_SCANCODE_Y]){
        }
        //kicks
        if(!keystates[SDLK_g] && keystates[SDLK_u] && !keystates[SDLK_y]){
        }
        //coup spéciaux si la jauge est pleine
        if(keystates[SDLK_y] && keystates[SDLK_g] && keystates[SDLK_u]){

        }
        movements(world, &world->p1);
        movements(world,&world->p2);
}

int main(int argc, char *argv[]){
    SDL_Window* fenetre; // Déclaration de la fenêtre
    SDL_Renderer* renderer;// Déclaration du renderer
    SDL_Event events; //évenement du jeu
    jeu world; //structure principal
    
    init(&fenetre,&renderer,&world);

    refresh_graphics(renderer,&world);
    // Boucle principale
    while(!world.terminer){
        gameplay_inputs(&events, &world);
        //update_données
        refresh_graphics(renderer,&world);
        SDL_Delay(10);
    }

    // Quitter SDL
    quit_game(&world,&fenetre,&renderer);
    return 0;
}