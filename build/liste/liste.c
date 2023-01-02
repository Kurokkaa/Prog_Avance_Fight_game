#include "liste.h"

bool isEmpty(list_anim * liste){
    return (liste == NULL)? 1 : 0;
}

void freeList( list_anim * liste){
    if(isEmpty(liste->next)){
        SDL_DestroyTexture(liste->animation.anim_text);
        free(liste);
    }
    else{
        freeList(liste->next);
    }
}

list_anim * createAnimList(SDL_Renderer *renderer, sprite_perso *perso, char *path, int nbFrame, int width){
    //Alloc mémoire
    list_anim * list = (list_anim*)malloc(sizeof(list_anim));

    //Test si la liste créée est dûe à une erreur alloc mémoire
    if(list == NULL){
        printf("Erreur allocation mémoire pour nouvelle liste.\n");
        return NULL;
    }

    //Initialisation 
    anim animation;
    animation.anim_text = load_image(path, renderer);
    animation.aura = 0;
    animation.counter = 0;
    animation.frame = 0;
    animation.nbFrame = nbFrame;
    animation.width= width;
    list->animation = animation;
    list->next = NULL;
    return list;
}


int insertAnimLast(list_anim * list, SDL_Renderer *renderer, sprite_perso *perso, char *path, int nbFrame, int width){
    //Alloc mémoire
    list_anim * last = malloc(sizeof(list_anim));

    //Test si la liste créée est dûe à une erreur alloc mémoire
    if(last == NULL){
        printf("Erreur allocation mémoire pour nouvelle liste.\n");
        return -1;
    }

    //Initialisation
    else{
        anim animation;
        animation.anim_text = load_image(path, renderer);
        animation.aura = 0;
        animation.counter = 0;
        animation.frame = 0;
        animation.nbFrame = nbFrame;
        animation.width= width;
        last->animation = animation;
        last->next = NULL;

        if(list == NULL){
            list = last;
        }
        else{
            list_anim * searchLast = list;
            while(searchLast->next !=NULL){
                searchLast = searchLast->next; 
            }
            searchLast->next = last;
        }
    }
    return 0;
}

/**
 * @brief Retourne la taille d'une liste
 * 
 * @param list dont on veut la taille
 * @return int taille de la liste
 */
int getSize(list_anim * list){
    if(isEmpty(list)){
        return 0;
    }
    return 1 + getSize(list->next);
}

/**
 * @brief Retourne une animation contenu dans la liste
 * 
 * @param list qui contient l'animation recherchée
 * @param state qu'on recherche
 * @return anim* 
 */
anim * getAnimation(list_anim * list, int state){
    anim animation;
    if (list == NULL) {
        return NULL;
    }
    if (state >= getSize(list)){
        printf("Index recherché supérieur à la taille de la liste.\n");
        return NULL;
    }
    if(state == 0){
        return &list->animation;
    }
    return getAnimation(list->next, state -1);
}

/**
 * @brief Change le 
 * 
 * @param list 
 * @param index 
 */
void setAura(list_anim * list, int index){
    if(!isEmpty(list)){
            if (index >= getSize(list)){
            printf("Index recherché supérieur à la taille de la liste.\n");
        }
        if(index == 0){
            list->animation.aura = !list->animation.aura ;
        }
        setAura(list->next, index - 1);
    }
}

/**
 * @brief Set the Frame object
 * 
 * @param list 
 * @param index 
 * @param frame 
 */
void setFrame(list_anim * list, int index, int frame){
    if(!isEmpty(list)){
            if (index >= getSize(list)){
            printf("Index recherché supérieur à la taille de la liste.\n");
        }
        if(index == 0){
            list->animation.frame = frame ;
        }
        setFrame(list->next, index - 1, frame);
    }
}

/**
 * @brief Set the Counter object
 * 
 * @param list 
 * @param index 
 * @param counter 
 */
void setCounter(list_anim * list, int index, int counter){
    if(!isEmpty(list)){
        if (index >= getSize(list)){
            printf("Index recherché supérieur à la taille de la liste.\n");
        }
        if(index == 0){
            list->animation.counter = counter ;
        }
        setCounter(list->next, index - 1, counter);
    }
}