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
    read_counter(world);
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
    case city_night:
        world->map.image_fond = load_image("build/ressources/map/city_night/city.bmp", renderer);
        world->map.map_structure = read_file_map("build/ressources/map/city_night/city_night_structure");
        world->map.plateformes = load_image("build/ressources/map/city_night/plateforme.bmp", renderer);
        break;

    case forest:
        world->map.image_fond = load_image("build/ressources/map/forest/forest.bmp", renderer);
        world->map.map_structure = read_file_map("build/ressources/map/forest/forest_structure");
        world->map.plateformes = load_image("build/ressources/map/forest/plateforme.bmp", renderer);
        break;

    case japan:
        world->map.image_fond = load_image("build/ressources/map/Japan/Japan.bmp", renderer);
        world->map.map_structure = read_file_map("build/ressources/map/Japan/Japan_structure");
        world->map.plateformes = load_image("build/ressources/map/Japan/plateforme.bmp", renderer);
        break;
    }
}

// fonction pour les miniatures de maps
void init_miniature(jeu *world, SDL_Renderer *renderer)
{
    world->menu_set.tab_map[0] = load_image("build/ressources/map/city_night/city_mini.bmp", renderer);
    world->menu_set.tab_map[1] = load_image("build/ressources/map/forest/forest_mini.bmp", renderer);
    world->menu_set.tab_map[2] = load_image("build/ressources/map/Japan/Japan_mini.bmp", renderer);
}

void init_music(jeu *world)
{
    world->music.menu = Mix_LoadWAV("build/ressources/Audio/alex-productions-cyberpunk-gaming-electro-stardust.wav");
    world->music.game_music = Mix_LoadWAV("build/ressources/Audio/twisterium-mad-power.wav"); 
    world->music.aura = Mix_LoadWAV("build/ressources/Audio/Aura.wav");
    world->music.berserk_scream = Mix_LoadWAV("build/ressources/Audio/Berserk_Scream.wav");
    world->music.fireball_sound = Mix_LoadWAV("build/ressources/Audio/Fireball.wav");
    world->music.fireball_explosion = Mix_LoadWAV("build/ressources/Audio/FireballExplosion.wav");
    world->music.gravityball_sound = Mix_LoadWAV("build/ressources/Audio/GravityBall.wav");
    world->music.grunt = Mix_LoadWAV("build/ressources/Audio/Grunt.wav");
    world->music.heavy_punch = Mix_LoadWAV("build/ressources/Audio/HeavyPunch.wav");
    world->music.hit_guard = Mix_LoadWAV("build/ressources/Audio/HitGuard.wav");
    world->music.kick = Mix_LoadWAV("build/ressources/Audio/Kick.wav");
    world->music.kick_air = Mix_LoadWAV("build/ressources/Audio/KickAir.wav");
    world->music.light_punch = Mix_LoadWAV("build/ressources/Audio/LightPunch.wav");
    world->music.menu_selector = Mix_LoadWAV("build/ressources/Audio/Menu_Selection.wav");
    world->music.punch_air = Mix_LoadWAV("build/ressources/Audio/PunchAir.wav");
    world->music.win_laugh = Mix_LoadWAV("build/ressources/Audio/Win_laugh.wav");
    world->music.menuPlayed = false;
}

void init_jeu(jeu *world, SDL_Renderer *renderer)
{
    srand(time(NULL));
    world->menu_set.menu = load_image("build/ressources/menu/menu.bmp", renderer);
    world->menu_set.options_menu = load_image("build/ressources/menu/options.bmp", renderer);
    world->menu_set.selection_maps_menu = load_image("build/ressources/menu/Maps_selector.bmp", renderer);
    world->menu_set.menu_fond = world->menu_set.menu;
    world->state = main_menu;
    world->terminer = false;
    world->menu_set.index_menu = 0;
    world->font.police_compteur = TTF_OpenFont("build/ressources/Polices/Reach_Story.ttf", 40);
    world->font.police_victoire = TTF_OpenFont("build/ressources/Polices/Reach_Story.ttf", 150);
    init_music(world);
    init_miniature(world, renderer);
    world->menu_set.cadreVie = load_image("./build/ressources/cadres.bmp", renderer);
    world->p1.audiochan.action = 2;
    world->p1.audiochan.aura = 3;
    world->p1.audiochan.chara_voice = 4;
    world->p2.audiochan.action = 5;
    world->p2.audiochan.aura = 6;
    world->p2.audiochan.chara_voice = 7;
    world->game_over = false;
}

void init_perso(SDL_Renderer *renderer, sprite_perso *perso, int x, int y, int w, int h, int speed, bool mirror, int choice)
{
    perso->perso_choisi = choice;
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
    perso->life = MAX_LIFE;
    perso->attack_launched = false;
    perso->guard = false;
    perso->life_guard = MAX_GUARD;
    perso->damage_bonus = false;
    perso->damage_bonus = false;
    init_hits(perso);
    init_combos(perso);
    perso->pos_tab_combo = 0;
    perso->special_bar = 300;
    perso->fireball.fireball = load_image("./build/ressources/Characters/Powers/fireball.bmp", renderer);
    perso->gravityball.fireball = load_image("./build/ressources/Characters/Powers/BlackHole1.bmp", renderer);

    perso->fireball.launched_fireball = false;
    perso->gravityball.launched_fireball = false;
    perso->buffer = malloc(sizeof(inputs) * BUFFER_SIZE);
    perso->double_jump = true;
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
    perso->hits.special_attack2 = malloc(sizeof(hit));

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

    perso->hits.heavy_punch->delay = 5;
    perso->hits.heavy_punch->launch = 0;

    perso->hits.kick->dmg = 30;
    perso->hits.kick->speed = 0;
    perso->hits.kick->range_x = 30;
    perso->hits.kick->range_y = 0;
    perso->hits.kick->effective_frame = 7;

    perso->hits.kick->delay = 20;
    perso->hits.kick->launch = 0;

    perso->hits.special_attack->dmg = 50;
    perso->hits.special_attack->speed = 10;
    perso->hits.special_attack->range_x = 400;
    perso->hits.special_attack->range_y = 0;
    perso->hits.special_attack->effective_frame = 0;
    perso->hits.special_attack->delay = 20;

    perso->hits.special_attack2->dmg = 10;
    perso->hits.special_attack2->speed = 3;
    perso->hits.special_attack2->range_x = 400;
    perso->hits.special_attack2->range_y = 0;
    perso->hits.special_attack2->effective_frame = 0;
    perso->hits.special_attack2->delay = 20;
}

void init_combo(int pos, int nb_coups, int frame_between, int required, sprite_perso *player)
{
    player->tab_combo[pos].nb_coups = nb_coups;
    player->tab_combo[pos].frame_between = frame_between;
    player->tab_combo[pos].required = required;
}

void init_combos(sprite_perso *player)
{
    // 1er combo
    init_combo(0, 3, 50, 100, player);
    player->tab_combo[0].input[0] = right;
    player->tab_combo[0].input[1] = left;
    player->tab_combo[0].input[2] = light_p;

    // 2eme combos
    init_combo(1, 3, 50, 200, player);
    player->tab_combo[1].input[0] = right;
    player->tab_combo[1].input[1] = left;
    player->tab_combo[1].input[2] = heavy_p;

    init_combo(2, 3, 50, 200, player);
    player->tab_combo[2].input[0] = right;
    player->tab_combo[2].input[1] = left;
    player->tab_combo[2].input[2] = kick;
}

void init_lootbox(lootbox *lootbox, SDL_Renderer *renderer)
{
    lootbox->texture[health_bonus] = load_image("build/ressources/Bonuses/Life_bonus.bmp", renderer);
    lootbox->texture[damage_bonus] = load_image("build/ressources/Bonuses/Damage_bonus.bmp", renderer);
    lootbox->texture[special_bonus] = load_image("build/ressources/Bonuses/Special_bonus.bmp", renderer);
    lootbox->x = generate_number(65, 950);
    lootbox->y = 0 - lootbox->h;
    lootbox->h = 25;
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("erreur SDL_mix %S", Mix_GetError());
        SDL_Quit();
    }
    else
    {
        Mix_AllocateChannels(8);   
        Mix_Volume(0, 30); 
        Mix_Volume(1, 70);  
        Mix_Volume(2, 70);
        Mix_Volume(3, 70);
        Mix_Volume(4, 70);
        Mix_Volume(5, 70);
        Mix_Volume(6, 70);
        Mix_Volume(7, 70);
        Mix_Volume(8, 70);

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
        init_state_animation(renderer, perso, idle, "build/ressources/Characters/Chara1/Idle.bmp", 15, 90);
        init_state_animation(renderer, perso, walk, "build/ressources/Characters/Chara1/Walking.bmp", 15, 90);
        init_state_animation(renderer, perso, jump, "build/ressources/Characters/Chara1/Jump.bmp", 10, 85);
        init_state_animation(renderer, perso, crouch, "build/ressources/Characters/Chara1/Idle.bmp", 15, 85);
        init_state_animation(renderer, perso, fall, "build/ressources/Characters/Chara1/Falling.bmp", 10, 95);
        init_state_animation(renderer, perso, backwards, "build/ressources/Characters/Chara1/Walking.bmp", 15, 85);
        init_state_animation(renderer, perso, flight, "build/ressources/Characters/Chara1/Jump.bmp", 10, 85);
        init_state_animation(renderer, perso, flight_control, "build/ressources/Characters/Chara1/Jump.bmp", 10, 85);
        init_state_animation(renderer, perso, fall_control, "build/ressources/Characters/Chara1/Falling.bmp", 10, 85);
        init_state_animation(renderer, perso, landing, "build/ressources/Characters/Chara1/Landing.bmp", 5, 90);
        init_state_animation(renderer, perso, knockback, "build/ressources/Characters/Chara1/KnockBack.bmp", 5, 85);
        init_state_animation(renderer, perso, lpunch, "build/ressources/Characters/Chara1/LightPunch.bmp", 16, 85);
        init_state_animation(renderer, perso, kickstate, "build/ressources/Characters/Chara1/Kick.bmp", 14, 110);
        init_state_animation(renderer, perso, hpunch, "build/ressources/Characters/Chara1/HeavyPunch.bmp", 15, 85);
        init_state_animation(renderer, perso, stun, "build/ressources/Characters/Chara1/Knocked.bmp", 5, 85);

        init_state_animation(renderer, perso, fireball, "build/ressources/Characters/Chara1/Fireball.bmp", 30, 90);
        init_state_animation(renderer, perso, gravityball, "build/ressources/Characters/Chara1/Fireball.bmp", 30, 90);
        init_state_animation(renderer, perso, looser, "build/ressources/Characters/Chara1/Defeat.bmp", 28, 90);
        init_state_animation(renderer, perso, winner, "build/ressources/Characters/Chara1/Victory.bmp", 30, 100);

        init_state_animation(renderer, perso, 19, "build/ressources/Characters/Powers/Aura_Spritesheet.Bmp", 8, 190);
        init_state_animation(renderer, perso, 20, "build/ressources/Characters/Powers/Transparent_Aura_Spritesheet.Bmp", 8, 190);
        perso->anim[19].aura = 0;
        break;

    case 1:
        init_state_animation(renderer, perso, idle, "build/ressources/Characters/Chara2/Idle.bmp", 15, 90);
        init_state_animation(renderer, perso, walk, "build/ressources/Characters/Chara2/Walking.bmp", 15, 90);
        init_state_animation(renderer, perso, jump, "build/ressources/Characters/Chara2/Jump.bmp", 10, 85);
        init_state_animation(renderer, perso, crouch, "build/ressources/Characters/Chara2/Idle.bmp", 15, 85);
        init_state_animation(renderer, perso, fall, "build/ressources/Characters/Chara2/Falling.bmp", 10, 90);
        init_state_animation(renderer, perso, backwards, "build/ressources/Characters/Chara2/Walking.bmp", 15, 85);
        init_state_animation(renderer, perso, flight, "build/ressources/Characters/Chara2/Jump.bmp", 10, 85);
        init_state_animation(renderer, perso, flight_control, "build/ressources/Characters/Chara2/Jump.bmp", 10, 85);
        init_state_animation(renderer, perso, fall_control, "build/ressources/Characters/Chara2/Falling.bmp", 10, 95);
        init_state_animation(renderer, perso, landing, "build/ressources/Characters/Chara2/Landing.bmp", 5, 90);
        init_state_animation(renderer, perso, knockback, "build/ressources/Characters/Chara2/KnockBack.bmp", 5, 85);
        init_state_animation(renderer, perso, lpunch, "build/ressources/Characters/Chara2/LightPunch.bmp", 16, 85);
        init_state_animation(renderer, perso, kickstate, "build/ressources/Characters/Chara2/Kick.bmp", 14, 110);
        init_state_animation(renderer, perso, hpunch, "build/ressources/Characters/Chara2/HeavyPunch.bmp", 15, 85);
        init_state_animation(renderer, perso, stun, "build/ressources/Characters/Chara2/Knocked.bmp", 5, 85);

        init_state_animation(renderer, perso, fireball, "build/ressources/Characters/Chara2/Fireball.bmp", 30, 90);
        init_state_animation(renderer, perso, gravityball, "build/ressources/Characters/Chara2/Fireball.bmp", 30, 90);
        init_state_animation(renderer, perso, looser, "build/ressources/Characters/Chara2/Defeat.bmp", 28, 90);
        init_state_animation(renderer, perso, winner, "build/ressources/Characters/Chara2/Victory.bmp", 30, 100);

        init_state_animation(renderer, perso, 19, "build/ressources/Characters/Powers/Aura_Spritesheet.Bmp", 8, 190);
        init_state_animation(renderer, perso, 20, "build/ressources/Characters/Powers/Transparent_Aura_Spritesheet.Bmp", 8, 190);
        perso->anim[19].aura = 0;
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
    tmp = SDL_LoadBMP(path);
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
        if (player->special_bar >= player->tab_combo[val].required)
        {
            player->special_bar -= player->tab_combo[val].required;
            switch (val)
            {
            case 0:
                player->chara_state = fireball;
                player->fireball.multiplicateur = player->mirror ? -1 : 1;
                break;
            case 1:

                player->chara_state = gravityball;
                player->gravityball.multiplicateur = player->mirror || player->x > 630 ? -1 : 1;
                break;
            case 2:
                player->berserk = true;
                player->damage_bonus = true;
                player->dmg_bonus_timer.start = true;
                player->dmg_bonus_timer.startTime = SDL_GetTicks();
                player->dmg_bonus_timer.pause = false;
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

    if (world->timer.start && !world->timer.pause && !world->timer.inf)
    {

        if ((SDL_GetTicks() - world->timer.startTime) / 1000 >= 1)
        {
            world->timer.startTime = SDL_GetTicks();
            world->timer.timer--;
        }
    }
}

void save_counter(char *select)
{
    FILE *file = fopen("build/counter.txt", "w");
    fwrite(select, strlen(select), 1, file);
    fclose(file);
}
void read_counter(jeu *world)
{
    char *File_name = "build/counter.txt";
    struct stat stat_file;
    FILE *file = fopen(File_name, "r"); // ouverture en mode lecture
    stat(File_name, &stat_file);
    int sz = stat_file.st_size;
    char text_file[sz + 1];
    fread(text_file, 1, sz, file);
    text_file[sz] = '\0';
    if (strcmp(text_file, "inf") == 0)
    {
        world->timer.inf = true;
    }
    else
    {
        world->timer.inf = false;
        world->timer.timer = atoi(text_file);
    }
}

/**
 * FONCTIONS MOUVEMENTS + COUPS
 */
void reset_state(sprite_perso *perso, enum character_state state)
{
    for (int i = 0; i <= gravityball; i++)
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

        if (perso->anim[jump].frame == perso->anim[jump].nbFrame)
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

    if (perso->chara_state == knockback)
    {
        if (!perso->mirror)
        {
            if (!equals(perso->x - perso->speed, perso->y + perso->h, world->map.map_structure, '0'))
            {
                perso->x -= perso->speed;
            }
        }

        else
        {
            if (!equals(perso->x + perso->speed, perso->y + perso->h, world->map.map_structure, '0'))
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
            if (equals(x, y + perso->h + perso->jump_height * 0.025, world->map.map_structure, ' ') && equals(x + perso->w, y + perso->h + perso->jump_height * 0.025, world->map.map_structure, ' '))
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

    if (perso->chara_state != flight && equals(x, y + perso->h + perso->jump_height * 0.025, world->map.map_structure, ' ') && equals(x + perso->w, y + perso->h + perso->jump_height * 0.025, world->map.map_structure, ' '))
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
                perso->double_jump = true;
                perso->anim[landing].frame = 0;
            }
            perso->anim[landing].counter = 0;
        }
        else
        {
            perso->anim[landing].counter++;
        }
    }

    if (perso->chara_state == lpunch)
    {
        if (perso->anim[lpunch].counter == 2)
        {
            perso->anim[lpunch].frame++;
            if (perso->anim[lpunch].frame == perso->hits.light_punch->effective_frame)
            {

                light_punch(perso, adversaire, world);
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
                heavy_punch(perso, adversaire, world);
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
                kick_hit(perso, adversaire, world);
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
                perso->anim[stun].counter = 0;
                if (perso->anim[stun].frame = perso->anim[stun].nbFrame)
                {
                    perso->anim[stun].frame = 4;
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

/*THROWABLE*/
if (perso->chara_state == fireball)
    {
        if(perso->anim[fireball].counter>=1){
            perso->anim[fireball].frame++;
            perso->anim[fireball].counter =0;
            if (perso->anim[fireball].frame == perso->anim[fireball].nbFrame)
            {
                perso->fireball.launched_fireball = true;
                perso->chara_state = idle;
            }
        }
        else{
            perso->anim[fireball].counter++;
        }
    }
    if (perso->chara_state == gravityball)
    
    {   
        if(perso->anim[gravityball].counter>=1){
            if (!perso->gravityball.launched_fireball)
            {
                perso->gravityball.timer_throw.startTime = SDL_GetTicks();
                perso->gravityball.timer_throw.pause = false;
            }
            perso->anim[gravityball].counter = 0;
            perso->anim[gravityball].frame++;
            if (perso->anim[gravityball].frame == perso->anim[gravityball].nbFrame)
            {
                perso->gravityball.launched_fireball = true;
                perso->chara_state = idle;
            }
        }
        else{
            perso->anim[gravityball].counter++;
        }
    }
}

void manage_aura(sprite_perso *perso)
{
    if (perso->anim[19].counter >= 5)
    {
        if (perso->anim[19].aura && !perso->damage_bonus)
        {
            if (perso->anim[19].frame != perso->anim[19].nbFrame)
            {
                perso->anim[19].frame++;
                perso->anim[20].frame++;
            }
            else
            {
                perso->anim[19].aura = 0;
                perso->anim[19].frame = 0;
                perso->anim[20].frame = 0;
            }
        }

        if (!perso->anim[19].aura && perso->damage_bonus)
        {
            perso->anim[19].frame++;
            perso->anim[20].frame++;
            if (perso->anim[19].frame == 2)
            {
                perso->anim[19].aura = 1;
            }
        }

        if (perso->damage_bonus && perso->anim[19].aura)
        {
            if (perso->anim[19].frame == 4)
            {
                perso->anim[19].frame = 2;
                perso->anim[20].frame = 2;
            }
            else
            {
                perso->anim[19].frame++;
                perso->anim[20].frame++;
            }
        }
        perso->anim[19].counter = 0;
    }
    else
    {
        perso->anim[19].counter++;
    }
}

bool equals(int x, int y, char **map_point, char test)
{
    int height_factor, width_factor;
    height_factor = CELL_HEIGHT;
    width_factor = CELL_WIDTH;
    return (map_point[y / height_factor][x / width_factor] == test);
}

void light_punch(sprite_perso *attacker, sprite_perso *receiver, jeu *world)
{

    if (!attacker->mirror)
    {
        if ((attacker->x + attacker->w + attacker->hits.light_punch->range_x >= receiver->x) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            Mix_PlayChannel(attacker->audiochan.action, world->music.light_punch, 0);
            if (receiver->guard)
            {
                if (!receiver->berserk)
                {   
                    Mix_PlayChannel(receiver->audiochan.action, world->music.hit_guard, 0);
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * 100 : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;
                }
            }
            else
            {
                receiver->special_bar += 10;
                attacker->special_bar += 5;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * 100 : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;

                    if (!receiver->broken_guard)
                    {
                        receiver->chara_state = stun;
                        receiver->stun_time = attacker->hits.light_punch->delay;
                    }
                }
            }
        }
        else{
            Mix_PlayChannel(attacker->audiochan.action, world->music.punch_air, 0);
        }
    }
    else
    {
        if ((attacker->x - attacker->hits.light_punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            Mix_PlayChannel(attacker->audiochan.action, world->music.light_punch, 0);
            if (receiver->guard)
            {
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.hit_guard, 0);
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * 100 : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;
                }
            }
            else
            {
                receiver->special_bar += 10;
                attacker->special_bar += 5;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * 100 : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;
                    if (!receiver->broken_guard)
                    {
                        receiver->chara_state = stun;
                        receiver->stun_time = attacker->hits.light_punch->delay;
                    }
                }
            }
        }
        else{
            Mix_PlayChannel(attacker->audiochan.action, world->music.punch_air, 0);
        }
    }
}

void heavy_punch(sprite_perso *attacker, sprite_perso *receiver, jeu *world)
{

    if (!attacker->mirror)
    {
        if ((attacker->x + attacker->w + attacker->hits.heavy_punch->range_x >= receiver->x) && (attacker->y + attacker->h / 2 >= receiver->y && attacker->y + attacker->h / 2 <= receiver->y + receiver->h))
        {
            
            Mix_PlayChannel(attacker->audiochan.action, world->music.heavy_punch, 0);
            if (receiver->guard)
            {
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.action, world->music.hit_guard, 0);
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * 100 : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                attacker->special_bar += 10;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * 100 : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
                    if (!receiver->broken_guard)
                    {
                        receiver->chara_state = stun;
                        receiver->stun_time = attacker->hits.heavy_punch->delay;
                    }
                }
            }
        }
        else{
            Mix_PlayChannel(attacker->audiochan.action, world->music.punch_air, 0);
        }
    }
    else
    {
        if ((attacker->x - attacker->hits.heavy_punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            Mix_PlayChannel(attacker->audiochan.action, world->music.heavy_punch, 0);
            if (receiver->guard)
            {
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.action, world->music.hit_guard, 0);
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * 100 : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                attacker->special_bar += 10;
                if (!receiver->berserk)
                {
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * 100 : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
                    if (!receiver->broken_guard)
                    {
                        Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                        receiver->chara_state = stun;
                        receiver->stun_time = attacker->hits.heavy_punch->delay;
                    }
                }
            }
        }
        else{
            Mix_PlayChannel(attacker->audiochan.action, world->music.punch_air, 0);
        }
    }
}

void kick_hit(sprite_perso *attacker, sprite_perso *receiver, jeu *world)
{

    if (!attacker->mirror)
    {
        if ((attacker->x + attacker->w + attacker->hits.kick->range_x >= receiver->x) && (attacker->y + attacker->h / 2 >= receiver->y && attacker->y + attacker->h / 2 <= receiver->y + receiver->h))
        {
            Mix_PlayChannel(attacker->audiochan.action, world->music.kick, 0);
            if (receiver->guard)
            {
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.action, world->music.hit_guard, 0);
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * 100 : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * 100 : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.kick, 0);
                    if (!receiver->broken_guard)
                    {
                        receiver->chara_state = stun;
                        receiver->stun_time = attacker->hits.kick->delay;
                    }
                }
            }
        }
        else{
            Mix_PlayChannel(attacker->audiochan.action, world->music.kick_air, 0);
        }
    }
    else
    {
        if ((attacker->x - attacker->hits.heavy_punch->range_x <= receiver->x + receiver->w) && (attacker->y + attacker->jump_height / 2 >= receiver->y && attacker->y + attacker->jump_height / 2 <= receiver->y + receiver->h))
        {
            Mix_PlayChannel(attacker->audiochan.action, world->music.kick, 0);
            if (receiver->guard)
            {
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.action, world->music.hit_guard, 0);
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * 100 : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * 100 : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                    if (!receiver->broken_guard)
                    {
                        receiver->chara_state = stun;
                        receiver->stun_time = attacker->hits.kick->delay;
                    }
                }
            }
        }
        else{
            Mix_PlayChannel(attacker->audiochan.action, world->music.kick_air, 0);
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
            if (perso->x + perso->w + perso->speed >= adversaire->x && perso->backwards != 1)
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
    int tete = p1->y;
    int pieds = p1->y - p1->h;
    if (p1->chara_state != jump || p2->chara_state != jump || p1->chara_state != flight || p2->chara_state != flight || p1->chara_state != flight_control || p2->chara_state != flight_control)
    {
        if (tete >= p2->y && pieds <= p2->y - p2->h)
        {
            if (!p1->mirror && p2->mirror)
            {
                if (p1->x + p1->w >= p2->x && p1->backwards != 1)
                {
                    if (!equals(p1->x + p1->speed, p1->y + p1->h, world->map.map_structure, '0'))
                    {
                        p1->chara_state = knockback;
                    }
                    else
                    {
                        p1->chara_state = idle;
                    }
                }
            }

            else
            {
                if (p1->x <= p2->x + p2->w && p1->backwards == 1)
                {
                    if (!equals(p1->x + p1->speed, p1->y + p1->h, world->map.map_structure, '0'))
                    {
                        p1->chara_state = knockback;
                    }
                    else
                    {
                        p1->chara_state = idle;
                    }
                }
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

void free_hits(sprite_perso *perso)
{

    free(perso->buffer);
    free(perso->hits.light_punch);
    free(perso->hits.heavy_punch);
    free(perso->hits.kick);
}

void free_music(jeu *world)
{
    Mix_FreeChunk(world->music.menu);
    Mix_FreeChunk(world->music.game_music);
    Mix_FreeChunk(world->music.aura);
    Mix_FreeChunk(world->music.berserk_scream);
    Mix_FreeChunk(world->music.fireball_sound);
    Mix_FreeChunk(world->music.fireball_explosion);
    Mix_FreeChunk(world->music.gravityball_sound);
    Mix_FreeChunk(world->music.grunt);
    Mix_FreeChunk(world->music.heavy_punch);
    Mix_FreeChunk(world->music.hit_guard);
    Mix_FreeChunk(world->music.kick);
    Mix_FreeChunk(world->music.kick_air);
    Mix_FreeChunk(world->music.light_punch);
    Mix_FreeChunk(world->music.menu_selector);
    Mix_FreeChunk(world->music.punch_air);
    Mix_FreeChunk(world->music.win_laugh);
}

/*FONCTIONS DE LIBERATION ESPACE*/
void quit_game(jeu *world, SDL_Window **fenetre, SDL_Renderer **renderer)
{
    free_hits(&world->p1);
    free_hits(&world->p2);
    destroy_textures(world);
    free_map_structure(world->map.map_structure);
    free_music(world);
    TTF_CloseFont(world->font.police_compteur);
    close_Joystick(world->joysticks);
    TTF_Quit();
    Mix_CloseAudio();

    IMG_Quit();
    SDL_DestroyWindow(*fenetre);
    SDL_DestroyRenderer(*renderer);
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

void destroy_anim(sprite_perso *perso)
{
    SDL_DestroyTexture(perso->anim[idle].anim_text);
    SDL_DestroyTexture(perso->anim[walk].anim_text);
    SDL_DestroyTexture(perso->anim[jump].anim_text);
    SDL_DestroyTexture(perso->anim[crouch].anim_text);
    SDL_DestroyTexture(perso->anim[fall].anim_text);
    SDL_DestroyTexture(perso->anim[backwards].anim_text);
    SDL_DestroyTexture(perso->anim[flight].anim_text);
    SDL_DestroyTexture(perso->anim[flight_control].anim_text);
    SDL_DestroyTexture(perso->anim[fall_control].anim_text);
    SDL_DestroyTexture(perso->anim[landing].anim_text);
    SDL_DestroyTexture(perso->anim[knockback].anim_text);
    SDL_DestroyTexture(perso->anim[lpunch].anim_text);
    SDL_DestroyTexture(perso->anim[kickstate].anim_text);
    SDL_DestroyTexture(perso->anim[hpunch].anim_text);
    SDL_DestroyTexture(perso->anim[stun].anim_text);
    SDL_DestroyTexture(perso->fireball.fireball);
}

void destroy_map(jeu *world)
{
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
    for (int i = 0; i < special_bonus; i++)
    {
        SDL_DestroyTexture(world->lootbox.texture[i]);
    }
    destroy_map(world);
}

void unpause(compteur *chrono)
{
    if (chrono->start)
    {
        chrono->startTime = SDL_GetTicks() - chrono->pauseTime;
        chrono->pause = false;
    }
}
/*INPUTS GAMEPLAY*/

/**
 * MENU INPUTS
 */
void handle_menu_inputs(SDL_Event *event, jeu *world, SDL_Renderer *renderer)
{
    Uint8 *keystates;

    while (SDL_PollEvent(event))
    {
        if (!world->music.menuPlayed)
        {
            
            Mix_PlayChannel(music, world->music.menu, -1);
            world->music.menuPlayed = true;
        }
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
                    Mix_Resume(music);
                    unpause(&world->timer);
                    unpause(&world->p1.chrono_guard);
                    unpause(&world->p2.chrono_guard);
                    unpause(&world->p1.chrono_special);
                    unpause(&world->p2.chrono_special);
                    unpause(&world->p1.dmg_bonus_timer);
                    unpause(&world->p2.dmg_bonus_timer);
                    unpause(&world->p1.gravityball.timer_throw);
                    unpause(&world->p2.gravityball.timer_throw);
                }

                else if (world->state == options)
                {
                    world->state = main_menu;
                    world->menu_set.menu_fond = world->menu_set.menu;
                    world->menu_set.index_menu = 0;
                }

                else if (world->state == selection_map)
                {
                    world->state = main_menu;
                    world->menu_set.menu_fond = world->menu_set.menu;
                    world->menu_set.index_menu = 0;
                }
            }

            if (event->key.keysym.sym == SDLK_z)
            {
                Mix_PlayChannel(selector,world->music.menu_selector,0);
                if (world->state == main_menu)

                {
                    world->menu_set.index_menu--;
                    if (world->menu_set.index_menu < 0)
                    {
                        world->menu_set.index_menu = 2;
                    }
                }
                if (world->state == pause)
                {
                    world->menu_set.index_menu--;
                    if (world->menu_set.index_menu < 0)
                    {
                        world->menu_set.index_menu = 1;
                    }
                }
            }

            if (event->key.keysym.sym == SDLK_s)
            {

                Mix_PlayChannel(selector, world->music.menu_selector, 0);
                if (world->state == main_menu)
                {
                    world->menu_set.index_menu++;
                    if (world->menu_set.index_menu > 2)
                    {
                        world->menu_set.index_menu = 0;
                    }
                }
                if (world->state == pause)
                {
                    world->menu_set.index_menu++;
                    if (world->menu_set.index_menu > 1)
                    {
                        world->menu_set.index_menu = 0;
                    }
                }
            }

            if (event->key.keysym.sym == SDLK_d)
            {
                Mix_PlayChannel(selector, world->music.menu_selector, 0);
                if (world->state == selection_map)
                {
                    
                    world->menu_set.index_menu++;
                    if (world->menu_set.index_menu > NB_MAPS - 1)
                    {
                        world->menu_set.index_menu = 0;
                    }
                }
                if (world->state == options)
                {
                    world->menu_set.index_menu == 3 ? world->menu_set.index_menu = 0 : world->menu_set.index_menu++;
                }
            }

            if (event->key.keysym.sym == SDLK_q)
            {
                Mix_PlayChannel(selector, world->music.menu_selector, 0);
                if (world->state == selection_map)
                {
                    world->menu_set.index_menu--;
                    if (world->menu_set.index_menu < 0)
                    {

                        world->menu_set.index_menu = NB_MAPS - 1;
                    }
                }
                if (world->state == options)
                {
                    world->menu_set.index_menu == 0 ? world->menu_set.index_menu = 3 : world->menu_set.index_menu--;
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
                        world->menu_set.menu_fond = world->menu_set.selection_maps_menu;
                        world->state = selection_map;

                        break;
                    case 1:
                        world->state = options;
                        world->menu_set.menu_fond = world->menu_set.options_menu;
                        world->menu_set.index_menu = 0;
                        break;
                    case 2:
                        world->terminer = true;
                        break;
                    }
                }
                else if (world->state == selection_map)
                {
                    Mix_PlayChannel(music, world->music.menu, 0);
                    switch (world->menu_set.index_menu)
                    {
                    case 0:
                        world->choosed_map = city_night;
                        break;
                    case 1:
                        world->choosed_map = forest;
                        break;
                    case 2:
                        world->choosed_map = japan;
                        break;
                    }

                    init_map(world, renderer);
                    if (world->choosed_map == forest)
                    {
                        init_perso(renderer, &world->p1, 256, 465, 55, 230, CHARA_SPEED, false, 0);
                        init_perso(renderer, &world->p2, 950, 465, 55, 230, CHARA_SPEED, true, 1);
                    }
                    else
                    {
                        init_perso(renderer, &world->p1, 65, 465, 55, 230, CHARA_SPEED, false, 0);
                        init_perso(renderer, &world->p2, 950, 465, 55, 230, CHARA_SPEED, true, 1);
                    }
                    init_controller(world);
                    world->game_over = false;
                    world->state = combat;
                    world->timestamp_w = 0;

                    init_timer(world);
                    for (int i = 0; i < 123; i++)
                    {
                        world->keystates_pre[i] = 0;
                    }

                    init_lootbox(&world->lootbox, renderer);
                    world->menu_set.index_menu = 0;
                }
                else if (world->state == options)
                {
                    switch (world->menu_set.index_menu)
                    {
                    case 0:
                        save_counter("60");
                        break;
                    case 1:
                        save_counter("90");
                        break;
                    case 2:
                        save_counter("120");
                        break;
                    case 3:
                        save_counter("inf");
                        break;
                    }
                }
                else if (world->state == pause)
                {
                    switch (world->menu_set.index_menu)
                    {
                    case 0:
                        world->state = combat;
                        break;
                    case 1:
                        world->menu_set.menu_fond = world->menu_set.menu;
                        world->state = main_menu;
                        break;
                    }
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

void pauseChrono(compteur *chrono)
{
    if (chrono->start)
    {
        chrono->pause = true;
        chrono->pauseTime = SDL_GetTicks() - chrono->startTime;
    }
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
        Mix_Pause(music);
        pauseChrono(&world->timer);
        pauseChrono(&world->p1.chrono_guard);
        pauseChrono(&world->p2.chrono_guard);
        pauseChrono(&world->p1.dmg_bonus_timer);
        pauseChrono(&world->p2.dmg_bonus_timer);
        pauseChrono(&world->p1.gravityball.timer_throw);
        pauseChrono(&world->p2.gravityball.timer_throw);
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
            if (world->p1.chara_state == walk){
                world->p1.backwards == true;
            }
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
            if (world->p1.chara_state == walk){
                world->p1.backwards == false;
            }
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
            }
            if (world->p1.chara_state == jump || world->p1.chara_state == flight || world->p1.chara_state == flight_control)
            {
                world->p1.chara_state = fall;
            }
            if (keystates[SDL_SCANCODE_S] != world->keystates_pre[SDL_SCANCODE_S])
            {
                add_input_buffer(&world->p1, down, world->timestamp_w);
            }
        }

        // sauts
        if ((keystates[SDL_SCANCODE_W] /*|| SDL_GameControllerGetAxis(world->joysticks[0],1)< (-10000)*/))
        {
            if (world->p1.chara_state == idle || world->p1.chara_state == walk)
            {
                world->p1.chara_state = jump;
            }
            else
            {
                if ((world->p1.chara_state == jump || world->p1.chara_state == flight || world->p1.chara_state == flight_control || world->p1.chara_state == fall_control || world->p1.chara_state == fall))
                    if (keystates[SDL_SCANCODE_W] != world->keystates_pre[SDL_SCANCODE_W] && world->p1.double_jump)
                    {

                        world->p1.jump_origin = world->p1.y;
                        world->p1.anim[flight].frame = 0;
                        world->p1.anim[flight_control].frame = 0;
                        world->p1.chara_state = flight;

                        world->p1.double_jump = false;
                    }
                if (keystates[SDL_SCANCODE_W] != world->keystates_pre[SDL_SCANCODE_W])
                {
                    add_input_buffer(&world->p1, up, world->timestamp_w);
                }
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
                    for (int i = 0; i < NB_COMBOS && !combop1; i++)
                    {
                        combop1 = read_combo(&world->p1, i);
                        if (combop1)
                        {
                            switch (i)
                            {
                            case 1:
                                Mix_PlayChannel(world->p1.audiochan.action, world->music.fireball_sound, 0);
                                break;

                            default:
                                break;
                            }
                        }
                    }
                    if (!combop1)
                    {
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
                    for (int i = 0; i < NB_COMBOS && !combop1; i++)
                    {
                        combop1 = read_combo(&world->p1, i);
                    }
                    if (!combop1)
                    {
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
                    for (int i = 0; i < NB_COMBOS && !combop1; i++)
                    {
                        combop1 = read_combo(&world->p1, i);
                    }
                    if (!combop1)
                    {
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
            if (world->p2.chara_state == walk){
                world->p2.backwards = true;
            }
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
            if(world->p2.chara_state == walk){
                world->p2.backwards = false;
            }
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

        if (keystates[SDL_SCANCODE_UP])
        {

            if (world->p2.chara_state == idle || world->p2.chara_state == walk)
            {
                world->p2.chara_state = jump;
            }
            else
            {
                if ((world->p2.chara_state == jump || world->p2.chara_state == flight || world->p2.chara_state == flight_control || world->p2.chara_state == fall_control || world->p2.chara_state == fall))
                    if (keystates[SDL_SCANCODE_UP] != world->keystates_pre[SDL_SCANCODE_UP] && world->p2.double_jump)
                    {

                        world->p2.jump_origin = world->p2.y;
                        world->p2.anim[flight].frame = 0;
                        world->p2.anim[flight_control].frame = 0;
                        world->p2.chara_state = flight;

                        world->p2.double_jump = false;
                    }
            }
            if (keystates[SDL_SCANCODE_UP] != world->keystates_pre[SDL_SCANCODE_UP])
            {
                add_input_buffer(&world->p2, up, world->timestamp_w);
            }
        }

        /* code */

        // coups
        if (keystates[SDL_SCANCODE_KP_4] && !keystates[SDL_SCANCODE_KP_8] && !keystates[SDL_SCANCODE_KP_9])
        {
            if (keystates[SDL_SCANCODE_KP_4] != world->keystates_pre[SDL_SCANCODE_KP_4])
            {

                if (world->p2.chara_state == idle || world->p2.chara_state == walk)
                {
                    add_input_buffer(&world->p2, light_p, world->timestamp_w);
                    for (int i = 0; i < NB_COMBOS && !combop2; i++)
                    {
                        combop2 = read_combo(&world->p2, i);
                    }
                    if (!combop2)
                    {
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
                    for (int i = 0; i < NB_COMBOS && !combop2; i++)
                    {
                        combop2 = read_combo(&world->p2, i);
                    }
                    if (!combop2)
                    {
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
                    for (int i = 0; i < NB_COMBOS && !combop2; i++)
                    {
                        combop2 = read_combo(&world->p2, i);
                    }
                    if (!combop2)
                    {
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

int generate_number(int a, int b)
{
    return rand() % (b - a) + a;
}

void reset_activate_lootbox(lootbox *lootbox)
{
    lootbox->active = true;
    lootbox->falling = true;
    lootbox->x = generate_number(65, 950);
    lootbox->y = 0 - lootbox->h;
    lootbox->collided = 0;
    lootbox->bonus = generate_number(health_bonus, special_bonus + 1);
}

void update_lootbox(jeu *world)
{
    if (world->lootbox.falling)
    {
        if ((equals(world->lootbox.x, world->lootbox.y + world->lootbox.h + world->lootbox.fallspeed, world->map.map_structure, ' ') || (equals(world->lootbox.x + world->lootbox.w, world->lootbox.y + world->lootbox.h + world->lootbox.fallspeed, world->map.map_structure, ' ') || world->lootbox.y < CELL_HEIGHT + 20) && world->lootbox.y < 667))
        {

            world->lootbox.y += world->lootbox.fallspeed;
        }
        else
        {
            world->lootbox.falling = false;
        }
    }
}

void check_lootbox_pickup(sprite_perso *player, lootbox *lootbox, int player_number)
{
    if (lootbox->active)
    {
        int tete = player->y;
        int pieds = player->y + player->h;

        if (lootbox->y >= tete && lootbox->y <= pieds)
        {
            if (player->x + player->w >= lootbox->x && player->x <= lootbox->x + lootbox->w)
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
}

void apply_bonus(lootbox *lootbox, sprite_perso *player)
{
    int x = 50;
    if (lootbox->bonus == health_bonus)
    {
        player->life += x;
    }

    if (!player->damage_bonus && lootbox->bonus == damage_bonus && !player->anim[19].aura)
    {
        player->damage_bonus = true;
        player->dmg_bonus_timer.start = true;
        player->dmg_bonus_timer.startTime = SDL_GetTicks();
        player->dmg_bonus_timer.pause = false;
    }

    if (lootbox->bonus == special_bonus)
    {
        player->special_bar += x;
    }
}

/**
 * Système de lootbox
 */
void lootbox_loop(jeu *world)
{
    if (world->lootbox.active == 0)
    { // On ne fait pas apparaitre de lootbox s'il y en a déjà une
        int n = generate_number(0, RANDOM_SPAWN_CHANCE);

        if (n == 1)
        {                                            // Si la fonction renvoie 1, alors on créé une lootbox
            reset_activate_lootbox(&world->lootbox); // place la lootbox de manière semi-aléatoire sur l'axe x
        }
    }

    if (world->lootbox.active == 1)
    {                                                         // Cas où une lootbox est active
        update_lootbox(world);                                // mouvement de la lootbox
        check_lootbox_pickup(&world->p1, &world->lootbox, 1); // vérifie si le joueur 1 a ramassé la lootbox
        check_lootbox_pickup(&world->p2, &world->lootbox, 2); // vérifie si le joueur 2 a ramassé la lootbox

        if (world->lootbox.collided == 1)
        {
            apply_bonus(&world->lootbox, &world->p1);
            world->lootbox.active = false;
        }

        if (world->lootbox.collided == 2)
        {
            apply_bonus(&world->lootbox, &world->p2);
            world->lootbox.active = false;
        }

        if (world->lootbox.collided == 3)
        {
            apply_bonus(&world->lootbox, &world->p1);
            apply_bonus(&world->lootbox, &world->p2);
            world->lootbox.active = false;
        }
    }
}

void regenerate_shield(sprite_perso *perso)
{
    if (!perso->chrono_guard.pause && !perso->broken_guard && (SDL_GetTicks() - perso->chrono_guard.startTime) / 1000 >= 1 && perso->life_guard < 100 && !perso->guard)
    {
        perso->life_guard += 10;
        if (perso->special_bar > 300)
        {
            perso->special_bar = 300;
        }
        perso->chrono_guard.startTime = SDL_GetTicks();
    }
}

void increase_special(sprite_perso *perso)
{
    if (!perso->chrono_special.pause && (SDL_GetTicks() - perso->chrono_special.startTime) / 1000 >= 0.5 && perso->special_bar < 300)
    {
        perso->special_bar += 1;
        if (perso->special_bar > 300)
        {
            perso->special_bar = 300;
        }
        perso->chrono_special.startTime = SDL_GetTicks();
    }
}

int numDigits(int n)
{
    int r = 1;
    if (n < 0)
        n = -n;
    while (n > 9)
    {
        n /= 10;
        r++;
    }
    return r;
}

void write_victory(int nb1, int nb2)
{
    char new_score[8 + numDigits(nb1) + numDigits(nb2)];
    FILE *file = fopen("build/victory.txt", "w");
    sprintf(new_score, "p1:%d\np2:%dn", nb1, nb2);
    fwrite(new_score, 7 + numDigits(nb1) + numDigits(nb2), 1, file);
    fclose(file);
}

void save_victory(int player)
{
    char *File_name = "build/victory.txt";
    struct stat stat_file;
    FILE *file = fopen(File_name, "r"); // ouverture en mode lecture
    stat(File_name, &stat_file);
    int sz = stat_file.st_size;
    char text_file[sz + 1];
    fread(text_file, 1, sz, file);
    text_file[sz] = '\0';

    int line = 1;
    int size;
    int nb1 = 0;
    int nb2 = 0;
    int i, j;
    for (i = 0; i < sz; i++)
    {

        if (text_file[i] == '\n')
        {
            line++;
        }
        if (text_file[i] == ':')
        {
            size = 0;

            while (text_file[i + size + 1] != '\n')
            {
                size++;
            }
            i += 1;
            int index = 0;
            char nombre[size + 1];
            int pos = 0;
            nombre[size] = '\0';
            while (text_file[i + index] != '\n' && index < size)
            {

                nombre[pos++] = text_file[i + index];

                index++;
            }

            if (line == 1)
            {
                nb1 = atoi(nombre);
            }
            if (line == 2)
            {
                nb2 = atoi(nombre);
            }
        }
    }

    if (player == 1)
    {
        nb1 += 1;
    }
    else
    {
        nb2 += 1;
    }
    fclose(file);
    write_victory(nb1, nb2);
}

void check_stats(sprite_perso *perso)
{
    if (perso->life > MAX_LIFE)
    {
        perso->life = MAX_LIFE;
    }

    if (perso->special_bar > MAX_SPECIAL)
    {
        perso->special_bar = MAX_SPECIAL;
    }

    if (perso->life_guard <= 0)
    {
        perso->broken_guard = true;
        perso->chara_state = stun;
        perso->guard = false;
        perso->stun_time = BROKEN_GUARD_STUN;
    }
}

void compute_game(jeu *world)
{
    if (!world->game_over)
    {
        if (world->timer.timer == 0)
        {
            world->timer.startTime = SDL_GetTicks();
            world->timer.timer = 10;

            if (world->p1.life > world->p2.life)
            {
                save_victory(1);
                world->state = endgame;
                world->game_over = true;
                world->p1.chara_state = winner;
                world->p2.chara_state = looser;
                world->p2.y += 40;
            }
            if (world->p1.life < world->p2.life)
            {
                save_victory(2);
                world->state = endgame;
                world->game_over = true;
                world->p1.chara_state = looser;
                world->p2.chara_state = winner;
                world->p1.y += 40;
            }
        }
        else
        {

            if (world->p1.life <= 0)
            {
                world->timer.startTime = SDL_GetTicks();
                world->timer.timer = 10;
                save_victory(2);
                world->state = endgame;
                world->game_over = true;
                world->p1.chara_state = looser;
                world->p2.chara_state = winner;
                world->p1.y += 40;
            }

            if (world->p2.life <= 0)
            {
                world->timer.startTime = SDL_GetTicks();
                world->timer.timer = 10;
                save_victory(1);
                world->state = endgame;
                world->game_over = true;
                world->p1.chara_state = winner;
                world->p2.chara_state = looser;
                world->p2.y += 40;
            }
        }
    }
}

void move_fireball(sprite_perso *perso)
{
    if (perso->fireball.launched_fireball)
    {
        perso->fireball.x += perso->hits.special_attack->speed * perso->fireball.multiplicateur;
    }
    else
    {
        perso->fireball.x = perso->mirror ? perso->x : perso->x + perso->w;
        perso->fireball.y = perso->y + perso->h / 2;
    }
}
void move_gravityball(sprite_perso *perso)
{
    if (perso->gravityball.launched_fireball)
    {
        if (perso->gravityball.multiplicateur == 1)
        {
            if (perso->gravityball.x >= 540)
            {
                perso->gravityball.x = 540;
            }
            else
            {
                perso->gravityball.x += perso->hits.special_attack2->speed;
            }
        }
        else
        {
            if (perso->gravityball.x <= 540)
            {
                perso->gravityball.x = 540;
            }
            else
            {
                perso->gravityball.x -= perso->hits.special_attack2->speed;
            }
        }
    }
    else
    {
        perso->gravityball.x = perso->mirror ? perso->x : perso->x + perso->w;
        perso->gravityball.y = perso->y;
    }
}

void check_fireball(sprite_perso *attacker, sprite_perso *receiver)
{
    if (!receiver->berserk)

    {
        if (attacker->fireball.x >= receiver->x && attacker->fireball.x <= receiver->x + receiver->w && attacker->fireball.launched_fireball && attacker->fireball.y >= receiver->y && attacker->fireball.y <= receiver->y + receiver->h)
        {
            if (receiver->guard)
            {
                receiver->life_guard -= attacker->hits.special_attack->dmg;
            }
            else
            {
                receiver->life -= attacker->hits.special_attack->dmg;
            }
            attacker->fireball.launched_fireball = false;
        }
        if (attacker->fireball.x > SCREEN_WIDTH || attacker->fireball.x < 0)
        {
            attacker->fireball.launched_fireball = false;
        }
    }
}

void check_gravityball(sprite_perso *attacker, sprite_perso *receiver)
{
    if (!receiver->berserk)
    {
        int distance = (sqrt(pow(receiver->x - attacker->gravityball.x, 2) + pow(receiver->y - attacker->gravityball.y, 2)));
        if (distance <= DISTANCE_MAX)
        {

            int coefficient = 100 * (DISTANCE_MAX - distance) / DISTANCE_MAX;

            if (attacker->gravityball.launched_fireball)
            {
                if ((SDL_GetTicks() - attacker->gravityball.timer_throw.startTime) / 1000 >= 10)
                {
                    attacker->gravityball.launched_fireball = false;
                }
                if (attacker->gravityball.x + 100 < receiver->x)
                {

                    receiver->x -= coefficient * BLACK_HOLE_STRENGH / 100;
                }
                if (attacker->gravityball.x + 100 >= receiver->x)
                {
                    receiver->x += coefficient * BLACK_HOLE_STRENGH / 100;
                }
                if (attacker->gravityball.y > receiver->y)
                {
                    receiver->y += coefficient * BLACK_HOLE_STRENGH / 100;
                }
                if (receiver->y > attacker->gravityball.y)
                {
                    receiver->y -= coefficient * BLACK_HOLE_STRENGH / 100;
                }

                if (receiver->x >= attacker->gravityball.x && receiver->x <= attacker->gravityball.x + 200 && attacker->fireball.y >= receiver->y && attacker->fireball.y <= receiver->y + receiver->h)
                {
                    if (receiver->guard)
                    {
                        receiver->life_guard -= attacker->hits.special_attack2->dmg;
                    }
                    else
                    {
                        receiver->life -= attacker->hits.special_attack2->dmg;
                    }
                }
            }
        }
    }
}

void check_bonus(sprite_perso *perso)
{
    if (perso->damage_bonus && !perso->dmg_bonus_timer.pause && (SDL_GetTicks() - perso->dmg_bonus_timer.startTime) / 1000 >= DMG_DURATION)
    {
        perso->damage_bonus = false;
        perso->dmg_bonus_timer.startTime = SDL_GetTicks();
        perso->berserk = false;
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
    move_fireball(&world->p1);
    move_fireball(&world->p2);
    move_gravityball(&world->p1);
    move_gravityball(&world->p2);
    check_fireball(&world->p1, &world->p2);
    check_fireball(&world->p2, &world->p1);
    check_gravityball(&world->p1, &world->p2);
    check_gravityball(&world->p2, &world->p1);
    lootbox_loop(world);
    check_bonus(&world->p1);
    check_bonus(&world->p2);
    movements(world, &world->p1, &world->p2);
    movements(world, &world->p2, &world->p1);
    sprites_collision(&world->p1, &world->p2, world);
    sprites_collision(&world->p2, &world->p1, world);
    change_directions(&world->p1, &world->p2);
    manage_aura(&world->p1);
    manage_aura(&world->p2);
    world->timestamp_w++;
    check_timer(world);
    check_stats(&world->p1);
    check_stats(&world->p2);
    compute_game(world);

    int height_factor, width_factor;
    height_factor = CELL_HEIGHT;
    width_factor = CELL_WIDTH;
}

void endgame_data(jeu *world)
{

    if ((SDL_GetTicks() - world->timer.startTime) / 1000 >= 1)
    {
        world->timer.timer--;
        world->timer.startTime = SDL_GetTicks();
    }
    if (world->timer.timer == 0)
    {
        world->state = main_menu;
    }

    if (world->p1.chara_state == winner)
    {
        world->p1.anim[winner].frame++;
        if (world->p1.anim[winner].frame == world->p1.anim[winner].nbFrame)
        {
            world->p1.anim[winner].frame = 5;
        }
        world->p2.anim[looser].frame++;
        if (world->p2.anim[looser].frame == world->p2.anim[looser].nbFrame)
        {
            world->p2.anim[looser].frame = world->p2.anim[looser].nbFrame - 1;
        }
    }

    else
    {
        world->p2.anim[winner].frame++;
        if (world->p2.anim[winner].frame == world->p2.anim[winner].nbFrame)
        {
            world->p2.anim[winner].frame = 5;
        }
        world->p1.anim[looser].frame++;
        if (world->p1.anim[looser].frame == world->p1.anim[looser].nbFrame)
        {
            world->p1.anim[looser].frame = world->p1.anim[looser].nbFrame - 1;
        }
    }
}