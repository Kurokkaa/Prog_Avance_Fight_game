#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

/*struct personnage{
};

enum perso{p1,p2};
*/

struct sprite_p{
    SDL_Texture * texture_perso;
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
    sprite_perso p1;
};

typedef struct world_s jeu;




#endif