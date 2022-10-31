
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include "structure/structure.h"
#include "graphics.h"


void init(SDL_Window* window, SDL_Renderer* render){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){ // Initialisation de la SDL
    printf("Erreur d'initialisation de la SDL: %s",SDL_GetError());
    SDL_Quit();
    }

    window = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
    if( window == NULL){ // En cas d’erreur
        printf("Erreur de la creation d’une fenetre: %s",SDL_GetError());
        SDL_Quit();
    }
    render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}

void gameplay_events(SDL_Event *event, jeu *world){
    Uint8 *keystates;
    while(SDL_PollEvent(event)){

        switch(event->type){
            case SDL_QUIT:
            world->terminer = true;
            break;

            case SDL_KEYDOWN:
            switch(event->key.keysym.sym){
                case SDLK_ESCAPE:
                case SDLK_q:
                world->terminer = true;
                break;

                case SDLK_RIGHT:
                world->pers1.x += world->pers1.speed ;
                break;

                case SDLK_LEFT:
                world->pers1.x -= world->pers1.speed;
                break;
            }
        }
    }
}

void init_jeu(jeu *world){
    world->pers1.speed = 10;
    world->terminer = false;
    world->pers1.x = 10;
    world->pers1.y = 10;
}

int main(int argc, char *argv[]){
    SDL_Window* fenetre; // Déclaration de la fenêtre
    SDL_Event events; // Événements liés à la fenêtre
    SDL_Renderer* renderer;
    
    jeu world;
    init_jeu(&world);
    init(fenetre,renderer);


    // Boucle principale
    while(!world.terminer){
        gameplay_events(&events, &world);
        //update_données
        refresh_graphics(renderer,&world);
        SDL_Delay(10);
    }

    // Quitter SDL
    SDL_DestroyWindow(fenetre);
    IMG_Quit;
    SDL_Quit();
    return 0;
}