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

void movements(jeu* world, sprite_perso* perso, sprite_perso* adversaire){
    //printf("X: %d\n", perso->x);
    //printf("Y: %d\n", perso->y);
    //printf("State: %d\n", perso->chara_state);
    //printf("Bckwd: %d\n", backwards);
    int x, y;
    x = perso->x;

    //*pos_init_x = perso->x;

    y = perso->y;

    //Contrôle en marche
    if(perso->chara_state == walk){
        if(canMove(perso, adversaire)){
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
        //if(y + perso->jump_height *0.025 > 720){ // A quoi servait ce test?
        if(y  > 465){ //replace correctement perso sur le sol
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

    if(perso->chara_state == knockback){
        if(!perso->mirror){
            if(!equals(x - perso->speed, y + perso->h, world->map.map_structure, '0')){
                perso->x -= perso->speed;
            }
        }
        else{
            if(!equals(x + perso->speed, y + perso->h, world->map.map_structure, '0')){
                perso->x += perso->speed;
            }
        }
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

void light_punch(sprite_perso* attacker, sprite_perso* receiver){
    printf("%d",!(attacker->hits.light_punch->launch));
    if((!attacker->hits.light_punch->launch)){
        attacker->hits.light_punch->launch = true;
        if(!attacker->mirror){
            if((attacker->x + attacker->w + attacker->hits.light_punch->range_x >= receiver->x) && (attacker->y + attacker->jump_height/2 >= receiver->y && attacker->y + attacker->jump_height/2 <= receiver->y + receiver->h)){
                receiver->life -= attacker->hits.light_punch->dmg ;        
            }
        }
        else{
            if((attacker->x - attacker->hits.light_punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height/2 >= receiver->y && attacker->y + attacker->jump_height/2 <= receiver->y + receiver->h)){
                receiver->life -= attacker->hits.light_punch->dmg ;
            }
        }
    }
    //check_life(receiver);
}

void reset_hit(sprite_perso* perso){
    if(perso->hits.light_punch->launch){
        perso->hits.light_punch->timer++;
        printf("%d", perso->hits.light_punch->timer);
        if(perso->hits.light_punch->timer >= 50){
            perso->hits.light_punch->launch = false;
            perso->hits.light_punch->timer = 0;
        }
    }
}


void change_directions(sprite_perso * p1, sprite_perso * p2){
    /*printf("P1 : %d\n", world->p1.mirror);
    printf("P1 / X : %d\n", world->p1.x);
    printf("P2 : %d\n", world->p2.mirror);
    printf("P2 / X : %d\n", world->p2.x);*/
    if(p1->x > p2->x){
        p1->mirror = true;
        p2->mirror = false;
    }
    else{
        p1->mirror = false;
        p2->mirror = true;
    }   
}

void init_controller(jeu* world){
    int nbJoystick = SDL_NumJoysticks();
    printf("il y a %d controller\n",nbJoystick);
    if(nbJoystick>0){
        world->joysticks = malloc(sizeof(SDL_GameController*)*nbJoystick);
        for(int i = 0 ; i<nbJoystick; i++){
        world->joysticks[i]=SDL_GameControllerOpen(i);
        }
    }
}

bool canMove(sprite_perso * perso, sprite_perso * adversaire){
    bool canMove = true;
    int * yAxis =(int*)malloc(sizeof(int)*2);
    yAxis[0] = perso->y;
    yAxis[1] = perso->y - perso->h;

    if(!perso->mirror && adversaire->mirror){
        if( yAxis[0] >= adversaire->y &&  yAxis[1] <= adversaire->y - adversaire->h ){ 
            if( perso->x + perso->speed >= adversaire->x - adversaire->w && perso->backwards != 1){
                canMove = false;
            }
        }
    }
    else{
        if( yAxis[0] >= adversaire->y &&  yAxis[1] <= adversaire->y - adversaire->h ){
            if(perso->x - perso->speed <= adversaire->x + adversaire->w && perso->backwards == 1){
                canMove = false;
            }
        }
    }
    return canMove; 
}

void sprites_collision(sprite_perso * p1, sprite_perso * p2, jeu * world){ // Cas où les deux sprites sont superposés (suite à par ex un saut), un choc se produit et ils reculent tout les deux si possibles
    int * yAxis =(int*)malloc(sizeof(int)*2);
    yAxis[0] = p1->y;
    yAxis[1] = p1->y - p1->h;
    if(!p1->mirror && p2->mirror){
        if( yAxis[0] >= p2->y &&  yAxis[1] <= p2->y - p2->h ){
            if(p1->x + p1->w >= p2->x && p1 ->backwards != 1){
                printf("COLLISION \n");
                /*if(!equals(p1->x - p1->speed / 2, p1->y + p1->h, world->map.map_structure, '0')){
                    p1->x = p1->x - p1->speed / 2;
                    p2->x = p2->x + p2->speed / 2;
                }
                else{
                    p2->x = p1->x + p1->w;
                }*/
                p1->chara_state = knockback;
                p2->chara_state = knockback;
            }
        }
    }
    else{
        if( yAxis[0] >= p2->y &&  yAxis[1] <= p2->y - p2->h){
            /*if(p1->x - p1->w <= p2->x ){
                printf("COLLISION 2 \n");
                if(!equals(p1->x + p1->speed, p1->y + p1->h, world->map.map_structure, '0')){
                    p1->x = p2->x + p2->w;
                }
                else{
                    p2->x = p1->x - p1->w;
                }
            }*/
            if(p1->x <= p2->x + p2->w && p1 ->backwards == 1){
                p1->chara_state = knockback;
                p2->chara_state = knockback;
            }
        }
    }

}

//fonction pour les miniatures de maps
void init_miniature(jeu* world,SDL_Renderer* renderer){
    world->menu_set.tab_map[0] = load_image("build/map/russia/RussiaMap_mini.png", renderer);
    world->menu_set.tab_map[1] = load_image("build/map/forest/forest_mini.png", renderer);
    world->menu_set.tab_map[2] = load_image("build/map/street_art/street_mini.png", renderer);
}