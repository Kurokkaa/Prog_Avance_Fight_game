#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


void init(SDL_Window** window, SDL_Renderer** render){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) // Initialisation de la SDL
    {
    printf("Erreur d’initialisation de la SDL: %s",SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
    if(fenetre == NULL) // En cas d’erreur
    {
        printf("Erreur de la creation d’une fenetre: %s",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    render = SDL_createRenderer(*window,-1,SDL_RENDERER_ACCELERATED);


}

int main(int argc, char *argv[])
{


    SDL_Window* fenetre; // Déclaration de la fenêtre
    SDL_Event evenements; // Événements liés à la fenêtre
    SDL_Renderer* renderer; 

    bool terminer = false;


    


    // Créer la fenêtre
    fenetre = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
    if(fenetre == NULL) // En cas d’erreur
    {
        printf("Erreur de la creation d’une fenetre: %s",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }



    // Boucle principale
    while(!terminer){
        SDL_PollEvent( &evenements );
    switch(evenements.type)
    {
    case SDL_QUIT:
    terminer = true; break;
    case SDL_KEYDOWN:
    switch(evenements.key.keysym.sym)
    {
    case SDLK_ESCAPE:
    case SDLK_q:
    terminer = true; break;
    }
    }
    }
    // Quitter SDL
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    return 0;
}