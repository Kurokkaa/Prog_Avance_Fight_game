#include "data.h"
#include "../constante.h"
#include "../init/init.h"
#include "../controller/controller.h"
#include "../lootbox/lootbox.h"



char **read_file_map(char *nom)
{
    FILE *file = fopen(nom, "r");
    char **map_struct = malloc(sizeof(char *) * 40);
    for (int i = 0; i < 40; i++)
    {
        map_struct[i] = malloc(sizeof(char) * 21);
        fread(map_struct[i], 21, 1, file);
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

bool read_combo(sprite_perso *player, int val, jeu * world)
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
                Mix_PlayChannel(player->audiochan.chara_voice, world->music.berserk_scream, 0);
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
    FILE *file = fopen("build/counter.bin", "wb");
    fwrite(select, strlen(select), 1, file);
    fclose(file);
}

void read_counter(jeu *world)
{
    char *File_name = "build/counter.bin";
    struct stat stat_file;
    FILE *file = fopen(File_name, "rb"); // ouverture en mode lecture
    stat(File_name, &stat_file);
    int sz = stat_file.st_size;
    char text_file[sz + 1];
    fread(text_file, 1, sz, file);
    text_file[sz] = '\0';
    world->timer.timer = atoi(text_file);
    if(world->timer.timer == 999){
        world->timer.inf = true;
    }
    else{
        world->timer.inf = false;
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
                if (!equals(x + perso->speed, y + perso->h, world->map.map_structure, '0')&& !equals(x + perso->speed + perso->w, y + perso->h, world->map.map_structure, '0'))
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
            if (equals(x - perso->air_speed, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x -= perso->air_speed;
            }
        }
        else
        {
            if (equals(x + perso->air_speed, y + perso->h, world->map.map_structure, ' ') && equals(x + perso->air_speed+perso->w, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x += perso->air_speed;
            }
        }
        perso->chara_state = flight;
    }

    // Contrôle en chute
    if (perso->chara_state == fall_control)
    {
        if (perso->backwards)
        {
            if (equals(x - perso->air_speed, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x -= perso->air_speed;
            }
        }
        else
        {
            if (equals(x + perso->air_speed, y + perso->h, world->map.map_structure, ' ') && equals(x + perso->air_speed+perso->w, y + perso->h, world->map.map_structure, ' '))
            {
                perso->x += perso->air_speed;
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
                perso->y -= HEIGHT_PER_FRAME;
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
            if (equals(x, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ') && equals(x + perso->w, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' '))
            {
                perso->y += HEIGHT_PER_FRAME;
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

    if (perso->chara_state != flight && equals(x, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ') && equals(x + perso->w, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' '))
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

            if (perso->anim[stun].counter == 5)
            {
                perso->anim[stun].frame++;
                perso->anim[stun].counter = 0;
                if (perso->anim[stun].frame == perso->anim[stun].nbFrame)
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
                //printf("%d",perso->life_guard);
                perso->life_guard = MAX_GUARD;
                //printf("%d",perso->life_guard);

                
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
                    Mix_PlayChannel(perso->audiochan.action, world->music.fireball_sound, 0);
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
           
            perso->anim[gravityball].counter = 0;
            perso->anim[gravityball].frame++;
            if (perso->anim[gravityball].frame == perso->anim[gravityball].nbFrame)
            {
                perso->gravityball.launched_fireball = true;
                perso->chara_state = idle;
                Mix_PlayChannel(world->gravityball_audio_chan, world->music.gravityball_sound, 0);
                Mix_Pause(0);
                perso->gravityball.timer_throw.startTime = SDL_GetTicks();
            }
        }
        else{
            perso->anim[gravityball].counter++;
        }
    }
}

void manage_aura(sprite_perso *perso, jeu * world)
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
                Mix_Pause(perso->audiochan.aura);
            }
        }

        if (!perso->anim[19].aura && perso->damage_bonus)
        {
            perso->anim[19].frame++;
            perso->anim[20].frame++;
            if (perso->anim[19].frame == 2)
            {
                perso->anim[19].aura = 1;
                Mix_PlayChannel(perso->audiochan.aura, world->music.aura, 0);
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

/*FONCTIONS DE LIBERATION ESPACE*/
void free_hits(jeu* world)
{
    for(int i = 0;i<NB_COMBOS;i++){
        free(world->p1.tab_combo[i].input);
    }
    free(world->p1.tab_combo);
    free(world->p1.buffer);
    free(world->p1.hits.light_punch);
    free(world->p1.hits.heavy_punch);
    free(world->p1.hits.kick);
    free(world->p1.hits.special_attack);
    free(world->p1.hits.special_attack2);
    
    for(int i = 0;i<NB_COMBOS;i++){
        free(world->p2.tab_combo[i].input);
    }
    free(world->p2.tab_combo);
    free(world->p2.buffer);
    free(world->p2.hits.light_punch);
    free(world->p2.hits.heavy_punch);
    free(world->p2.hits.kick);
    free(world->p2.hits.special_attack);
    free(world->p2.hits.special_attack2);
    free(world->keystates_pre);
}

void TTF_free_police(jeu* world){
    free(world->font.police_compteur);
    free(world->font.police_victoire);
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


void quit_game(jeu *world, SDL_Window **fenetre, SDL_Renderer **renderer)
{
    free_hits(world);
    TTF_free_police(world);
    free_menu(world);
    destroy_textures(world);
    free_map_structure(world->map.map_structure);
    free_music(world);
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

void free_menu(jeu* world){
    SDL_DestroyTexture(world->menu_set.cadreVie);
    SDL_DestroyTexture(world->menu_set.menu);
    SDL_DestroyTexture(world->menu_set.selection_maps_menu);
    SDL_DestroyTexture(world->menu_set.options_menu);
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
    for(int i = 0 ; i<NB_ANIM ; i++){
        SDL_DestroyTexture(perso->anim[i].anim_text);
    }
    free(perso->anim);
    SDL_DestroyTexture(perso->fireball.fireball);
}

void destroy_map(jeu *world)
{
    for(int i = 0 ; i<NB_MAPS ; i++){
        SDL_DestroyTexture(world->menu_set.tab_map[i]);
    }
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

void pauseChrono(compteur *chrono)
{
    if (chrono->start)
    {
        chrono->pause = true;
        chrono->pauseTime = SDL_GetTicks() - chrono->startTime;
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
    FILE *file = fopen("build/victory.bin", "wb");
    sprintf(new_score, "p1:%d\np2:%dn", nb1, nb2);
    fwrite(new_score, 7 + numDigits(nb1) + numDigits(nb2), 1, file);
    fclose(file);
}

void save_victory(int player)
{
    char *File_name = "build/victory.bin";
    struct stat stat_file;
    FILE *file = fopen(File_name, "rb"); // ouverture en mode lecture
    stat(File_name, &stat_file);
    int sz = stat_file.st_size;
    char text_file[sz + 1];
    fread(text_file, 1, sz, file);
    text_file[sz] = '\0';

    int line = 1;
    int size;
    int nb1 = 0;
    int nb2 = 0;
    int i;
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

    if (perso->life_guard <= 0 && !perso->broken_guard)
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
            world->timer.timer = ENDGAME_TIMER;

            if (world->p1.life > world->p2.life)
            {
                save_victory(1);
                Mix_PlayChannel(world->p1.audiochan.chara_voice, world->music.win_laugh, 0);
                world->state = endgame;
                world->game_over = true;
                world->p1.chara_state = winner;
                world->p2.chara_state = looser;
                world->p2.y += 40;
            }
            if (world->p1.life < world->p2.life)
            {
                save_victory(2);
                Mix_PlayChannel(world->p2.audiochan.chara_voice, world->music.win_laugh, 0);
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
                world->timer.timer = ENDGAME_TIMER;
                save_victory(2);
                Mix_PlayChannel(world->p2.audiochan.chara_voice, world->music.win_laugh, 0);
                world->state = endgame;
                world->game_over = true;
                world->p1.chara_state = looser;
                world->p2.chara_state = winner;
                world->p1.y += 40;
            }

            if (world->p2.life <= 0)
            {
                world->timer.startTime = SDL_GetTicks();
                world->timer.timer = ENDGAME_TIMER;
                save_victory(1);
                Mix_PlayChannel(world->p1.audiochan.chara_voice, world->music.win_laugh, 0);
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

void check_fireball(sprite_perso *attacker, sprite_perso *receiver, jeu * world)
{
    if (attacker->fireball.x >= receiver->x && attacker->fireball.x <= receiver->x + receiver->w && attacker->fireball.launched_fireball && attacker->fireball.y >= receiver->y && attacker->fireball.y <= receiver->y + receiver->h)
        {
            Mix_PlayChannel(-1, world->music.fireball_explosion, 0);
        }
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
                Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
            }
            attacker->fireball.launched_fireball = false;
        }
        if (attacker->fireball.x > SCREEN_WIDTH || attacker->fireball.x < 0)
        {
            attacker->fireball.launched_fireball = false;
        }
    }
}

void check_gravityball(sprite_perso *attacker, sprite_perso *receiver, jeu * world)
{
    if ((attacker->gravityball.launched_fireball && !attacker->gravityball.ending))
    {
        int distance = (sqrt(pow(receiver->x - attacker->gravityball.x, 2) + pow(receiver->y - attacker->gravityball.y, 2)));
        if (distance <= DISTANCE_MAX && !receiver->berserk)
        {
            int coefficient = 100 * (DISTANCE_MAX - distance) / DISTANCE_MAX;

         
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
              
                if(!attacker->gravityball.dmg_timer.pause && (SDL_GetTicks()-attacker->gravityball.dmg_timer.startTime)/1000>=0.5)
                {
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
                    attacker->gravityball.dmg_timer.startTime = SDL_GetTicks();
                }
        }
         if ((SDL_GetTicks() - attacker->gravityball.timer_throw.startTime) / 1000 >= 10)
            {
                attacker->gravityball.ending = true;
            }
    }
    else if(attacker->gravityball.ending){

        if(attacker->anim[21].counter>=5){
            attacker->anim[21].frame++;
            attacker->anim[21].counter = 0;
            if(attacker->anim[21].frame == attacker->anim[21].nbFrame){
               attacker->anim[21].frame = 0;
               Mix_FadeOutChannel(world->gravityball_audio_chan, 10);
               Mix_Resume(0);
               attacker->gravityball.ending = false;
               attacker->gravityball.launched_fireball = false;
            }
        }
        else{
            attacker->anim[21].counter++;
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
    check_fireball(&world->p1, &world->p2, world);
    check_fireball(&world->p2, &world->p1, world);
    check_gravityball(&world->p1, &world->p2, world);
    check_gravityball(&world->p2, &world->p1, world);
    lootbox_loop(world);
    check_bonus(&world->p1);
    check_bonus(&world->p2);
    movements(world, &world->p1, &world->p2);
    movements(world, &world->p2, &world->p1);
    sprites_collision(&world->p1, &world->p2, world);
    sprites_collision(&world->p2, &world->p1, world);
    change_directions(&world->p1, &world->p2);
    manage_aura(&world->p1, world);
    manage_aura(&world->p2, world);
    world->timestamp_w++;
    check_timer(world);
    check_stats(&world->p1);
    check_stats(&world->p2);
    compute_game(world);
}

void endgame_data(jeu *world)
{
    Mix_Pause(world->p1.audiochan.aura);
    Mix_Pause(world->p2.audiochan.aura);
    Mix_Pause(world->gravityball_audio_chan);
    if(Mix_Paused(0)){
        Mix_Resume(0);
    }
    if ((SDL_GetTicks() - world->timer.startTime) / 1000 >= 1)
    {
        world->timer.timer--;
        world->timer.startTime = SDL_GetTicks();
    }
    if (world->timer.timer == 0)
    {
        world->music.gameMusicPlayed = false;
        world->state = main_menu;
        world->menu_set.menu_fond = world->menu_set.menu;
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