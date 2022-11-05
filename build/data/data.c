#include "data.h"



char** read_file_map(char* nom){
    
    FILE* file = fopen(nom,"r");
    char** map_struct = malloc(sizeof(char*)*40);
    for(int i =0 ;i<40;i++){
        map_struct[i]=malloc(sizeof(char)*20);
    }
    fread(map_struct,20,40,file);
    return map_struct;
}




SDL_Texture* load_image( char path[],SDL_Renderer *renderer){
    SDL_Surface* tmp = NULL;
    SDL_Texture* texture = NULL;
    tmp = IMG_Load(path);
    if(NULL == tmp){
        fprintf(stderr, "Erreur pendant chargement image BMP: %s", SDL_GetError());
        return NULL;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture){
        fprintf(stderr, "Erreur pendant creation de la texture liee a l'image chargee: %s", SDL_GetError());
        return NULL;
    }
    return texture;
}