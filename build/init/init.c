#include "init.h"

/**
 * @brief Initialisation de la manette
 * 
 * @param world les données du monde du jeu
 */
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

/**
 * @brief Initialisation du timer
 * 
 * @param world les données du monde du jeu
 */
void init_timer(jeu *world)
{
    world->timer.startTime = SDL_GetTicks();
    world->timer.pause = false;
    world->timer.start = true;
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
    world->p1.gravityball.dmg_timer.pause = false;
    world->p1.gravityball.dmg_timer.start = true;
    world->p2.gravityball.dmg_timer.pause = false;
    world->p2.gravityball.dmg_timer.start = true;
    read_counter(world);
}

/**
 * @brief Initialise les maps du jeu
 *  
 * @param world les données du monde du jeu
 * @param renderer le moteur de rendu
 */
void init_map(jeu *world, SDL_Renderer *renderer)
{
    switch (world->choosed_map)
    {
    case city_night:
        world->map.image_fond = load_image("build/ressources/map/city_night/city.bmp", renderer);
        world->map.map_structure = read_file_map("build/ressources/map/city_night/city_night_structure.bin");
        world->map.plateformes = load_image("build/ressources/map/city_night/plateforme.bmp", renderer);
        break;

    case forest:
        world->map.image_fond = load_image("build/ressources/map/forest/forest.bmp", renderer);
        world->map.map_structure = read_file_map("build/ressources/map/forest/forest_structure.bin");
        world->map.plateformes = load_image("build/ressources/map/forest/plateforme.bmp", renderer);
        break;

    case japan:
        world->map.image_fond = load_image("build/ressources/map/Japan/Japan.bmp", renderer);
        world->map.map_structure = read_file_map("build/ressources/map/Japan/Japan_structure.bin");
        world->map.plateformes = load_image("build/ressources/map/Japan/plateforme.bmp", renderer);
        break;
    }
}

// fonction pour les miniatures de maps
void init_miniature(jeu *world, SDL_Renderer *renderer)
{
    world->menu_set.tab_map = malloc(sizeof(SDL_Texture*)*NB_MAPS);
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
    world->music.gameMusicPlayed = false;
}

void init_jeu(jeu *world, SDL_Renderer *renderer)
{
    world->menu_set.menu = load_image("build/ressources/menu/menu.bmp", renderer);
    world->menu_set.options_menu = load_image("build/ressources/menu/options.bmp", renderer);
    world->menu_set.selection_maps_menu = load_image("build/ressources/menu/Maps_selector.bmp", renderer);
    world->menu_set.menu_fond = world->menu_set.menu;
    world->state = main_menu;
    world->terminer = false;
    world->menu_set.index_menu = 0;
    world->font.police_compteur = TTF_OpenFont("build/ressources/Polices/A Love of Thunder.ttf", 30);
    world->font.police_victoire = TTF_OpenFont("build/ressources/Polices/A Love of Thunder.ttf", 80);
    init_music(world);
    init_miniature(world, renderer);
    world->menu_set.cadreVie = load_image("./build/ressources/cadres.bmp", renderer);
    world->p1.audiochan.action = 2;
    world->p1.audiochan.aura = 3;
    world->p1.audiochan.chara_voice = 4;
    world->p2.audiochan.action = 5;
    world->p2.audiochan.aura = 6;
    world->p2.audiochan.chara_voice = 7;
    world->gravityball_audio_chan = 8;
    world->game_over = false;
    world->keystates_pre = malloc(sizeof(int)*123);
}

void init_perso(SDL_Renderer *renderer, sprite_perso *perso, int x, int y, int w, int h, int speed, bool mirror, int choice)
{
    perso->perso_choisi = choice;
    perso->x = x;
    perso->y = y;
    perso->w = w;
    perso->h = h;
    perso->speed = CHARA_SPEED;
    perso->air_speed = AIR_CHARA_SPEED;
    perso->jump_height = 300;
    perso->chara_state = idle;
    perso->backwards = false;
    perso->jump_origin = y;
    perso->permibility = false;
    perso->mirror = mirror;
    perso->life = MAX_LIFE;
    perso->attack_launched = false;
    perso->guard = false;
    perso->life_guard = MAX_GUARD;
    perso->damage_bonus = false;
    perso->damage_bonus = false;
    perso->tab_combo = malloc(sizeof(combo)*NB_COMBOS);
    for(int i = 0;i<NB_COMBOS;i++){
        perso->tab_combo[i].input = malloc(sizeof(enum combos_inputs)*MAX_SIZE_COMBO);
    }
    init_hits(perso);
    init_combos(perso);
    perso->pos_tab_combo = 0;
    perso->special_bar = 300;
    perso->fireball.fireball = load_image("./build/ressources/Characters/Powers/fireball.bmp", renderer);
    perso->fireball.launched_fireball = false;
    perso->gravityball.launched_fireball = false;
    perso->gravityball.ending = false;
    perso->buffer = malloc(sizeof(inputs) * BUFFER_SIZE);
    perso->double_jump = true;
    perso->berserk = false;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        perso->buffer[i].input = 0;
        perso->buffer[i].timestamp = 0;
        perso->pos_tab_combo = 0;
    }
    perso->anim = malloc(sizeof(anim)*NB_ANIM);
    perso->broken_guard = false;
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
    lootbox->texture = malloc(sizeof(SDL_Texture*)*3);
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
        Mix_AllocateChannels(9);   
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
        init_state_animation(renderer, perso, winner, "build/ressources/Characters/Chara1/Victory.bmp", 30, 100);
        init_state_animation(renderer, perso, looser, "build/ressources/Characters/Chara1/Defeat.bmp", 28, 90);
        init_state_animation(renderer, perso, 19, "build/ressources/Characters/Powers/Aura_Spritesheet.Bmp", 8, 190);
        init_state_animation(renderer, perso, 20, "build/ressources/Characters/Powers/Transparent_Aura_Spritesheet.Bmp", 8, 190);
        init_state_animation(renderer, perso, 21, "build/ressources/Characters/Powers/gravityBall.bmp", 4, 199);
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
       init_state_animation(renderer, perso, 21, "build/ressources/Characters/Powers/gravityBall.bmp", 4, 199);
        perso->anim[19].aura = 0;
        break;
    }
}