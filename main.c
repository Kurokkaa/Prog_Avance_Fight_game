#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL.h>

int main(int argc, char** argv){
    bool play = true;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stdout,"Erreur du lancement de SDL %s\n",SDL_GetError());
        return play=false;
    }

    //initialisation de la fenêtre
    SDL_Window* window = NULL;
    Window = SDL_CreateWindow("battle_game", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,488,192,SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);
    SDL_Surface *image = SDL_LoadPNG("./ressources/fond1.png");
    SDL_Texture *

    while (play){
        
    }
    if(!play){
        return EXIT_SUCCESS;
    }
    
}