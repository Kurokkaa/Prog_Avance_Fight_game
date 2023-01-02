#include "movements.h"



void movements(jeu *world, sprite_perso *perso, sprite_perso *adversaire)
{
    int x, y;
    x = perso->x;
    y = perso->y;

    reset_state(perso, perso->chara_state);
    if (perso->chara_state == idle)
    {
        if((equals(x + perso->speed, y + perso->h+HEIGHT_PER_FRAME, world->map.map_structure, '0')&& equals(x + perso->speed + perso->w, y+HEIGHT_PER_FRAME + perso->h, world->map.map_structure, '0'))){
            perso->permibility = false;
        }

        else if(perso->permibility){
            perso->chara_state = fall;
        }

        if (getAnimation(perso->anim, idle)->counter < 3)
        {
            setCounter(perso->anim, idle, getAnimation(perso->anim, idle)->counter + 1);
        }
        else
        {
            setCounter(perso->anim, idle, 0);
            setFrame(perso->anim, idle, getAnimation(perso->anim, idle)->frame + 1);
        }

        if (getAnimation(perso->anim, idle)->frame == getAnimation(perso->anim, idle)->nbFrame)
        {
            setFrame(perso->anim, idle, 0);
        }
    }

    // Contrôle en marche
    if (perso->chara_state == walk)
    {
        if((equals(x + perso->speed, y + perso->h, world->map.map_structure, '0')&& equals(x + perso->speed + perso->w, y + perso->h, world->map.map_structure, '0'))){
            perso->permibility = false;
        }
        else if(perso->permibility){
            perso->chara_state = fall;
        }

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
            if (getAnimation(perso->anim, walk)->counter < 3)
            {
                setCounter(perso->anim, walk, getAnimation(perso->anim, walk)->counter + 1);
            }
            else
            {
                setCounter(perso->anim, walk, 0);
                setFrame(perso->anim, walk, getAnimation(perso->anim, walk)->frame + 1 );
            }

            if (getAnimation(perso->anim, walk)->frame == getAnimation(perso->anim, walk)->nbFrame)
            {
                setFrame(perso->anim, walk, 0);
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
        if (getAnimation(perso->anim, jump)->frame == getAnimation(perso->anim, jump)->nbFrame)
        {
            perso->jump_origin = perso->y;
            perso->chara_state = flight;
        }
        else
        {
            setFrame(perso->anim, jump, getAnimation(perso->anim, jump)->frame +1 );
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

        if (getAnimation(perso->anim, knockback)->counter == 2)
        {
            setFrame(perso->anim, knockback, getAnimation(perso->anim, knockback)->frame + 1 );
            if (getAnimation(perso->anim, knockback)->frame >= getAnimation(perso->anim, knockback)->nbFrame)
            {
                perso->chara_state = idle;
                setFrame(perso->anim, knockback, 0);
            }
            setCounter(perso->anim, knockback, 0);
        }

        else
        {
            setCounter(perso->anim, knockback, getAnimation(perso->anim, knockback)->counter + 1);
        }
    }

    // Automatisation de la chute
    if ((perso->chara_state == fall))
    {
        if((equals(x + perso->speed, y + perso->h+HEIGHT_PER_FRAME, world->map.map_structure, '0')&& equals(x + perso->speed + perso->w, y + perso->h+HEIGHT_PER_FRAME, world->map.map_structure, '0'))){
            perso->permibility = false;
            perso->chara_state = idle;
          
        }
        x = perso->x;
        y = perso->y;
        if (y > 465)
        { // replace correctement perso sur le sol
            perso->y = 465;
           
            perso->chara_state = landing;
            perso->permibility = false;
        }
        else
        {
            if (equals(x, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ') && equals(x + perso->w, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ')||perso->permibility)
            {
                perso->y += HEIGHT_PER_FRAME;
               
            }
            else
            {
                perso->chara_state = landing;
                perso->permibility = false;
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
        if (getAnimation(perso->anim, landing)->counter == 3)
        {
            setFrame(perso->anim, landing, getAnimation(perso->anim, landing)->frame + 1);

            if (getAnimation(perso->anim, landing)->frame >= getAnimation(perso->anim, landing)->nbFrame)
            {
                perso->chara_state = idle;
                perso->double_jump = true;
                setFrame(perso->anim, landing, 0);
            }
            setCounter(perso->anim, landing, 0);
        }
        else
        {
            setCounter(perso->anim, landing, getAnimation(perso->anim, landing)->counter + 1);
        }
    }

    if(perso->chara_state == lpunch){
        hits_anim(lpunch, 2, perso, adversaire, world);
    }

    if(perso->chara_state == hpunch){
        hits_anim(hpunch, 3, perso, adversaire, world);
    }

    if(perso->chara_state == kickstate){
        hits_anim(kickstate, 2, perso, adversaire, world); 
    }

    /*stun */
    if (perso->chara_state == stun)
    {
        if (perso->stun_time >= 1)
        {
            perso->stun_time--;

            if (getAnimation(perso->anim, stun)->counter == 5)
            {
                setFrame(perso->anim, stun,getAnimation(perso->anim, stun)->frame + 1 );
                setCounter(perso->anim, stun, 0);
                if (getAnimation(perso->anim, stun)->frame == getAnimation(perso->anim, stun)->nbFrame)
                {
                    setFrame(perso->anim, stun, 4);
                }
            }
            else
            {
                setCounter(perso->anim, stun, getAnimation(perso->anim, stun)->counter + 1);
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
            if(getAnimation(perso->anim, fireball)->counter >= 1){
                setFrame(perso->anim, fireball, getAnimation(perso->anim, fireball)->frame + 1);
                setCounter(perso->anim, fireball, 0);
                if (getAnimation(perso->anim, fireball)->frame == getAnimation(perso->anim, fireball)->nbFrame)
                {
                    perso->fireball.launched_fireball = true;
                    Mix_PlayChannel(perso->audiochan.action, world->music.fireball_sound, 0);
                    perso->chara_state = idle;
                }
            }
            else{
                setCounter(perso->anim, fireball, getAnimation(perso->anim, fireball)->counter + 1);
            }
        }

    if (perso->chara_state == gravityball)
    {   
        if(getAnimation(perso->anim, gravityball)->counter >= 1){
           
           setCounter(perso->anim, gravityball, 0);
           setFrame(perso->anim, gravityball, getAnimation(perso->anim, gravityball)->frame + 1);
            if (getAnimation(perso->anim, gravityball)->frame == getAnimation(perso->anim, gravityball)->nbFrame)
            {
                perso->gravityball.launched_fireball = true;
                perso->chara_state = idle;
                Mix_PlayChannel(world->gravityball_audio_chan, world->music.gravityball_sound, 0);
                Mix_Pause(0);
                perso->gravityball.timer_throw.startTime = SDL_GetTicks();
            }
        }
        else{
            setCounter(perso->anim, gravityball, getAnimation(perso->anim, gravityball));
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