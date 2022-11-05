#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../constante.h"
#include "../map/russia/map_russia.h"

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

struct maps{
    Russia map_r;
    
};

typedef struct maps map;

struct world_s{
    enum game_state state;
    bool terminer; /*!< Champ indiquant si l'on est à la fin du jeu */
    sprite_perso p1;
    enum map_choice choice;
    map maps;
};

typedef struct world_s jeu;




#endif