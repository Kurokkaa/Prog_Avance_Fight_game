#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include "structure/structure.h"
#include "graphics.h"

void init_jeu(jeu *world){
    world->terminer = false;
}

void init_perso(SDL_Renderer* renderer, jeu* world, int x, int y, int w, int h, int speed){ //voir moyen pour charger texture spécifique
    world->p1.x = x;
    world->p1.y = y;
    world->p1.w = w;
    world->p1.h = h;
    world->p1.speed = speed;
    init_texture(renderer, &world->p1);
}

void init(SDL_Window* window, SDL_Renderer* renderer, jeu* world){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){ // Initialisation de la SDL
        printf("Erreur d'initialisation de la SDL: %s",SDL_GetError());
        SDL_Quit();
    }

    window = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
    if( window == NULL){ // En cas d’erreur
        printf("Erreur de la creation d'une fenetre: %s",SDL_GetError());
        SDL_Quit();
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ){
        printf( "Erreur initialisation de SDL_Image: %s\n", IMG_GetError() );
        SDL_Quit();
    }
    init_jeu(world);
    init_perso(renderer,world,10,10,25,100,10);
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
                world->p1.x += world->p1.speed ;
                break;

                case SDLK_LEFT:
                world->p1.x -= world->p1.speed;
                break;
            }
        }
    }
}

int main(int argc, char *argv[]){
    SDL_Window* fenetre; // Déclaration de la fenêtre
    SDL_Event events; // Événements liés à la fenêtre
    SDL_Renderer* renderer;
    
    jeu world;
    
    init(fenetre,renderer,&world);


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