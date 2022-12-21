#include "data.h"
#include "../constante.h"

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

void init_map(jeu* world,SDL_Renderer* renderer){
    switch(world->choosed_map){
        case russia:
            world->map.image_fond = load_image("build/map/city_night/city.bmp",renderer);
            world->map.map_structure = read_file_map("build/map/russia/russia_structure");
            world->map.plateformes = load_image("build/map/russia/plateforme.png",renderer);
            world->map.taille_cellule_H = sizeof(world->map.map_structure);
            world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
            break;        
        case forest:
            world->map.image_fond = load_image("build/map/forest/forest.png",renderer);
            world->map.map_structure = read_file_map("build/map/forest/forest_structure");
            world->map.plateformes = load_image("build/map/forest/plateforme.png",renderer);
            world->map.taille_cellule_H = sizeof(world->map.map_structure);
            world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
            break;  
        case street_art:
            world->map.image_fond = load_image("build/map/street_art/street.png",renderer);
            world->map.map_structure = read_file_map("build/map/street_art/street_structure");
            world->map.plateformes = load_image("build/map/street_art/plateforme.png",renderer);
            world->map.taille_cellule_H = sizeof(world->map.map_structure);
            world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
            break;  
    }
}

//fonction pour les miniatures de maps
void init_miniature(jeu* world,SDL_Renderer* renderer){
    world->menu_set.tab_map[0] = load_image("build/map/russia/RussiaMap_mini.png", renderer);
    world->menu_set.tab_map[1] = load_image("build/map/forest/forest_mini.png", renderer);
    world->menu_set.tab_map[2] = load_image("build/map/street_art/street_mini.png", renderer);
}


void init_jeu(jeu *world, SDL_Renderer* renderer){
    world->menu_set.menu_fond = load_image("build/ressources/menu/menu.png", renderer);
    world->state = main_menu;
    world->terminer = false;
    world->menu_set.index_menu = 0;
}

void init_perso(SDL_Renderer* renderer, sprite_perso* perso, int x, int y, int w, int h, int speed,bool mirror){ //voir moyen pour charger texture spécifique  
    perso->perso_choisi = 0;
    perso->x = x;
    perso->y = y;
    perso->w = w;
    perso->h = h;
    perso->speed = CHARA_SPEED;
    perso->jump_height = 300;
    perso->chara_state = idle;
    perso->backwards = false;
    perso->jump_origin = y;
    perso->anim.frame = 0;
    perso->mirror = mirror;
    perso->life = 20;
    init_hits(perso);
    init_texture(renderer, perso);
    perso->pos_tab_combo = 0;
    perso->buffer = malloc(sizeof(inputs)*60);
    init_state_animations(renderer, perso);
}

void init_hits(sprite_perso* perso){
    perso->hits.light_punch = malloc(sizeof(hit));
    perso->hits.heavy_punch = malloc(sizeof(hit));
    perso->hits.kick = malloc(sizeof(hit));
    perso->hits.special_attack = malloc(sizeof(hit));

    perso->hits.light_punch->dmg = 2;
    perso->hits.light_punch->speed = 0;
    perso->hits.light_punch->range_x = 250;
    perso->hits.light_punch->range_y = 0;
    perso->hits.light_punch->frame = 0;
    perso->hits.light_punch->animation = 0;
    perso->hits.light_punch->launch = false;
    perso->hits.light_punch->timer = 0;
    perso->hits.light_punch->delay = 50;


    perso->hits.heavy_punch->dmg = 2;
    perso->hits.heavy_punch->speed = 0;
    perso->hits.heavy_punch->range_x = 250;
    perso->hits.heavy_punch->range_y = 0;
    perso->hits.heavy_punch->frame = 0;
    perso->hits.heavy_punch->animation = 0;
    perso->hits.heavy_punch->launch = false;
    perso->hits.heavy_punch->timer = 0;
    perso->hits.heavy_punch->delay = 50;

    perso->hits.kick->dmg = 1;
    perso->hits.kick->speed = 0;
    perso->hits.kick->range_x = 400;
    perso->hits.kick->range_y = 0;
    perso->hits.kick->frame = 0;
    perso->hits.kick->animation = 0;
    perso->hits.kick->launch = false;
    perso->hits.kick->timer = 0;
    perso->hits.kick->delay = 20;

    perso->hits.special_attack->dmg = 1;
    perso->hits.special_attack->speed = 0;
    perso->hits.special_attack->range_x = 400;
    perso->hits.special_attack->range_y = 0;
    perso->hits.special_attack->frame = 0;
    perso->hits.special_attack->animation = 0;
    perso->hits.special_attack->launch = false;
    perso->hits.special_attack->timer = 0;
    perso->hits.special_attack->delay = 20;
}

void init_combo(sprite_perso* player){
    //1er combo
    player->tab_combo[0].nb_coups = 3;
    player->tab_combo[0].input[0] = right;
    player->tab_combo[0].input[1] = left;
    player->tab_combo[0].input[2] = down;
}
  
void init(SDL_Window** window, SDL_Renderer** renderer, jeu* world){
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1){ // Initialisation de la SDL
        printf("Erreur d'initialisation de la SDL: %s",SDL_GetError());
        SDL_Quit();
    }
    
    *window = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if( window == NULL){ // En cas d’erreur
        printf("Erreur de la creation d'une fenetre: %s",SDL_GetError());
        SDL_Quit();
    }
    //SDL_SetWindowFullscreen(*window,SDL_WINDOW_FULLSCREEN);

    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED);
    
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ){
        printf( "Erreur initialisation de SDL_Image: %s\n", IMG_GetError() );
        SDL_Quit();
    }
    init_jeu(world, *renderer);
}

void init_state_animations(SDL_Renderer* renderer, sprite_perso* perso){
    switch (perso->perso_choisi)
    {
    case 0:
        perso->anim.anim_text[idle] = load_image("build/ressources/Characters/Chara1/Yellow_idle_spritesheet.png", renderer);
        perso->anim.frame = 0;
        break;
    
    default:
        perso->anim.anim_text[idle] = load_image("build/ressources/Characters/Chara1/Yellow_idle_spritesheet.png", renderer);
        break;
    }
}


/**
 * FONCTIONS DE CHARGEMENTS ET LECTURES
*/

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

bool read_combo(sprite_perso* player, int val){
    //1er combo
    bool found = false;
    bool equal = true;
    for(int i = 0 ; i<BUFFER_SIZE - player->tab_combo[val].nb_coups && !found ; i++){
        for(int j = i; j<i+player->tab_combo[val].nb_coups && equal ; j++){
            if(player->tab_combo[val].input[j] != player->buffer[j].input){
                equal = false;
            }
        }
        if(equal){
            found = true;
            for(int i = 0; i<BUFFER_SIZE ; i++){
                player->buffer[i].input = 0;
                player->buffer[i].timestamp = 0;
            }
            player->pos_tab_combo=0;
        }
    }
    return found;
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

/**
 * FONCTIONS MOUVEMENTS + COUPS
*/
void movements(jeu* world, sprite_perso* perso, sprite_perso* adversaire){
    int x, y;
    x = perso->x;
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
        if(y  > 465){ //replace correctement perso sur le sol
            perso->y = 465;
            perso->chara_state = landing;
        }
        else{
            if(equals(x, y + perso->h + perso->jump_height*0.025, world->map.map_structure, ' ')){
                perso->y += perso->jump_height*0.025;
            }
            else{
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
        perso->anim.frame++;
        if(perso->anim.frame >= 20){
            perso->chara_state = idle;
            perso->anim.frame = 0;
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
        perso->anim.frame++;
        if(perso->anim.frame >= 20){
            perso->chara_state = idle;
            perso->anim.frame = 0;
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
        if(perso->hits.light_punch->timer >= 50){
            perso->hits.light_punch->launch = false;
            perso->hits.light_punch->timer = 0;
        }
    }
}


void change_directions(sprite_perso * p1, sprite_perso * p2){
    if(p1->x > p2->x){
        p1->mirror = true;
        p2->mirror = false;
    }
    else{
        p1->mirror = false;
        p2->mirror = true;
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
                p1->chara_state = knockback;
                p2->chara_state = knockback;
            }
        }
    }
    else{
        if( yAxis[0] >= p2->y &&  yAxis[1] <= p2->y - p2->h){
            if(p1->x <= p2->x + p2->w && p1 ->backwards == 1){
                p1->chara_state = knockback;
                p2->chara_state = knockback;
            }
        }
    }

}

//fonction pour les combos
void add_input_buffer(sprite_perso* player, enum combos_inputs touche_appui, int timestamp){
    if(player->pos_tab_combo == BUFFER_SIZE){
        for(int i = 0; i<BUFFER_SIZE; i++){
            player->buffer[i] = player->buffer[i+1];
        }
        player->pos_tab_combo = BUFFER_SIZE-1;
    }
    player->buffer[player->pos_tab_combo].input = touche_appui;
    player->buffer[player->pos_tab_combo++].timestamp = timestamp;

    for(int i = 0 ;i<player->pos_tab_combo;i++){
        printf("%d",player->buffer[i].input);
    }
}


/*FONCTIONS DE LIBERATION ESPACE*/
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

/*INPUTS GAMEPLAY*/

/**
 * MENU INPUTS
*/
void handle_menu_inputs(SDL_Event *event, jeu *world, SDL_Renderer* renderer){
    Uint8 *keystates;
    while(SDL_PollEvent(event)){
        if(event->type == SDL_QUIT){
            world->terminer = true;
        }

        if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.sym == SDLK_ESCAPE){
                if(world->state == pause){
                    world->state = combat;
                }
            }

            if(event->key.keysym.sym == SDLK_z){
                if(world->state != selection_map ){
                    world->menu_set.index_menu--;
                    if(world->menu_set.index_menu < 0){
                        world->menu_set.index_menu = 2;
                    }
                }
            }

            if(event->key.keysym.sym == SDLK_s ){
                if(world->state != selection_map){
                    world->menu_set.index_menu++;
                    if(world->menu_set.index_menu > 2){
                    world->menu_set.index_menu = 0;
                    }
                } 
            }

            if(event->key.keysym.sym == SDLK_d){
                if(world->state == selection_map){
                    world->menu_set.index_menu++;
                    if(world->menu_set.index_menu > NB_MAPS-1){
                        world->menu_set.index_menu = 0;
                    }
                } 
            }

            if(event->key.keysym.sym == SDLK_q){
                if(world->state == selection_map){
                    world->menu_set.index_menu--;
                    if(world->menu_set.index_menu < 0){
                    
                        world->menu_set.index_menu = NB_MAPS-1;
                    }
                } 
            }

            if(event->key.keysym.sym ==SDLK_RETURN ){
                if(world->state == main_menu){
                    switch(world->menu_set.index_menu){
                        case 0:
                            world->menu_set.index_menu = 0;
                            free(world->menu_set.menu_fond);
                            world->menu_set.menu_fond = load_image("build/ressources/menu/selection_map.png", renderer);
                            world->state = selection_map;
                            init_miniature(world,renderer);
                        break;
                        case 1:
                            world->state = options;
                        break;
                        case 2:
                            world->terminer = true;
                        break;
                    }
                }
                else if(world->state == selection_map){
                    switch(world->menu_set.index_menu){
                        case 0:
                            world->choosed_map = russia;
                        break;
                        case 1:
                            world->choosed_map = forest;
                        break;
                        case 2:
                            world->choosed_map = street_art;
                        break;
                    }
                    init_map(world, renderer);
                    init_perso(renderer,&world->p1,65, 465 ,125,232,CHARA_SPEED,false);
                    init_perso(renderer,&world->p2,1000, 465 ,125,232,CHARA_SPEED,true);
                    init_controller(world);
                    world->state = combat;
                    world->timestamp_w = 0;
                    
                    for(int i = 0; i<123;i++){
                        world->keystates_pre[i]=0;
                    }
                    init_combo(&world->p1);
                
                }
                }
                if(world->state!=combat){
                    for(int i = 0; i<123; i++){
                        world->keystates_pre[i]=keystates[i];
                        }
                }
            }
    }
}

/**
 * INPUTS GAMEPLAY COMBAT
*/
void gameplay_inputs(SDL_Event *event, jeu *world){
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_JoystickUpdate();
   // checkJoystick(world->joysticks);
    reset_hit(&world->p1);
    reset_hit(&world->p2);
    //printf("%d",world->p1.hits.punch->timer);
    while(SDL_PollEvent(event)){
        if(event->type == SDL_QUIT){
            world->terminer = true;
        }
    }   
        if(keystates[SDL_SCANCODE_ESCAPE] && !world->keystates_pre[SDL_SCANCODE_ESCAPE] ){
            
            world->state = pause;
        }
        
        if((!keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D] && !keystates[SDL_SCANCODE_S]||keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D]) && (world->p1.chara_state == walk || world->p1.chara_state == crouch)){
            world->p1.chara_state = idle;
        }
        if((!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT] && !keystates[SDL_SCANCODE_DOWN]|| keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT])  && (world->p2.chara_state == walk || world->p1.chara_state == crouch)){
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
            if(keystates[SDL_SCANCODE_A] != world->keystates_pre[SDL_SCANCODE_A]){
                add_input_buffer(&world->p1,left,world->timestamp_w);
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
            if(keystates[SDL_SCANCODE_LEFT] != world->keystates_pre[SDL_SCANCODE_LEFT]){
                add_input_buffer(&world->p2,left,world->timestamp_w);
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
            if(keystates[SDL_SCANCODE_RIGHT] != world->keystates_pre[SDL_SCANCODE_RIGHT]){
                add_input_buffer(&world->p2,right,world->timestamp_w);
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
            if(keystates[SDL_SCANCODE_D] != world->keystates_pre[SDL_SCANCODE_D]){
                add_input_buffer(&world->p1,right,world->timestamp_w);
            }
        }
        if(!keystates[SDL_SCANCODE_Z] && keystates[SDL_SCANCODE_S]){
            world->p1.chara_state = crouch;
            if(keystates[SDL_SCANCODE_S] != world->keystates_pre[SDL_SCANCODE_S]){
                add_input_buffer(&world->p1,down,world->timestamp_w);
            }
        }
      
        if(!keystates[SDL_SCANCODE_UP] && keystates[SDL_SCANCODE_DOWN]){
            world->p2.chara_state = crouch;
            if(keystates[SDL_SCANCODE_S] != world->keystates_pre[SDL_SCANCODE_S]){
                add_input_buffer(&world->p2,down,world->timestamp_w);
            }
        }
        //sauts
        if((keystates[SDL_SCANCODE_W] /*|| SDL_GameControllerGetAxis(world->joysticks[0],1)< (-10000)*/) && (world->p1.chara_state == idle || world->p1.chara_state == walk)){
            world->p1.chara_state = jump;
            if(keystates[SDL_SCANCODE_W] != world->keystates_pre[SDL_SCANCODE_W]){
                add_input_buffer(&world->p1,up,world->timestamp_w);
            }
        }
        if(keystates[SDL_SCANCODE_UP] && (world->p2.chara_state == idle || world->p2.chara_state == walk)){
            world->p2.chara_state = jump;
            if(keystates[SDL_SCANCODE_UP] != world->keystates_pre[SDL_SCANCODE_UP]){
                add_input_buffer(&world->p2,up,world->timestamp_w);
            }
        }
        //coups
        if(keystates[SDL_SCANCODE_G] && !keystates[SDL_SCANCODE_U] && !keystates[SDL_SCANCODE_Y]){
           
            if(keystates[SDL_SCANCODE_G] != world->keystates_pre[SDL_SCANCODE_G]){
                add_input_buffer(&world->p1,light_p,world->timestamp_w);
            }

            //wordld
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
            //light_punch(&world->p2, &world->p1);
        }
        //kicks
        if(!keystates[SDL_SCANCODE_KP_4] && keystates[SDL_SCANCODE_KP_8] && !keystates[SDL_SCANCODE_KP_9]){
            //special(world->p2, world->p1);
        }
        //coup spéciaux si la jauge est pleine
        if(!keystates[SDL_SCANCODE_KP_4] && !keystates[SDL_SCANCODE_KP_8] && keystates[SDL_SCANCODE_KP_9]){
           // kick(world->p2, world->p1);
        }
        movements(world, &world->p1, &world->p2);
        movements(world,&world->p2, &world->p1);
        sprites_collision(&world->p1, &world->p2, world);
        sprites_collision(&world->p2, &world->p1, world);
        change_directions(&world->p1, &world->p2);
        world->timestamp_w++;
        for(int i = 0; i<123; i++){
            world->keystates_pre[i]=keystates[i];
        }
        //collision_perso(&world->p1, &world->p2, pos_init_P1x);
        //collision_perso(&world->p2, &world->p1, pos_init_P2x);
        bool combop1 = false;
        bool combop2 = false;
        int i,j;
        for(i = 0 ; i< NB_COMBOS && !combop1 ; i++){
            combop1 = read_combo(&world->p1,i);
           
        }
        for(j = 0;j<NB_COMBOS && !combop2;j++){
             combop2 = read_combo(&world->p2,i);
        }
        //printf("p1 : %d, p2 : %d \n", combop1, combop2);
}