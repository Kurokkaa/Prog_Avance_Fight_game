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
    //printf("X: %d\n", perso->x);
    //printf("Y: %d\n", perso->y);
    //printf("State: %d\n", perso->chara_state);
    //printf("Bckwd: %d\n", backwards);
    int x, y;
    x = perso->x;
    y = perso->y;

    //Contrôle en marche
    if(perso->chara_state == walk){
            if(perso->backwards){
                if(!equals(x - perso->speed, y + perso->h, world->map.map_structure, '0')){
                    perso->x -= perso->speed;
                }
            }
            else{
                if(!equals(x + perso->speed, y + perso->h, world->map.map_structure, '0')){
                    perso->x += perso->speed;
                }
            }
    }

    //Contrôle en vol
    if(perso->chara_state == flight_control){
        if(perso->backwards){
                if(equals(x - perso->speed, y + perso->h, world->map.map_structure, ' ')){
                    perso->x -= perso->speed * 0.2;
                }
            }
            else{
                if(equals(x + perso->speed, y + perso->h, world->map.map_structure, ' ')){
                    perso->x += perso->speed * 0.2;
                }
        }
        perso->chara_state = flight;
    }

    //Contrôle en chute
    if(perso->chara_state == fall_control){
        if(perso->backwards){
                if(equals(x - perso->speed, y + perso->h, world->map.map_structure, ' ')){
                    perso->x -= perso->speed * 0.4;
                }
            }
            else{
                if(equals(x + perso->speed, y + perso->h, world->map.map_structure, ' ')){
                    perso->x += perso->speed * 0.4;
                }
        }
        perso->chara_state = fall;
    }

    //Initialisation du saut
    if(perso->chara_state == jump){
        perso->jump_origin = perso->y;
        perso->chara_state = flight;
    }

    //Contrôle de la hauteur du saut
    if(perso->chara_state == flight){
        if(perso->y > perso->jump_origin - perso->jump_height){
            if(!equals(x, y, world->map.map_structure, '0')){
                perso->y -= perso->jump_height*0.025;
            }
            else{
                perso->chara_state = fall;
            }
        }
        else{
            perso->chara_state = fall;
        }
    }

    //Automatisaion de la chute
    if((perso->chara_state == fall)){
        x = perso->x;
        y = perso->y;
        if(y + perso->jump_height *0.025 > 720){
            perso->y = 465;
            perso->chara_state = landing;
        }
        else{
            if(equals(x, y + perso->h + perso->jump_height*0.025, world->map.map_structure, ' ')){
                perso->y += perso->jump_height*0.025;
            }
            else{
                //printf("COLLISION\n");
                perso->chara_state = landing;
            }
        }
    }

    //Cas où le personnage chute s'il n'y a pas de plateformes
    x = perso->x;
    y = perso->y;
    if(perso->chara_state !=flight && equals(x, y + perso->h + perso->jump_height *0.025 , world->map.map_structure, ' ') && equals(x + perso->x, y + perso->h + perso->jump_height *0.025 , world->map.map_structure, ' ')){
        perso->chara_state = fall;
    }

    if(perso->chara_state == landing){
       // printf("idle: %d\n", perso->animation);
        perso->animation++;
        if(perso->animation >= 20){
            perso->chara_state = idle;
            perso->animation = 0;
        }
    }
}

bool equals(int x, int y, char** map_point, char test){
    int height_factor, width_factor;
    height_factor = CELL_HEIGHT;
    width_factor = CELL_WIDTH;
    return (map_point[y/height_factor][x/width_factor] == test);
}

void punch(sprite_perso* attacker, sprite_perso* receiver){
    printf("%d",!(attacker->hits.punch->launch));
    if((!attacker->hits.punch->launch)){
        attacker->hits.punch->launch = true;
        if(!attacker->mirror){
            if((attacker->x + attacker->w + attacker->hits.punch->range_x >= receiver->x) && (attacker->y + attacker->jump_height/2 >= receiver->y && attacker->y + attacker->jump_height/2 <= receiver->y + receiver->h)){
                receiver->life -= attacker->hits.punch->dmg ;        
            }
        }
        else{
            if((attacker->x - attacker->hits.punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height/2 >= receiver->y && attacker->y + attacker->jump_height/2 <= receiver->y + receiver->h)){
                receiver->life -= attacker->hits.punch->dmg ;
            }
        }
    }
    //check_life(receiver);
}

void reset_hit(sprite_perso* perso){
    if(perso->hits.punch->launch){
        perso->hits.punch->timer++;
        printf("%d", perso->hits.punch->timer);
        if(perso->hits.punch->timer >= 50){
            perso->hits.punch->launch = false;
            perso->hits.punch->timer = 0;
        }
    }
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

void init_controller(jeu* world){
    int nbJoystick = SDL_NumJoysticks();
    printf("il y a %d controller",nbJoystick);
    if(nbJoystick>0){
        world->joysticks = malloc(sizeof(SDL_GameController*)*nbJoystick);
        for(int i = 0 ; i<nbJoystick; i++){
        world->joysticks[i]=SDL_GameControllerOpen(i);
        }
    }
   
}