#include "data.h"
#include "../constante.h"


char** read_file_map(char* nom){
    FILE* file = fopen(nom,"r");
    char** map_struct = malloc(sizeof(char*)*40);
    for(int i =0 ;i<40;i++){
        map_struct[i]=malloc(sizeof(char)*20);
        fread(map_struct[i],20,1,file);
    }
    if(map_struct == NULL){
        printf("Erreur de chargement de structure");
    }
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

void movements(jeu* world, sprite_perso* perso){
    int x, y;
    x = perso->x + perso->speed;
    y = perso->y;
    if(equals(x, y + perso->h, world->map.map_structure, ' ') && perso->chara_state != flight){
        perso->x = x;
    }

    if(perso->chara_state == jump){
        perso->jump_origin = perso->y;
        perso->chara_state = flight;
    }

    if(perso->chara_state == flight){
        if(perso->y > perso->jump_origin - perso->jump_height){
            printf("Y: %d\n", perso->y);
            if(!equals(x, y + perso->h, world->map.map_structure, '0')){
                perso->y -= perso->jump_height*0.005;
            }
        }
        else{
            perso->chara_state = fall;
        }
    }

    if((perso->chara_state == fall)){
        x = perso->x + perso->speed;
        y = perso->y;
        if(equals(x, y + perso->h, world->map.map_structure, ' ')){
            perso->y += perso->jump_height*0.005;
        }
        else{
            perso->chara_state = idle;
        }
    }
}

bool equals(int x, int y, char** map_point, char test){
    int height_factor, width_factor;
    height_factor = CELL_HEIGHT;
    width_factor = CELL_WIDTH;
    return (map_point[y/height_factor][x/width_factor] == test);
}


