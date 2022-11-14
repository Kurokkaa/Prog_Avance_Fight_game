#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "structure/structure.h"
#include "graphics/graphics.h"
#include "data/data.h"
#include "constante.h"
void quit_game(jeu* world,SDL_Window** fenetre,SDL_Renderer** renderer){
    SDL_DestroyWindow(*fenetre);
    SDL_DestroyRenderer(*renderer);
    destroy_textures(world);
    free_map_structure(world->map.map_structure);
    close_Joystick(world->joysticks);
    IMG_Quit();
    SDL_Quit();
}
void close_Joystick(jeu* world){
    int nbJoystick = SDL_NumJoysticks();
    if(nbJoystick>0){
        printf("%i",nbJoystick);
        for(int i = 0; i<nbJoystick;i++){
        SDL_GameControllerClose(world->joysticks[i]);
        }
        free(world->joysticks);
    }
    
}
void free_map_structure(char** map_structure){
    for(int i = 0; i<40;i++){
        free(map_structure[i]);
    }
    free(map_structure);
}
void destroy_textures(jeu* world){
    SDL_DestroyTexture(world->p1.texture_perso);
    SDL_DestroyTexture(world->map.image_fond);
    SDL_DestroyTexture(world->map.plateformes);
}

void init_jeu(jeu *world){
    world->state = combat;
    world->choosed_map = russia;
    world->terminer = false;
    
}
void init_map(jeu* world,SDL_Renderer* renderer){
    switch(world->choosed_map){
        case russia:
            world->map.image_fond = load_image("build/map/russia/RussiaMap.png",renderer);
            world->map.map_structure = read_file_map("build/map/russia/russia_structure");
            world->map.plateformes = load_image("build/map/russia/plateforme.png",renderer);
            world->map.taille_cellule_H = sizeof(world->map.map_structure);
            world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
            break;        
    }
}
void init_perso(SDL_Renderer* renderer, sprite_perso* perso, int x, int y, int w, int h, int speed,bool mirror){ //voir moyen pour charger texture spécifique
    
    perso->x = x;
    perso->y = y;
    perso->w = w;
    perso->h = h;
    perso->speed = CHARA_SPEED;
    perso->jump_height = 300;
    perso->chara_state = idle;
    perso->backwards = false;
    perso->jump_origin = y;
    perso->animation = 0;
    perso->mirror = mirror;
    perso->life = 20;
    init_hits(perso);
    init_texture(renderer, perso);
}

void init_hits(sprite_perso* perso){
    perso->hits.punch.dmg = 2;
    perso->hits.punch.speed = 0;
    perso->hits.punch.range_x = 250;
    perso->hits.punch.range_y = 0;
    perso->hits.punch.frame = 0;
    perso->hits.punch.animation = 0;
    perso->hits.punch.launch = false;
    perso->hits.punch.timer = 0;

    perso->hits.kick.dmg = 1;
    perso->hits.kick.speed = 0;
    perso->hits.kick.range_x = 400;
    perso->hits.kick.range_y = 0;
    perso->hits.kick.frame = 0;
    perso->hits.kick.animation = 0;
    perso->hits.kick.launch = false;
    perso->hits.kick.timer = 0;
}

void init(SDL_Window** window, SDL_Renderer** renderer, jeu* world){
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1){ // Initialisation de la SDL
        printf("Erreur d'initialisation de la SDL: %s",SDL_GetError());
        SDL_Quit();
    }
    

    *window = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
    if( window == NULL){ // En cas d’erreur
        printf("Erreur de la creation d'une fenetre: %s",SDL_GetError());
        SDL_Quit();
    }
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ){
        printf( "Erreur initialisation de SDL_Image: %s\n", IMG_GetError() );
        SDL_Quit();
    }
    init_jeu(world);
    init_perso(*renderer,&world->p1,65, 465 ,125,232,CHARA_SPEED,false);
    init_perso(*renderer,&world->p2,1000, 465 ,125,232,CHARA_SPEED,true);
    init_map(world,*renderer);
    init_controller(world);
}
init_controller(jeu* world){
    int nbJoystick = SDL_NumJoysticks();
    printf("il y a %d controller",nbJoystick);
    if(nbJoystick>0){
        world->joysticks = malloc(sizeof(SDL_GameController*)*nbJoystick);
        for(int i = 0 ; i<nbJoystick; i++){
        world->joysticks[i]=SDL_GameControllerOpen(i);
    }
    }
   
}
void checkJoystick(SDL_Joystick** joysticks){
    SDL_GameControllerUpdate(); // Mise à jour de l'état des controller
    int i;
    for(int j =0; j<SDL_NumJoysticks();j++){
        for(i = 0 ;i<20;i++){
            if(SDL_GameControllerGetButton(joysticks[j],i)){
                printf("bouton %d presse",i);
            }
        }
    }
    printf("joystick X :%d \n" ,SDL_GameControllerGetAxis(joysticks[0],0));
    printf("joystick Y : %d \n",SDL_GameControllerGetAxis(joysticks[0],1));
}


void gameplay_inputs(SDL_Event *event, jeu *world){
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_JoystickUpdate();
   // checkJoystick(world->joysticks);
    reset_hit(&world->p1);
    reset_hit(&world->p2);
    while(SDL_PollEvent(event)){
        if(event->type == SDL_QUIT){
            world->terminer = true;
        }
    }   
        if((!keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D]||keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D]) && world->p1.chara_state == walk){
            world->p1.chara_state = idle;
        }
        if((!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]) && world->p2.chara_state == walk){
            world->p2.chara_state = idle;
        }
            //deplacement gauche
        if((keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D]) /*|| SDL_GameControllerGetAxis(world->joysticks[0],0)<-(10000)*/){
            //world->p1.speed = -CHARA_SPEED;
            
            if(world->p1.chara_state == idle){
                world->p1.chara_state = walk;
                world->p1.backwards = true;
            }
            if(world->p1.chara_state == flight){
                world->p1.chara_state = flight_control;
                world->p1.backwards = true;
            }
            if(world->p1.chara_state == fall){
                world->p1.chara_state = fall_control;
                world->p1.backwards = true;
            }
        }
        if(keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT]){
            //world->p1.speed = -CHARA_SPEED;
            if(world->p2.chara_state == idle){
                world->p2.chara_state = walk;
                world->p2.backwards = true;
            }
            if(world->p2.chara_state == flight){
                world->p2.chara_state = flight_control;
                world->p2.backwards = true;
            }
            if(world->p2.chara_state == fall){
                world->p2.chara_state = fall_control;
                world->p2.backwards = true;
            }
        }

        if(!keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]){
            //world->p1.speed = CHARA_SPEED;
            if(world->p2.chara_state == idle){
                world->p2.chara_state = walk;
                world->p2.backwards = false;
            }
            if(world->p2.chara_state == flight){
                world->p2.chara_state = flight_control;
                world->p2.backwards = false;
            }
            if(world->p2.chara_state == fall){
                world->p2.chara_state = fall_control;
                world->p2.backwards = false;
            }
        }

        //deplacement droite
        if(!keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D]/*|| SDL_GameControllerGetAxis(world->joysticks[0],0)>10000*/){
            //world->p1.speed = CHARA_SPEED;
            if(world->p1.chara_state == idle){
                world->p1.chara_state = walk;
                world->p1.backwards = false;
            }
            if(world->p1.chara_state == flight){
                world->p1.chara_state = flight_control;
                world->p1.backwards = false;
            }
            if(world->p1.chara_state == fall){
                world->p1.chara_state = fall_control;
                world->p1.backwards = false;
            }
        }

        //sauts
        if((keystates[SDL_SCANCODE_W] /*|| SDL_GameControllerGetAxis(world->joysticks[0],1)< (-10000)*/) && (world->p1.chara_state == idle || world->p1.chara_state == walk)){
            world->p1.chara_state = jump;
        }
        if(keystates[SDL_SCANCODE_UP] && (world->p2.chara_state == idle || world->p2.chara_state == walk)){
            world->p2.chara_state = jump;
        }
        //coups
        if(keystates[SDL_SCANCODE_G] && !keystates[SDL_SCANCODE_U] && !keystates[SDL_SCANCODE_Y]){
            punch(&world->p1, &world->p2);
        }
        //kicks
        if(!keystates[SDL_SCANCODE_G] && keystates[SDL_SCANCODE_U] && !keystates[SDL_SCANCODE_Y]){
            //special(world->p1, world->p2);
        }
        //coup spéciaux si la jauge est pleine
        if(keystates[SDL_SCANCODE_Y] && !keystates[SDL_SCANCODE_G] && !keystates[SDL_SCANCODE_U]){
           // kick(world->p1, world->p2);
        }

         //coups
        if(keystates[SDL_SCANCODE_KP_4] && !keystates[SDL_SCANCODE_KP_8] && !keystates[SDL_SCANCODE_KP_9]){
            punch(&world->p2, &world->p1);
        }
        //kicks
        if(!keystates[SDL_SCANCODE_KP_4] && keystates[SDL_SCANCODE_KP_8] && !keystates[SDL_SCANCODE_KP_9]){
            //special(world->p2, world->p1);
        }
        //coup spéciaux si la jauge est pleine
        if(!keystates[SDL_SCANCODE_KP_4] && !keystates[SDL_SCANCODE_KP_8] && keystates[SDL_SCANCODE_KP_9]){
           // kick(world->p2, world->p1);
        }
        movements(world, &world->p1);
        movements(world,&world->p2);
        change_directions(world);
}

void change_directions(jeu* world){
    if(world->p1.x>world->p2.x){
        world->p1.mirror = true;
        world->p2.mirror = false;
        if(world->p1.x == world->p2.x+world->p2.w && world->p2.y == world->p1.y){
            world->p1.x+=CHARA_SPEED;
            world->p2.x-=CHARA_SPEED;
        }
    }
    else{
        world->p1.mirror = false;
        world->p2.mirror = true;
        if(world->p1.x+world->p1.w == world->p2.x && world->p2.y == world->p1.y){
            world->p1.x-=CHARA_SPEED;
            world->p2.x+=CHARA_SPEED;
        }
    }
    
}
int main(int argc, char *argv[]){
    SDL_Window* fenetre; // Déclaration de la fenêtre
    SDL_Renderer* renderer;// Déclaration du renderer
    SDL_Event events; //évenement du jeu
    jeu world; //structure principal    
    init(&fenetre,&renderer,&world);
    
    refresh_graphics(renderer,&world);
    // Boucle principale
    while(!world.terminer){
        gameplay_inputs(&events, &world);
        //update_données
        refresh_graphics(renderer,&world);
        SDL_Delay(10);
    }

    // Quitter SDL
    //quit_game(&world,&fenetre,&renderer);
    return 0;
}