#ifndef dojo_h
#define dojo_h



struct map_russia{
    char** map_structure;
    int taille_cellule_H;
    int taille_cellule_W;
    SDL_Texture* image_fond;
    SDL_Texture* plateformes;
};

typedef struct map_russia Russia;

#endif