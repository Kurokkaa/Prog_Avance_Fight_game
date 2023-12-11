#include "hits.h"

/**
 * @brief gere les animations des coups, et leur lancement
 *  
 * @param character_state etat du personnage
 * @param counter vitesse de l'animation
 * @param attacker qui attaque
 * @param receiver qui se fait attaquer
 * @param world Le monde
 */
void hits_anim(int character_state, int counter, sprite_perso * attacker, sprite_perso * receiver, jeu * world){
    //si il bloque ou non
    if (getAnimation(attacker->anim, character_state)->counter == counter)
    {
        setFrame(attacker->anim, character_state, getAnimation(attacker->anim, character_state)->frame + 1);
        if (getAnimation(attacker->anim, character_state)->frame == attacker->hits.light_punch->effective_frame)
        {
            //selon le type de coups
            switch (character_state)
            {
            case lpunch:
                light_punch(attacker, receiver, world);
                break;

            case hpunch:
                heavy_punch(attacker, receiver, world);
                break;

            case kickstate:
                kick_hit(attacker, receiver, world);
                break;
            }
        }
        if (getAnimation(attacker->anim, character_state)->frame >= getAnimation(attacker->anim, character_state)->nbFrame)
        {
            attacker->chara_state = idle;
            setFrame(attacker->anim, character_state, 0);
            attacker->attack_launched = false;
        }
        setCounter(attacker->anim, character_state, 0);
    }
    else
    {
        setCounter(attacker->anim, character_state, getAnimation(attacker->anim, character_state)->counter + 1);
    }
}

/**
 * @brief gère les hitboxs des coups de poings leger, le son 
 *  
 * @param attacker celui qui donne le coup
 * @param receiver celui qui reçoit les coups
 * @param world Le monde
 */
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
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * BERSERK_DMG : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;

                }
            }
            else
            {
                receiver->special_bar += 10;
                attacker->special_bar += 5;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * BERSERK_DMG : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;

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
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * BERSERK_DMG : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;
                }
            }
            else
            {
                receiver->special_bar += 10;
                attacker->special_bar += 5;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.light_punch->dmg * BERSERK_DMG : attacker->hits.light_punch->dmg * 1.5) : attacker->hits.light_punch->dmg;
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

/**
 * @brief gère les coups de poings lourds (hitbox/son...)
 *  
 * @param attacker celui qui donne le coup
 * @param receiver celui qui reçoit les coups
 * @param world Le monde
 */
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
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * BERSERK_DMG : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                attacker->special_bar += 10;
                if (!receiver->berserk)
                {
                    Mix_PlayChannel(receiver->audiochan.chara_voice, world->music.grunt, 0);
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * BERSERK_DMG : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
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
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * BERSERK_DMG : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                attacker->special_bar += 10;
                if (!receiver->berserk)
                {
                    receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.heavy_punch->dmg * BERSERK_DMG : attacker->hits.heavy_punch->dmg * 1.5) : attacker->hits.heavy_punch->dmg;
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

/**
 * @brief gère les coups de pieds (son etc..)
 *  
 * @param attacker celui qui donne le coup
 * @param receiver celui qui reçoit les coups
 * @param world Le monde
 */
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
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * BERSERK_DMG : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * BERSERK_DMG : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
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
                    receiver->life_guard -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * BERSERK_DMG : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
                }
            }
            else
            {
                receiver->special_bar += 20;
                receiver->life -= attacker->damage_bonus ? (attacker->berserk ? attacker->hits.kick->dmg * BERSERK_DMG : attacker->hits.kick->dmg * 1.5) : attacker->hits.kick->dmg;
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

/**
 * @brief gère les boules de feux 
 *  
 * @param attacker celui qui donne le coup
 * @param receiver celui qui reçoit les coups
 * @param world Le monde
 */
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

/**
 * @brief gère les trous noirs
 *  
 * @param attacker celui qui donne le coup
 * @param receiver celui qui reçoit les coups
 * @param world Le monde
 */
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

        if(getAnimation(attacker->anim, 21)->counter>=5){
            setFrame(attacker->anim, 21,getAnimation(attacker->anim, 21)->frame + 1 );
            setCounter(attacker->anim, 21, 0);
            if(getAnimation(attacker->anim, 21)->frame == getAnimation(attacker->anim, 21)->nbFrame){
               setFrame(attacker->anim,21, 0);
               Mix_FadeOutChannel(world->gravityball_audio_chan, 10);
               Mix_Resume(0);
               attacker->gravityball.ending = false;
               attacker->gravityball.launched_fireball = false;
            }
        }
        else{
            setCounter(attacker->anim, 21, getAnimation(attacker->anim, 21)->counter +1 );
        }
        
    }
    
}

/**
 * @brief gère le déplacement de la boule de feu
 *  
 * @param perso données relative aux personnages
 */
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

/**
 * @brief gère le déplacement du trou noir
 *  
 * @param perso gère les données relatives aux personnages
 */
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
