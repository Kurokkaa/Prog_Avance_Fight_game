/**
 * \file structure.h
 * \brief Header d'un module pour la gestion et modification des structures du jeu
 * \author John STACCIONI/ Théo Barrier
 * \version 1.0
 * \date 07 avril 2022
 */

#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

struct sprite_p{
    SDL_Texture * text_perso;
    int x;
    int y;
    int w; /*!<largeur du sprite */
    int h; /*!<hauteur du sprite */
    int speed;
    int perso;
};

typedef struct sprite_p sprite_perso ;

enum state{combat, menu, pause, options};

struct world_s{
    enum state etat;
    bool terminer; /*!< Champ indiquant si l'on est à la fin du jeu */
    sprite_perso pers1;
};

typedef struct world_s jeu;




#endif