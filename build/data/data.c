#include "data.h"
#include "../constante.h"

void init_controller(jeu *world)
{
    int nbJoystick = SDL_NumJoysticks();
    printf("il y a %d controller\n", nbJoystick);
    if (nbJoystick > 0)
    {
        world->joysticks = malloc(sizeof(SDL_GameController *) * nbJoystick);
        for (int i = 0; i < nbJoystick; i++)
        {
            world->joysticks[i] = SDL_GameControllerOpen(i);
        }
    }
}
void init_timer(jeu *world)
{
    world->timer.startTime = SDL_GetTicks();
    world->timer.pause = false;
    world->timer.start = true;
    world->timer.timer = 60;
    world->p1.chrono_guard.startTime = SDL_GetTicks();
    world->p1.chrono_guard.pause = false;
    world->p1.chrono_guard.start = true;
    world->p2.chrono_guard.startTime = SDL_GetTicks();
    world->p2.chrono_guard.pause = false;
    world->p2.chrono_guard.start = true;
    world->p1.chrono_special.startTime = SDL_GetTicks();
    world->p1.chrono_special.pause = false;
    world->p1.chrono_special.start = true;
    world->p2.chrono_special.startTime = SDL_GetTicks();
    world->p2.chrono_special.pause = false;
    world->p2.chrono_special.start = true;
    

}
void init_map(jeu *world, SDL_Renderer *renderer)
{
    switch (world->choosed_map)
    {
    case russia:
        world->map.image_fond = load_image("build/map/city_night/city.bmp", renderer);
        world->map.map_structure = read_file_map("build/map/russia/russia_structure");
        world->map.plateformes = load_image("build/map/russia/plateforme.png", renderer);
        world->map.taille_cellule_H = sizeof(world->map.map_structure);
        world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
        break;
    case forest:
        world->map.image_fond = load_image("build/map/forest/forest.png", renderer);
        world->map.map_structure = read_file_map("build/map/forest/forest_structure");
        world->map.plateformes = load_image("build/map/forest/plateforme.png", renderer);
        world->map.taille_cellule_H = sizeof(world->map.map_structure);
        world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
        break;
    case street_art:
        world->map.image_fond = load_image("build/map/street_art/street.png", renderer);
        world->map.map_structure = read_file_map("build/map/street_art/street_structure");
        world->map.plateformes = load_image("build/map/street_art/plateforme.png", renderer);
        world->map.taille_cellule_H = sizeof(world->map.map_structure);
        world->map.taille_cellule_W = sizeof(world->map.map_structure[0]);
        break;
    }
}

// fonction pour les miniatures de maps
void init_miniature(jeu *world, SDL_Renderer *renderer)
{
    world->menu_set.tab_map[0] = load_image("build/map/russia/RussiaMap_mini.png", renderer);
    world->menu_set.tab_map[1] = load_image("build/map/forest/forest_mini.png", renderer);
    world->menu_set.tab_map[2] = load_image("build/map/street_art/street_mini.png", renderer);
}
void init_music(jeu* world){
    

    world->music.menu = Mix_LoadWAV("build/ressources/Audio/Game_theme.wav");
    world->music.light_punch = Mix_LoadWAV("build/ressources/Audio/missile_shot.wav");
    world->music.heavy_punch = Mix_LoadWAV("build/ressources/Audio/missile_shot.wav");
    world->music.kick = Mix_LoadWAV("build/ressources/Audio/missile_shot.wav");
    world->music.fireball = Mix_LoadWAV("build/ressources/Audio/missile_shot.wav");
    world->music.frappe = Mix_LoadWAV("build/ressources/Audio/missile_shot.wav");
    world->music.change = Mix_LoadWAV("build/ressources/Audio/missile_shot.wav");
    
}
void init_jeu(jeu *world, SDL_Renderer *renderer)
{
    world->menu_set.menu_fond = load_image("build/ressources/menu/menu.png", renderer);
    world->state = main_menu;
    world->terminer = false;
    world->menu_set.index_menu = 0;
    init_music(world);
}

void init_perso(SDL_Renderer *renderer, sprite_perso *perso, int x, int y, int w, int h, int speed, bool mirror)
{ // voir moyen pour charger texture spécifique
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
    perso->mirror = mirror;
    perso->life = 100;
    perso->attack_launched = false;
    perso->guard = false;
    perso->life_guard = 100;
    init_hits(perso);
    init_combos(perso);
    perso->pos_tab_combo = 0;
    perso->special_bar=0;
    perso->fireball.fireball = load_image("./build/ressources/Characters/Chara1/fireball.bmp",renderer);
    perso->fireball.launched_fireball = false;
    perso->buffer = malloc(sizeof(inputs) * BUFFER_SIZE);
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        perso->buffer[i].input = 0;
        perso->buffer[i].timestamp = 0;
        perso->pos_tab_combo = 0;
    }
    init_chara_state(renderer, perso);
}

void init_hits(sprite_perso *perso)
{
    perso->hits.light_punch = malloc(sizeof(hit));
    perso->hits.heavy_punch = malloc(sizeof(hit));
    perso->hits.kick = malloc(sizeof(hit));
    perso->hits.special_attack = malloc(sizeof(hit));

    perso->hits.light_punch->dmg = 10;
    perso->hits.light_punch->speed = 0;
    perso->hits.light_punch->range_x = 50;
    perso->hits.light_punch->range_y = 0;
    perso->hits.light_punch->effective_frame = 8;
    perso->hits.light_punch->timer = 0;
    perso->hits.light_punch->delay = 50;
    perso->hits.light_punch->launch = 0;

    perso->hits.heavy_punch->dmg = 40;
    perso->hits.heavy_punch->speed = 0;
    perso->hits.heavy_punch->range_x = 30;
    perso->hits.heavy_punch->range_y = 0;
    perso->hits.heavy_punch->effective_frame = 11;
    perso->hits.heavy_punch->launch = false;
    perso->hits.heavy_punch->timer = 0;
    perso->hits.heavy_punch->delay = 5;
    perso->hits.heavy_punch->launch = 0;

    perso->hits.kick->dmg = 1;
    perso->hits.kick->speed = 0;
    perso->hits.kick->range_x = 400;
    perso->hits.kick->range_y = 0;
    perso->hits.kick->effective_frame = 7;
    perso->hits.kick->timer = 0;
    perso->hits.kick->delay = 20;
    perso->hits.kick->launch = 0;
    perso->hits.special_attack->dmg = 1;
    perso->hits.special_attack->speed = 0;
    perso->hits.special_attack->range_x = 400;
    perso->hits.special_attack->range_y = 0;
    perso->hits.special_attack->effective_frame = 0;
    perso->hits.special_attack->timer = 0;
    perso->hits.special_attack->delay = 20;
}
void init_combo(int pos,int nb_coups,int frame_between,int required,sprite_perso *player){
    player->tab_combo[pos].nb_coups = nb_coups;
    player->tab_combo[pos].frame_between = frame_between;
    player->tab_combo[pos].required =required;
}
void init_combos(sprite_perso *player)
{
    // 1er combo
    init_combo(0,3,30,100,player);
    player->tab_combo[0].input[0] = right;
    player->tab_combo[0].input[1] = left;
    player->tab_combo[0].input[2] = light_p;
    init_combo(1,2,30,200,player);
    player->tab_combo[1].input[0] = left;
    player->tab_combo[1].input[1] = right;

    
    
}

void init_lootbox(lootbox * lootbox, SDL_Renderer * renderer){
    
    lootbox->texture[health_bonus] = load_image("build/ressources/Bonuses/Life_bonus.bmp", renderer);
    lootbox->texture[damage_bonus] = load_image("build/ressources/Bonuses/Damage_bonus.bmp", renderer);
    lootbox->texture[special_bonus] = load_image("build/ressources/Bonuses/Special_bonus.bmp", renderer);
    lootbox->x = generate_number(65, 950);
    lootbox->y = 0 - lootbox->h;
    lootbox->h = 28;
    lootbox->w = 18;
    lootbox->fallspeed = 3;
    lootbox->active = false;
    lootbox->collided = 0;
    lootbox->falling = false;
    lootbox->bonus = special_bonus;
}

void init(SDL_Window **window, SDL_Renderer **renderer, jeu *world)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    { // Initialisation de la SDL
        printf("Erreur d'initialisation de la SDL: %s", SDL_GetError());
        SDL_Quit();
    }

    *window = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    { // En cas d’erreur
        printf("Erreur de la creation d'une fenetre: %s", SDL_GetError());
        SDL_Quit();
    }
    // SDL_SetWindowFullscreen(*window,SDL_WINDOW_FULLSCREEN);

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("Erreur initialisation de SDL_Image: %s\n", IMG_GetError());
        SDL_Quit();
    }

    if (TTF_Init() == -1)
    {
        printf("erreur dans l'initialisation de TTF: %s", SDL_GetError());
        SDL_Quit();
    }

    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1 ){
        printf("erreur SDL_mix %S",Mix_GetError());
        SDL_Quit();
    }
    else{
         Mix_AllocateChannels(2); // Allouer 2 cannaux 
        Mix_Volume(0, MIX_MAX_VOLUME); // Mets le son a 100% en volume pour le premier cannaux
        Mix_Volume(1, 70); // Mets le son a 50% en volume pour le deuxièmme cannaux 
    }

    init_jeu(world, *renderer);
    
}

void init_state_animation(SDL_Renderer *renderer, sprite_perso *perso, enum character_state state, char *path, int nbFrame, int width)
{

    perso->anim[state].anim_text = load_image(path, renderer);
    perso->anim[state].frame = 0;
    perso->anim[state].nbFrame = nbFrame;
    perso->anim[state].width = width;
    perso->anim[state].counter = 0;
}

void init_chara_state(SDL_Renderer *renderer, sprite_perso *perso)
{
    switch (perso->perso_choisi)
    {
    case 0:
        init_state_animation(renderer, perso, idle, "build/ressources/Characters/Chara1/Idle.png", 15, 150);
        init_state_animation(renderer, perso, walk, "build/ressources/Characters/Chara1/Walking.png", 15, 150);
        init_state_animation(renderer, perso, jump, "build/ressources/Characters/Chara1/Jump.png", 10, 150);
        init_state_animation(renderer, perso, crouch, "build/ressources/Characters/Chara1/Idle.png", 15, 150);
        init_state_animation(renderer, perso, fall, "build/ressources/Characters/Chara1/Falling.png", 10, 150);
        init_state_animation(renderer, perso, backwards, "build/ressources/Characters/Chara1/Walking.png", 15, 150);
        init_state_animation(renderer, perso, flight, "build/ressources/Characters/Chara1/Jump.png", 10, 150);
        init_state_animation(renderer, perso, flight_control, "build/ressources/Characters/Chara1/Jump.png", 10, 150);
        init_state_animation(renderer, perso, fall_control, "build/ressources/Characters/Chara1/Falling.png", 10, 150);
        init_state_animation(renderer, perso, landing, "build/ressources/Characters/Chara1/Landing.png", 5, 150);
        init_state_animation(renderer, perso, knockback, "build/ressources/Characters/Chara1/KnockBack.png", 5, 150);
        init_state_animation(renderer, perso, lpunch, "build/ressources/Characters/Chara1/LightPunch.png", 16, 150);
        init_state_animation(renderer, perso, kickstate, "build/ressources/Characters/Chara1/Kick.png", 14, 150);
        init_state_animation(renderer, perso, hpunch, "build/ressources/Characters/Chara1/HeavyPunch.png", 15, 150);
        init_state_animation(renderer, perso, stun, "build/ressources/Characters/Chara1/Knocked.png", 5, 150);

        break;

    default:
        perso->anim[idle].anim_text = load_image("build/ressources/Characters/Chara1/Yellow_idle_spritesheet.png", renderer);
        break;
    }
}

/**
 * FONCTIONS DE CHARGEMENTS ET LECTURES
 */

char **read_file_map(char *nom)
{
    FILE *file = fopen(nom, "r");
    char **map_struct = malloc(sizeof(char *) * 40);
    for (int i = 0; i < 40; i++)
    {
        map_struct[i] = malloc(sizeof(char) * 20);
        fread(map_struct[i], 20, 1, file);
    }
    if (map_struct == NULL)
    {
        printf("Erreur de chargement de structure");
    }
    return map_struct;
}

SDL_Texture *load_image(char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = IMG_Load(path);
    if (NULL == tmp)
    {
        fprintf(stderr, "Erreur pendant chargement image BMP: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if (NULL == texture)
    {
        fprintf(stderr, "Erreur pendant creation de la texture liee a l'image chargee: %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

bool read_combo(sprite_perso *player, int val)
{

    // 1er combo
    bool found = false;
    bool equal;
    int j, k;
    int depart;
    for (int i = 0; i <= player->pos_tab_combo - player->tab_combo[val].nb_coups && !found; i++)
    {
        equal = true;
        depart = i;
        k = 0;
        for (j = i; j < i + player->tab_combo[val].nb_coups && equal; j++)
        {
            if (player->tab_combo[val].input[k] != player->buffer[j].input)
            {
                equal = false;
            }
            k++;
        }

        if (equal)
        {

            if (player->buffer[depart + player->tab_combo[val].nb_coups - 1].timestamp - player->buffer[depart].timestamp <= player->tab_combo[val].frame_between)
            {
                found = true;
                
            }
        }
    }
    if (found)
    {
        for (int i = 0; i < player->pos_tab_combo; i++)
            {
                player->buffer[i].input = 0;
                player->buffer[i].timestamp = 0;
            }
        player->pos_tab_combo = 0;
        if(player->special_bar>=player->tab_combo[val].required){
            player->special_bar -= player->tab_combo[val].required;
            switch (val)
            {
            case 0:
                player->chara_state = fireball;
                break;
            case 1:
                player->x =1000;
            default:
                break;
            }
            
        }
    }
    return found;
}

void checkJoystick(SDL_Joystick **joysticks)
{
    SDL_GameControllerUpdate(); // Mise à jour de l'état des controller
    int i;
    for (int j = 0; j < SDL_NumJoysticks(); j++)
    {
        for (i = 0; i < 20; i++)
        {
            if (SDL_GameControllerGetButton(joysticks[j], i))
            {
                printf("bouton %d presse", i);
            }
        }
    }
    printf("joystick X :%d \n", SDL_GameControllerGetAxis(joysticks[0], 0));
    printf("joystick Y : %d \n", SDL_GameControllerGetAxis(joysticks[0], 1));
}

void check_timer(jeu *world)
{

    if (world->timer.start && !world->timer.pause)
    {

        if ((SDL_GetTicks() - world->timer.startTime) / 1000 >= 1)
        {
            world->timer.startTime = SDL_GetTicks();
            world->timer.timer--;
        }
    }
}

/**
 * FONCTIONS MOUVEMENTS + COUPS
 */
void reset_state(sprite_perso *perso, enum character_state state)
{
    for (int i = 0; i < 2; i++)
    {
        if (i != state)
        {
            perso->anim[i].frame = 0;
        }
    }
}

void movements(jeu *world, sprite_perso *perso, sprite_perso *adversaire)
{
    int x, y;
    x = perso->x;
    y = perso->y;

    reset_state(perso, perso->chara_state);
    if (perso->chara_state == idle)
    {

        if (perso->anim[idle].counter < 3)
        {
            perso->anim[idle].counter++;
        }
        else
        {
            perso->anim[idle].counter = 0;
            perso->anim[idle].frame++;
        }

        if (perso->anim[idle].frame == perso->anim[idle].nbFrame)
        {
            perso->anim[idle].frame = 0;
        }
    }

    // Contrôle en marche
    if (perso->chara_state == walk)
    {

        if (canMove(perso, adversaire))
        {
            if (perso->backwards)
            {
                if (!equals(x - perso->speed, y + perso->h, world->map.map_structure, '0'))
                {
                    perso->x -= perso->speed;
                }
            }
            else
            {
                if (!equals(x + perso->speed, y + perso->h, world->map.map_structure, '0'))
                {
                    perso->x += perso->speed;
                }
            }
            if (perso->anim[walk].counter < 3)
            {
                perso->anim[walk].counter++;
            }
            else
            {
                perso->anim[walk].counter = 0;
                perso->anim[walk].frame++;
            }

            if (perso->anim[walk].frame == perso->anim[walk].nbFrame)
            {
                perso->anim[walk].frame = 0;
            }
        }
    }

    // Contrôle en vol
    if (perso->chara_state == flight_control)
    {
        if (perso->backwards)
        {
            if (equals(x - perso->speed, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x -= perso->speed * 0.2;
            }
        }
        else
        {
            if (equals(x + perso->speed, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x += perso->speed * 0.2;
            }
        }
        perso->chara_state = flight;
    }

    // Contrôle en chute
    if (perso->chara_state == fall_control)
    {
        if (perso->backwards)
        {
            if (equals(x - perso->speed, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x -= perso->speed * 0.4;
            }
        }
        else
        {
            if (equals(x + perso->speed, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x += perso->speed * 0.4;
            }
        }
        perso->chara_state = fall;
    }

    // Initialisation du saut
    if (perso->chara_state == jump)
    {

        if (perso->anim[jump].frame = perso->anim[jump].nbFrame)
        {
            perso->jump_origin = perso->y;
            perso->chara_state = flight;
        }
        else
        {
            perso->anim[jump].frame++;
        }
    }

    // Contrôle de la hauteur du saut
    if (perso->chara_state == flight)
    {
        if (perso->y > perso->jump_origin - perso->jump_height)
        {
            if (!equals(x, y, world->map.map_structure, '0'))
            {
                perso->y -= perso->jump_height * 0.025;
            }
            else
            {
                perso->chara_state = fall;
            }
        }
        else
        {
            perso->chara_state = fall;
        }
    }

    // Automatisation de la chute
    if ((perso->chara_state == fall))
    {
        x = perso->x;
        y = perso->y;
        if (y > 465)
        { // replace correctement perso sur le sol
            perso->y = 465;
            perso->chara_state = landing;
        }
        else
        {
            if (equals(x, y + perso->h + perso->jump_height * 0.025, world->map.map_structure, ' '))
            {
                perso->y += perso->jump_height * 0.025;
            }
            else
            {
                perso->chara_state = landing;
            }
        }
    }

    // Cas où le personnage chute s'il n'y a pas de plateformes
    x = perso->x;
    y = perso->y;

    if (perso->chara_state != flight && equals(x, y + perso->h + perso->jump_height * 0.025, world->map.map_structure, ' '))
    {
        perso->chara_state = fall;
    }

    if (perso->chara_state == landing)
    {
        if (perso->anim[landing].counter == 3)
        {
            perso->anim[landing].frame++;

            if (perso->anim[landing].frame >= perso->anim[landing].nbFrame)
            {
                perso->chara_state = idle;
                perso->anim[landing].frame = 0;
            }
            perso->anim[landing].counter = 0;
        }
        else
        {
            perso->anim[landing].counter++;
        }
    }

    if (perso->chara_state == knockback)
    {
        if (!perso->mirror)
        {
            if (!equals(x - perso->speed, y + perso->h, world->map.map_structure, '0'))
            {
                perso->x -= perso->speed;
            }
        }
        else
        {
            if (!equals(x + perso->speed, y + perso->h, world->map.map_structure, '0'))
            {
                perso->x += perso->speed;
            }
        }
        if (perso->anim[knockback].counter == 2)
        {
            perso->anim[knockback].frame++;
            if (perso->anim[knockback].frame >= perso->anim[knockback].nbFrame)
            {
                perso->chara_state = idle;
                perso->anim[knockback].frame = 0;
            }
            perso->anim[knockback].counter = 0;
        }
        else
        {
            perso->anim[knockback].counter++;
        }
    }
    if (perso->chara_state == lpunch)
    {
        if (perso->anim[lpunch].counter == 2)
        {
            perso->anim[lpunch].frame++;
            if (perso->anim[lpunch].frame == perso->hits.light_punch->effective_frame)
            {

                light_punch(perso, adversaire);
            }
            if (perso->anim[lpunch].frame >= perso->anim[lpunch].nbFrame)
            {
                perso->chara_state = idle;
                perso->anim[lpunch].frame = 0;
                perso->attack_launched = false;
            }
            perso->anim[lpunch].counter = 0;
        }
        else
        {
            perso->anim[lpunch].counter++;
        }
    }
    if (perso->chara_state == hpunch)
    {
        if (perso->anim[hpunch].counter == 3)
        {
            perso->anim[hpunch].frame++;
            if (perso->anim[hpunch].frame == perso->hits.heavy_punch->effective_frame)
            {
                heavy_punch(perso, adversaire);
            }
            if (perso->anim[hpunch].frame >= perso->anim[hpunch].nbFrame)
            {
                perso->chara_state = idle;
                perso->anim[hpunch].frame = 0;
                perso->attack_launched = false;
            }
            perso->anim[hpunch].counter = 0;
        }
        else
        {
            perso->anim[hpunch].counter++;
        }
    }
    if (perso->chara_state == kickstate)
    {
        if (perso->anim[kickstate].counter == 2)
        {
            perso->anim[kickstate].frame++;
            if (perso->anim[kickstate].frame == perso->hits.kick->effective_frame)
            {
                kick_hit(perso, adversaire);
            }
            if (perso->anim[kickstate].frame >= perso->anim[kickstate].nbFrame)
            {
                perso->chara_state = idle;
                perso->anim[kickstate].frame = 0;
                perso->attack_launched = false;
            }
            perso->anim[kickstate].counter = 0;
        }
        else
        {
            perso->anim[kickstate].counter++;
        }
    }
    /*stun */
    if (perso->chara_state == stun)
    {
        if (perso->stun_time >= 1)
        {

            perso->stun_time--;

            if (perso->anim[stun].counter == 10)
            {
                perso->anim[stun].frame++;

                if (perso->anim[stun].frame = perso->anim[stun].nbFrame)
                {
                    perso->anim[stun].frame = 0;
                }
            }
            else
            {
                perso->anim[stun].counter++;
            }
        }
        else
        {
            if (perso->broken_guard)
            {
                perso->broken_guard = false;
                perso->life_guard = 100;
            }
            perso->attack_launched = 0;
            perso->chara_state = idle;
        }
    }
    if(perso->chara_state == fireball){
        perso->fireball.launched_fireball = true;
        perso->chara_state = idle;

    }
}

bool equals(int x, int y, char **map_point, char test)
{
    int height_factor, width_factor;
    height_factor = CELL_HEIGHT;
    width_factor = CELL_WIDTH;
    return (map_point[y / height_factor][x / width_factor] == test);
}

void light_punch(sprite_perso *attacker, sprite_perso *receiver)
{

    if (!attacker->mirror)
    {
        if ((attacker->x + attacker->w + attacker->hits.light_punch->range_x >= receiver->x) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            if (receiver->guard)
            {
                printf("guard life : %d", receiver->life_guard);
                receiver->life_guard -= attacker->hits.light_punch->dmg;
                if (receiver->life_guard <= 0)
                {
                    receiver->broken_guard = true;
                    receiver->chara_state = stun;
                    receiver->stun_time = 400;
                }
            }
            else
            {
                receiver->special_bar +=10;
                receiver->life -= attacker->hits.light_punch->dmg;
                if (!receiver->broken_guard)
                {
                    receiver->chara_state = stun;
                    receiver->stun_time = attacker->hits.light_punch->delay;
                }
            }
        }
    }
    else
    {
        if ((attacker->x - attacker->hits.light_punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            if (receiver->guard)
            {
                receiver->life_guard -= attacker->hits.light_punch->dmg;
                printf("guard life : %d", receiver->life_guard);
                if (receiver->life_guard <= 0)
                {
                    receiver->broken_guard = true;
                    receiver->chara_state = stun;
                    receiver->stun_time = 400;
                    printf("brisé");
                }
            }
            else
            {   receiver->special_bar +=10;
                receiver->life -= attacker->hits.light_punch->dmg;
                if (!receiver->broken_guard)
                {
                    receiver->chara_state = stun;
                    receiver->stun_time = attacker->hits.light_punch->delay;
                }
            }
        }
    }
}

void heavy_punch(sprite_perso *attacker, sprite_perso *receiver)
{

    if (!attacker->mirror)
    {
        if ((attacker->x + attacker->w + attacker->hits.heavy_punch->range_x >= receiver->x) && (attacker->y + attacker->h / 2 >= receiver->y && attacker->y + attacker->h / 2 <= receiver->y + receiver->h))
        {
            if (receiver->guard)
            {
                printf("guard life : %d", receiver->life_guard);
                receiver->life_guard -= attacker->hits.heavy_punch->dmg;
                if (receiver->life_guard <= 0)
                {
                    receiver->broken_guard = true;
                    receiver->chara_state = stun;
                    receiver->stun_time = 400;
                }
            }
            else
            {
                receiver->special_bar +=20;
                printf("%d",receiver->special_bar);
                receiver->life -= attacker->hits.heavy_punch->dmg;
                if (!receiver->broken_guard)
                {
                    receiver->chara_state = stun;
                    receiver->stun_time = attacker->hits.heavy_punch->delay;
                }
            }
        }
    }
    else
    {
        if ((attacker->x - attacker->hits.heavy_punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            if (receiver->guard)
            {
                receiver->life_guard -= attacker->hits.heavy_punch->dmg;
                printf("guard life : %d", receiver->life_guard);
                if (receiver->life_guard <= 0)
                {
                    receiver->broken_guard = true;
                    receiver->chara_state = stun;
                    receiver->stun_time = 400;
                    printf("brisé");
                }
            }
            else
            {   
                receiver->special_bar +=20;
                receiver->life -= attacker->hits.heavy_punch->dmg;
                if (!receiver->broken_guard)
                {
                    receiver->chara_state = stun;
                    receiver->stun_time = attacker->hits.heavy_punch->delay;
                }
            }
        }
    }
}

void kick_hit(sprite_perso *attacker, sprite_perso *receiver)
{

    if (!attacker->mirror)
    {
        if ((attacker->x + attacker->w + attacker->hits.kick->range_x >= receiver->x) && (attacker->y + attacker->h / 2 >= receiver->y && attacker->y + attacker->h / 2 <= receiver->y + receiver->h))
        {
            if (receiver->guard)
            {
                printf("guard life : %d", receiver->life_guard);
                receiver->life_guard -= attacker->hits.kick->dmg;
                if (receiver->life_guard <= 0)
                {
                    receiver->broken_guard = true;
                    receiver->chara_state = stun;
                    receiver->stun_time = 400;
                }
            }
            else
            {
                receiver->special_bar +=20;
                receiver->life -= attacker->hits.kick->dmg;
                if (!receiver->broken_guard)
                {
                    receiver->chara_state = stun;
                    receiver->stun_time = attacker->hits.kick->delay;
                }
            }
        }
    }
    else
    {
        if ((attacker->x - attacker->hits.heavy_punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            if (receiver->guard)
            {
                receiver->life_guard -= attacker->hits.kick->dmg;
                printf("guard life : %d", receiver->life_guard);
                if (receiver->life_guard <= 0)
                {
                    receiver->broken_guard = true;
                    receiver->chara_state = stun;
                    receiver->stun_time = 400;
                    printf("brisé");
                }
            }
            else
            {
                receiver->special_bar +=20;
                receiver->life -= attacker->hits.kick->dmg;
                if (!receiver->broken_guard)
                {
                    receiver->chara_state = stun;
                    receiver->stun_time = attacker->hits.kick->delay;
                }
            }
        }
    }
}

void change_directions(sprite_perso *p1, sprite_perso *p2)
{
    if (p1->x > p2->x)
    {
        p1->mirror = true;
        p2->mirror = false;
    }
    else
    {
        p1->mirror = false;
        p2->mirror = true;
    }
}

bool canMove(sprite_perso *perso, sprite_perso *adversaire)
{
    bool canMove = true;
    int *yAxis = (int *)malloc(sizeof(int) * 2);
    yAxis[0] = perso->y;
    yAxis[1] = perso->y - perso->h;

    if (!perso->mirror && adversaire->mirror)
    {
        if (yAxis[0] >= adversaire->y && yAxis[1] <= adversaire->y - adversaire->h)
        {
            if (perso->x + perso->speed >= adversaire->x - adversaire->w && perso->backwards != 1)
            {
                canMove = false;
            }
        }
    }
    else
    {
        if (yAxis[0] >= adversaire->y && yAxis[1] <= adversaire->y - adversaire->h)
        {
            if (perso->x - perso->speed <= adversaire->x + adversaire->w && perso->backwards == 1)
            {
                canMove = false;
            }
        }
    }
    return canMove;
}

void sprites_collision(sprite_perso *p1, sprite_perso *p2, jeu *world)
{ // Cas où les deux sprites sont superposés (suite à par ex un saut), un choc se produit et ils reculent tout les deux si possibles
    int *yAxis = (int *)malloc(sizeof(int) * 2);
    yAxis[0] = p1->y;
    yAxis[1] = p1->y - p1->h;
    if (!p1->mirror && p2->mirror)
    {
        if (yAxis[0] >= p2->y && yAxis[1] <= p2->y - p2->h)
        {
            if (p1->x + p1->w >= p2->x && p1->backwards != 1)
            {
                p1->chara_state = knockback;
                p2->chara_state = knockback;
            }
        }
    }
    else
    {
        if (yAxis[0] >= p2->y && yAxis[1] <= p2->y - p2->h)
        {
            if (p1->x <= p2->x + p2->w && p1->backwards == 1)
            {
                p1->chara_state = knockback;
                p2->chara_state = knockback;
            }
        }
    }
}

// fonction d'ajout de l'input dans le buffer
void add_input_buffer(sprite_perso *player, enum combos_inputs touche_appui, int timestamp)
{
    if (player->pos_tab_combo == BUFFER_SIZE)
    {
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            player->buffer[i] = player->buffer[i + 1];
        }
        player->pos_tab_combo = BUFFER_SIZE - 1;
    }
    player->buffer[player->pos_tab_combo].input = touche_appui;
    player->buffer[player->pos_tab_combo++].timestamp = timestamp;
}
void free_hits(sprite_perso* perso){
    
    free(perso->buffer);
    free(perso->hits.light_punch);
    free(perso->hits.heavy_punch);
    free(perso->hits.kick);
}
/*FONCTIONS DE LIBERATION ESPACE*/
void quit_game(jeu *world, SDL_Window **fenetre, SDL_Renderer **renderer)
{
    SDL_DestroyWindow(*fenetre);
    SDL_DestroyRenderer(*renderer);
    free_hits(&world->p1);
    free_hits(&world->p2);
    destroy_textures(world);
    free_map_structure(world->map.map_structure);
    TTF_CloseFont(world->font.police_compteur);
    close_Joystick(world->joysticks);
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

void close_Joystick(jeu *world)
{
    int nbJoystick = SDL_NumJoysticks();
    if (nbJoystick > 0)
    {
        for (int i = 0; i < nbJoystick; i++)
        {
            SDL_GameControllerClose(world->joysticks[i]);
        }
        free(world->joysticks);
    }
}

void free_map_structure(char **map_structure)
{
    for (int i = 0; i < 40; i++)
    {
        free(map_structure[i]);
    }
    free(map_structure);
}
void destroy_anim(sprite_perso* perso){
    for(int i = 0;i<=stun;i++){
        SDL_DestroyTexture(perso->anim[i].anim_text);
    }
    SDL_DestroyTexture(perso->fireball.fireball);
}
void destroy_map(jeu* world){
    SDL_DestroyTexture(world->menu_set.tab_map[0]);
    SDL_DestroyTexture(world->menu_set.tab_map[1]);
    SDL_DestroyTexture(world->menu_set.tab_map[2]);
}
void destroy_textures(jeu *world)
{
   SDL_DestroyTexture(world->menu_set.menu_fond);
   destroy_anim(&world->p1);
   destroy_anim(&world->p2);
    SDL_DestroyTexture(world->map.image_fond);
    SDL_DestroyTexture(world->map.plateformes);
    destroy_map(world);

}

void unpause(compteur* chrono){
    chrono->startTime = SDL_GetTicks() - chrono->pauseTime;
    chrono->pause = false;
}
/*INPUTS GAMEPLAY*/

/**
 * MENU INPUTS
 */
void handle_menu_inputs(SDL_Event *event, jeu *world, SDL_Renderer *renderer)
{
    Uint8 *keystates;
    Mix_PlayChannel(1,world->music.menu,-1);

    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            world->terminer = true;
        }

        if (event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                if (world->state == pause)
                {
                    world->state = combat;
                    Mix_Volume(1,0);
                   unpause(&world->timer);
                    unpause(&world->p1.chrono_guard);
                    unpause(&world->p2.chrono_guard);
                    unpause(&world->p1.chrono_special);
                    unpause(&world->p2.chrono_special);
                }
            }

            if (event->key.keysym.sym == SDLK_z)
            {   
                if (world->state != selection_map)
                {   Mix_PlayChannel(0,world->music.change,1);
                    world->menu_set.index_menu--;
                    if (world->menu_set.index_menu < 0)
                    {
                        world->menu_set.index_menu = 2;
                    }
                }
            }

            if (event->key.keysym.sym == SDLK_s)
            {    
                if (world->state != selection_map)
                {Mix_PlayChannel(0,world->music.change,1);
                    world->menu_set.index_menu++;
                    if (world->menu_set.index_menu > 2)
                    {
                        world->menu_set.index_menu = 0;
                    }
                }
            }

            if (event->key.keysym.sym == SDLK_d)
            {
                if (world->state == selection_map)
                {    Mix_PlayChannel(0,world->music.change,1);
                    world->menu_set.index_menu++;
                    if (world->menu_set.index_menu > NB_MAPS - 1)
                    {
                        world->menu_set.index_menu = 0;
                    }
                }
            }

            if (event->key.keysym.sym == SDLK_q)
            {
                if (world->state == selection_map)
                {   Mix_PlayChannel(0,world->music.change,1);
                    world->menu_set.index_menu--;
                    if (world->menu_set.index_menu < 0)
                    {

                        world->menu_set.index_menu = NB_MAPS - 1;
                    }
                }
            }

            if (event->key.keysym.sym == SDLK_RETURN)
            {
                if (world->state == main_menu)
                {
                    switch (world->menu_set.index_menu)
                    {
                    case 0:
                        world->menu_set.index_menu = 0;
                        free(world->menu_set.menu_fond);
                        world->menu_set.menu_fond = load_image("build/ressources/menu/selection_map.png", renderer);
                        world->state = selection_map;
                        init_miniature(world, renderer);
                        break;
                    case 1:
                        world->state = options;
                        break;
                    case 2:
                        world->terminer = true;
                        break;
                    }
                }
                else if (world->state == selection_map)
                {
                    switch (world->menu_set.index_menu)
                    {
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
                    init_perso(renderer, &world->p1, 65, 465, 100, 230, CHARA_SPEED, false);
                    init_perso(renderer, &world->p2, 950, 465, 100, 230, CHARA_SPEED, true);
                    init_controller(world);
                    world->state = combat;
                    world->timestamp_w = 0;
                    world->font.police_compteur = TTF_OpenFont("build/ressources/Polices/Reach_Story.ttf", 40);
                    init_timer(world);
                    if (world->font.police_compteur == NULL)
                    {
                        printf("chargement de la police incomplete");
                    }
                    for (int i = 0; i < 123; i++)
                    {
                        world->keystates_pre[i] = 0;
                    }
                    
                    init_lootbox(&world->lootbox,renderer);
                    // check_game(world); //???
                }
            }
            if (world->state != combat)
            {
                for (int i = 0; i < 123; i++)
                {
                    world->keystates_pre[i] = keystates[i];
                }
            }
        }
    }
}
void pauseChrono(compteur* chrono){
     chrono->pause = true;
     chrono->pauseTime = SDL_GetTicks() - chrono->startTime;
}
/**
 * INPUTS GAMEPLAY COMBAT
 */
void gameplay_inputs(SDL_Event *event, jeu *world)
{
    
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_JoystickUpdate();
    // checkJoystick(world->joysticks);
     bool combop1 = false;
     bool combop2 = false;

    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            world->terminer = true;
        }
    }
    if (keystates[SDL_SCANCODE_ESCAPE] && !world->keystates_pre[SDL_SCANCODE_ESCAPE])
    {
        world->state = pause;
        pauseChrono(&world->timer);
        pauseChrono(&world->p1.chrono_guard);
        pauseChrono(&world->p2.chrono_guard);
    }

    if (world->p1.chara_state != stun && !world->p1.attack_launched && !world->p1.guard)
    {

        // aucune touche de déplacement appuyé
        if ((!keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D] && !keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D]) && (world->p1.chara_state == walk || world->p1.chara_state == crouch))
        {
            world->p1.chara_state = idle;
        }
        // deplacement gauche
        if ((keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D]) /*|| SDL_GameControllerGetAxis(world->joysticks[0],0)<-(10000)*/)
        {
            // world->p1.speed = -CHARA_SPEED;

            if (world->p1.chara_state == idle)
            {
                world->p1.chara_state = walk;
                world->p1.backwards = true;
            }
            if (world->p1.chara_state == flight)
            {
                world->p1.chara_state = flight_control;
                world->p1.backwards = true;
            }
            if (world->p1.chara_state == fall)
            {
                world->p1.chara_state = fall_control;
                world->p1.backwards = true;
            }
            if (keystates[SDL_SCANCODE_A] != world->keystates_pre[SDL_SCANCODE_A])
            {
                add_input_buffer(&world->p1, left, world->timestamp_w);
            }
        }
        // deplacement droite
        if (!keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D] /*|| SDL_GameControllerGetAxis(world->joysticks[0],0)>10000*/)
        {
            // world->p1.speed = CHARA_SPEED;

            if (world->p1.chara_state == idle)
            {
                world->p1.chara_state = walk;
                world->p1.backwards = false;
            }
            if (world->p1.chara_state == flight)
            {
                world->p1.chara_state = flight_control;
                world->p1.backwards = false;
            }
            if (world->p1.chara_state == fall)
            {
                world->p1.chara_state = fall_control;
                world->p1.backwards = false;
            }
            if (keystates[SDL_SCANCODE_D] != world->keystates_pre[SDL_SCANCODE_D])
            {
                add_input_buffer(&world->p1, right, world->timestamp_w);
            }
        }
        if (!keystates[SDL_SCANCODE_Z] && keystates[SDL_SCANCODE_S])
        {
            if (world->p1.chara_state == idle || world->p1.chara_state == walk)
            {
                world->p1.chara_state = crouch;
                if (keystates[SDL_SCANCODE_S] != world->keystates_pre[SDL_SCANCODE_S])
                {
                    add_input_buffer(&world->p1, down, world->timestamp_w);
                }
            }
        }

        // sauts
        if ((keystates[SDL_SCANCODE_W] /*|| SDL_GameControllerGetAxis(world->joysticks[0],1)< (-10000)*/) && (world->p1.chara_state == idle || world->p1.chara_state == walk))
        {
            world->p1.chara_state = jump;
            if (keystates[SDL_SCANCODE_W] != world->keystates_pre[SDL_SCANCODE_W])
            {
                add_input_buffer(&world->p1, up, world->timestamp_w);
            }
        }

        // coups de poings leger
        if (keystates[SDL_SCANCODE_G] && !keystates[SDL_SCANCODE_Y] && !keystates[SDL_SCANCODE_J])
        {
            if (keystates[SDL_SCANCODE_G] != world->keystates_pre[SDL_SCANCODE_G])
            {

                if (world->p1.chara_state == idle || world->p1.chara_state == walk)
                {   
                    add_input_buffer(&world->p1, light_p, world->timestamp_w);
                    for(int i = 0; i<NB_COMBOS && !combop1;i++){
                    combop1 = read_combo(&world->p1, i);
                    }
                    if(!combop1){
                    world->p1.chara_state = lpunch;
                    world->p1.attack_launched = true;
                    }

                    
                }
            }
        }

        // coup spéciaux si la jauge est pleine
        if (!keystates[SDL_SCANCODE_G] && keystates[SDL_SCANCODE_Y] && !keystates[SDL_SCANCODE_J])
        {
            if (keystates[SDL_SCANCODE_Y] != world->keystates_pre[SDL_SCANCODE_Y])
            {

                if (world->p1.chara_state == idle || world->p1.chara_state == walk)
                {
                    add_input_buffer(&world->p1, heavy_p, world->timestamp_w);
                    for(int i = 0; i<NB_COMBOS && !combop1;i++){
                    combop1 = read_combo(&world->p1, i);
                    }
                    if(!combop1){
                    world->p1.chara_state = hpunch;
                    world->p1.attack_launched = true;
                    }

                }
            }
        }
        if (!keystates[SDL_SCANCODE_Y] && !keystates[SDL_SCANCODE_G] && keystates[SDL_SCANCODE_J])
        {
            if (keystates[SDL_SCANCODE_J] != world->keystates_pre[SDL_SCANCODE_J])
            {

                if (world->p1.chara_state == idle || world->p1.chara_state == walk)
                {
                  add_input_buffer(&world->p1, kick, world->timestamp_w);
                    for(int i = 0; i<NB_COMBOS && !combop1;i++){
                    combop1 = read_combo(&world->p1, i);
                    }
                    if(!combop1){
                    world->p1.chara_state = kickstate;
                    world->p1.attack_launched = true;
                    }

                }
            }
        }
        if (keystates[SDL_SCANCODE_H] && (world->p1.chara_state == idle || world->p1.chara_state == walk) && !world->p1.broken_guard)
        {
            world->p1.guard = true;
            world->p1.chara_state = idle;
        }
    }
    if (!keystates[SDL_SCANCODE_H] && (world->p1.chara_state == idle || world->p1.chara_state == walk))
    {
        world->p1.guard = false;
    }
    if (world->p2.chara_state != stun && !world->p2.attack_launched && !world->p2.guard)
    {
        if ((!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT] && !keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]) && (world->p2.chara_state == walk || world->p2.chara_state == crouch))
        {
            world->p2.chara_state = idle;
        }
        if (keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT])
        {
            // world->p1.speed = -CHARA_SPEED;

            if (world->p2.chara_state == idle)
            {
                world->p2.chara_state = walk;
                world->p2.backwards = true;
            }
            if (world->p2.chara_state == flight)
            {
                world->p2.chara_state = flight_control;
                world->p2.backwards = true;
            }
            if (world->p2.chara_state == fall)
            {
                world->p2.chara_state = fall_control;
                world->p2.backwards = true;
            }
            if (keystates[SDL_SCANCODE_LEFT] != world->keystates_pre[SDL_SCANCODE_LEFT])
            {
                add_input_buffer(&world->p2, left, world->timestamp_w);
            }
        }

        if (!keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT])
        {
            // world->p1.speed = CHARA_SPEED;

            if (world->p2.chara_state == idle)
            {
                world->p2.chara_state = walk;
                world->p2.backwards = false;
            }
            if (world->p2.chara_state == flight)
            {
                world->p2.chara_state = flight_control;
                world->p2.backwards = false;
            }
            if (world->p2.chara_state == fall)
            {
                world->p2.chara_state = fall_control;
                world->p2.backwards = false;
            }
            if (keystates[SDL_SCANCODE_RIGHT] != world->keystates_pre[SDL_SCANCODE_RIGHT])
            {
                add_input_buffer(&world->p2, right, world->timestamp_w);
            }
        }

        if (!keystates[SDL_SCANCODE_UP] && keystates[SDL_SCANCODE_DOWN])
        {
            if (world->p2.chara_state == idle || world->p2.chara_state == walk)
            {
                world->p2.chara_state = crouch;
                if (keystates[SDL_SCANCODE_DOWN] != world->keystates_pre[SDL_SCANCODE_DOWN])
                {
                    add_input_buffer(&world->p2, down, world->timestamp_w);
                }
            }
        }

        if (keystates[SDL_SCANCODE_UP] && (world->p2.chara_state == idle || world->p2.chara_state == walk))
        {
            world->p2.chara_state = jump;
            if (keystates[SDL_SCANCODE_UP] != world->keystates_pre[SDL_SCANCODE_UP])
            {
                add_input_buffer(&world->p2, up, world->timestamp_w);
            }
        }

        // coups
        if (keystates[SDL_SCANCODE_KP_4] && !keystates[SDL_SCANCODE_KP_8] && !keystates[SDL_SCANCODE_KP_9])
        {
            if (keystates[SDL_SCANCODE_KP_4] != world->keystates_pre[SDL_SCANCODE_KP_4])
            {

                if (world->p2.chara_state == idle || world->p2.chara_state == walk)
                {
                     add_input_buffer(&world->p2, light_p, world->timestamp_w);
                    for(int i = 0; i<NB_COMBOS && !combop2;i++){
                        combop2 = read_combo(&world->p2, i);
                    }
                    if(!combop2){
                    world->p2.chara_state = lpunch;
                    world->p2.attack_launched = true;
                    }
                }
            }
        }
        // speciaux
        if (!keystates[SDL_SCANCODE_KP_4] && keystates[SDL_SCANCODE_KP_8] && !keystates[SDL_SCANCODE_KP_9])
        {
            if (keystates[SDL_SCANCODE_KP_8] != world->keystates_pre[SDL_SCANCODE_KP_8])
            {

                if (world->p2.chara_state == idle || world->p2.chara_state == walk)
                {
                    add_input_buffer(&world->p2, heavy_p, world->timestamp_w);
                    for(int i = 0; i<NB_COMBOS && !combop2;i++){
                    combop2 = read_combo(&world->p2, i);
                    }
                    if(!combop2){
                    world->p2.chara_state = hpunch;
                    world->p2.attack_launched = true;
                    }
                }
            }
        }
        // coup spéciaux si la jauge est pleine
        if (!keystates[SDL_SCANCODE_KP_4] && !keystates[SDL_SCANCODE_KP_8] && keystates[SDL_SCANCODE_KP_9])
        {
            if (keystates[SDL_SCANCODE_KP_9] != world->keystates_pre[SDL_SCANCODE_KP_9])
            {

                if (world->p2.chara_state == idle || world->p2.chara_state == walk)
                {
                     add_input_buffer(&world->p2, kick, world->timestamp_w);
                    for(int i = 0; i<NB_COMBOS && !combop2;i++){
                    combop2 = read_combo(&world->p2, i);
                    }
                    if(!combop2){
                    world->p2.chara_state = kickstate;
                    world->p2.attack_launched = true;
                    }
                }
            }
        }
        if (keystates[SDL_SCANCODE_KP_5] && !world->p2.broken_guard && (world->p2.chara_state == idle || world->p2.chara_state == walk))
        {
            world->p2.guard = true;
            world->p2.chara_state = idle;
        }
    }
    if (!keystates[SDL_SCANCODE_KP_5] && (world->p2.chara_state == idle || world->p2.chara_state == walk))
    {
        world->p2.guard = false;
    }
    for (int i = 0; i < 123; i++)
    {
        world->keystates_pre[i] = keystates[i];
    }
}

/**
 * GESTION DES LOOTBOX
 */

int generate_number(int a, int b){
     srand ( time (NULL) ) ;
    return rand() % (b - a) + a;
}

void reset_activate_lootbox(lootbox * lootbox){
    lootbox->active = true;
    lootbox->falling = true;
    lootbox->x = generate_number(65, 950);
    lootbox->y = 0 - lootbox->h;
    lootbox->collided = 0;
    lootbox->bonus = generate_number(health_bonus,special_bonus+1);
}
void update_lootbox(jeu * world){
    if (world->lootbox.falling)
    {
        if ((equals(world->lootbox.x + world->lootbox.w / 2, world->lootbox.y + world->lootbox.h + world->lootbox.fallspeed, world->map.map_structure, ' ') || world->lootbox.y < CELL_HEIGHT + 20) && world->lootbox.y < 701 ){
            world->lootbox.y += world->lootbox.fallspeed;
        }
        else{
            world->lootbox.falling = false;
        }
    }
}

void check_lootbox_pickup(sprite_perso *player, lootbox *lootbox, int player_number){
    int collision_x = lootbox->x - player->x;
    int collision_y = lootbox->y - player->y - player->h;
    int distance = sqrt(collision_x * collision_x + collision_y * collision_y);
    if (distance < player->w && lootbox->active){ 
        if (lootbox->collided != 0){
            lootbox->collided = 3;
        }
        else{
            lootbox->collided = player_number;
        }
        lootbox->active=false;
        lootbox->falling=false;
    }
    /*if (lootbox->active){
        int *yAxis = (int *)malloc(sizeof(int) * 2);
        yAxis[0] = player->y;
        yAxis[1] = player->y - player->h;
        if (!player->mirror)
        {
            if (yAxis[0] >= lootbox->y && yAxis[1] <= lootbox->y - lootbox->h)
            {
                if (player->x + player->w >= lootbox->x && player->backwards != 1)
                {
                    if (lootbox->collided != 0)
                    {
                        lootbox->collided = 3;
                    }
                    else
                    {
                        lootbox->collided = player_number;
                    }
                }
            }
        }
        else
        {
            if (yAxis[0] >= lootbox->y && yAxis[1] <= lootbox->y - lootbox->h)
            {
                if (player->x <= lootbox->x + lootbox->w && player->backwards == 1)
                {
                    if (lootbox->collided != 0)
                    {
                        lootbox->collided = 3;
                    }
                    else
                    {
                        lootbox->collided = player_number;
                    }
                }
            }
        }
    }*/
}

void apply_bonus(lootbox * lootbox, sprite_perso *player){
    int x = 50;
    if (lootbox->bonus == health_bonus){
        player->life += x;
        if (player->life > MAX_LIFE)
        {
            player->life = MAX_LIFE;
        }
    }

    if (lootbox->bonus == damage_bonus){
        player->damage_bonus = true;
    }

    if (lootbox->bonus == special_bonus){
        player->special_bar += x;
        if (player->special_bar > MAX_SPECIAL){
            player->special_bar = MAX_SPECIAL;
        }
    }
}

/**
 * @brief boucle qui permet de gérer l'apparition et la récupération des lootbox
 *
 * @param world structure contenant les données du monde
 */
void lootbox_loop(jeu *world){
    printf("Lootbox:\n");
    printf("x: %d\n", world->lootbox.x);
    printf("y: %d\n", world->lootbox.y);
    printf("Active: %d\n", world->lootbox.active);
    printf("Falling: %d\n", world->lootbox.falling);
    if (world->lootbox.active == 0){ // On ne fait pas apparaitre de lootbox s'il y en a déjà une
        int n = generate_number(0, RANDOM_SPAWN_CHANCE);
        
        if (n == 1){  // Si la fonction renvoie 1, alors on créé une lootbox
            reset_activate_lootbox(&world->lootbox); // place la lootbox de manière semi-aléatoire sur l'axe x
        }
    }

    
    if (world->lootbox.active == 1){  // Cas où une lootbox est active
        update_lootbox(world);                                // mouvement de la lootbox
        check_lootbox_pickup(&world->p1, &world->lootbox, 1); // vérifie si le joueur 1 a ramassé la lootbox
        check_lootbox_pickup(&world->p2, &world->lootbox, 2); // vérifie si le joueur 2 a ramassé la lootbox
    }

    if (world->lootbox.collided == 1){
        apply_bonus(&world->lootbox, &world->p1);
        world->lootbox.active = false;
    }

    if (world->lootbox.collided == 2){
        apply_bonus(&world->lootbox, &world->p2);
        world->lootbox.active = false;
    }

    if (world->lootbox.collided == 3){
        apply_bonus(&world->lootbox, &world->p1);
        apply_bonus(&world->lootbox, &world->p2);
        world->lootbox.active = false;
    }
}
void regenerate_shield(sprite_perso* perso){
     if (!perso->chrono_guard.pause && !perso->broken_guard && (SDL_GetTicks() - perso->chrono_guard.startTime) / 1000 >= 1 && perso->life_guard < 100 && !perso->guard)
    {
        perso->life_guard += 10;
        if(perso->special_bar >300){
            perso->special_bar=300;
        }
        perso->chrono_guard.startTime = SDL_GetTicks();
    }
}
void increase_special(sprite_perso* perso){
     if (!perso->chrono_special.pause && (SDL_GetTicks() - perso->chrono_special.startTime) / 1000 >= 0.5 && perso->special_bar < 300)
    {
        perso->special_bar+= 1;
        if(perso->special_bar >300){
            perso->special_bar=300;
        }
        perso->chrono_special.startTime = SDL_GetTicks();
    }
}

void compute_game(jeu *world){
    if(world->timer.timer == 0){
        if(world->p1.life > world->p2.life){
            printf("Victoire P1\n");
        }
        if(world->p1.life < world->p2.life){
            printf("Victoire P2\n");
        }
        else{
            printf("Egalité\n");
        }
    }

    if(world->p1.life <= 0){
        printf("Victoire P2\n");
    }

    if(world->p2.life <= 0){
        printf("Victoire P1\n");
    }
}

/**
 * MISE A JOUR DES DONNEES DU JEU
 */

void update_data(jeu *world)
{

   regenerate_shield(&world->p1);
   regenerate_shield(&world->p2);
   increase_special(&world->p1);
   increase_special(&world->p2);
  // move_fireball(world,&world->p1);
   //move_fireball(world,&world->p2);
    lootbox_loop(world);
    
    
    
    
    

    movements(world, &world->p1, &world->p2);
    movements(world, &world->p2, &world->p1);
    sprites_collision(&world->p1, &world->p2, world);
    sprites_collision(&world->p2, &world->p1, world);
    change_directions(&world->p1, &world->p2);
    world->timestamp_w++;

    check_timer(world);
    compute_game(world);
}