#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>
#include "structure/structure.h"
#include "graphics/graphics.h"
#include "data/data.h"
#include "constante.h"


int main(int argc, char *argv[]){
    SDL_Window* fenetre; // Déclaration de la fenêtre
    SDL_Renderer* renderer;// Déclaration du renderer
    SDL_Event events; //évenement du jeu
    jeu world; //structure principal   
   
    init(&fenetre,&renderer,&world);
    
    refresh_graphics(renderer,&world);
    // Boucle principale
    while(!world.terminer){
        if(world.state == combat){
            gameplay_inputs(&events, &world);
            update_data(&world);
        }
        else{
            handle_menu_inputs(&events, &world, renderer);
        }
       
        refresh_graphics(renderer,&world);
        SDL_Delay(10);
    }
    // Quitter SDL
    quit_game(&world,&fenetre,&renderer);
    return 0;
}
