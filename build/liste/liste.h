
#ifndef liste_h
#define liste_h

#include "../structure/structure.h"
#include <SDL2/SDL.h>
#include "../data/data.h"

bool isEmpty(list_anim * liste);
list_anim * createAnimList(SDL_Renderer *renderer, sprite_perso *perso, char *path, int nbFrame, int width);
void freeList( list_anim * liste);
int insertAnimLast(list_anim * list, SDL_Renderer *renderer, sprite_perso *perso, char *path, int nbFrame, int width);
int getSize(list_anim * list);
anim * getAnimation(list_anim * list, int state);
void setAura(list_anim * list, int index);
void setFrame(list_anim * list, int index, int frame);
void setCounter(list_anim * list, int index, int counter);
void testList(list_anim * list);

#endif