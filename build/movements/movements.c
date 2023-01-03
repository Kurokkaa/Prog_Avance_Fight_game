#include "movements.h"


/**
 * @brief Gestion des mouvements du personnage
 * 
 * @param world les données du monde du jeu
 * @param perso le personnage pour lequel on traite les données
 * @param adversaire l'adversaire du personnage dont on traite les données
 */
void movements(jeu *world, sprite_perso *perso, sprite_perso *adversaire)
{
    int x, y;
    x = perso->x;
    y = perso->y;

    reset_state(perso, perso->chara_state); //reset les frames 

    /*Cas où personnage est idle*/
    if (perso->chara_state == idle)
    {
        if((equals(x + perso->speed, y + perso->h+HEIGHT_PER_FRAME, world->map.map_structure, '0')&& equals(x + perso->speed + perso->w, y+HEIGHT_PER_FRAME + perso->h, world->map.map_structure, '0'))){
            perso->permibility = false; //annule la perméabilité du personnage (ne peut pas passer à travers les plateformes)
        }

        else if(perso->permibility){
            perso->chara_state = fall; //si perso est perméable, il peut passer à travers une plateforme
        }

        
        if (getAnimation(perso->anim, idle)->counter < 3) //gestion de l'animation de marche
        {
            setCounter(perso->anim, idle, getAnimation(perso->anim, idle)->counter + 1); //compteur permet de réguler vitesse de défilement des frames de l'anim
        }
        else
        {
            setCounter(perso->anim, idle, 0); //si counter supérieur à valeur de la condition, on le remet à zéro et augmente la frame
            setFrame(perso->anim, idle, getAnimation(perso->anim, idle)->frame + 1);
        }

        if (getAnimation(perso->anim, idle)->frame == getAnimation(perso->anim, idle)->nbFrame) //si l'animation est à la fin, on la remet à zéro
        {
            setFrame(perso->anim, idle, 0);
        }
    }

    // Contrôle en marche
    if (perso->chara_state == walk)
    {
        if((equals(x + perso->speed, y + perso->h, world->map.map_structure, '0')&& equals(x + perso->speed + perso->w, y + perso->h, world->map.map_structure, '0'))){
            perso->permibility = false; //annule la perméabilité du personnage (ne peut pas passer à travers les plateformes)
        }
        else if(perso->permibility){
            perso->chara_state = fall; //si perso est perméable, il peut passer à travers une plateforme
        }

        if (canMove(perso, adversaire)) //si le perso peut bouger car il n'entre pas en collision avec son adversaire
        {
            if (perso->backwards) //si perso va en arrière
            {
                if (!equals(x - perso->speed, y + perso->h, world->map.map_structure, '0')) //on teste s'il n'est pas bloqué par une limite
                {
                    perso->x -= perso->speed; //on le décale vers la gauche
                }
            }
            else
            {
                if (!equals(x + perso->speed, y + perso->h, world->map.map_structure, '0')&& !equals(x + perso->speed + perso->w, y + perso->h, world->map.map_structure, '0')) //on teste s'il n'est pas bloqué par une limite
                {
                    perso->x += perso->speed; //on le décale vers la droite
                }
            }
            //Gestion des animations de walk
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
            if (equals(x - perso->air_speed, y + perso->h, world->map.map_structure, ' ')) //on test si le perso peut se déplacer à l'endroit voulu, si oui on le déplace
            {
                perso->x -= perso->air_speed;
            }
        }
        else
        {
            if (equals(x + perso->air_speed, y + perso->h, world->map.map_structure, ' ') && equals(x + perso->air_speed+perso->w, y + perso->h, world->map.map_structure, ' ')) //on test si le perso peut se déplacer à l'endroit voulu, si oui on le déplace
            {
                perso->x += perso->air_speed;
            }
        }
        perso->chara_state = flight; //on le repasse en vol pour décrémenter son y et le faire monter
    }

    // Contrôle en chute
    if (perso->chara_state == fall_control)
    {
        if (perso->backwards)
        {
            if (equals(x - perso->air_speed, y + perso->h, world->map.map_structure, ' ')) //on test si le perso peut se déplacer à l'endroit voulu, si oui on le déplace
            {
                perso->x -= perso->air_speed;
            }
        }
        else
        {
            if (equals(x + perso->air_speed, y + perso->h, world->map.map_structure, ' ') && equals(x + perso->air_speed+perso->w, y + perso->h, world->map.map_structure, ' ')) //on test si le perso peut se déplacer à l'endroit voulu, si oui on le déplace
            {
                perso->x += perso->air_speed;
            }
        }
        perso->chara_state = fall; //on le repasse en chute pour incrémenter son y et le faire descendre
    }

    // Initialisation du saut
    if (perso->chara_state == jump)
    {
        if (getAnimation(perso->anim, jump)->frame == getAnimation(perso->anim, jump)->nbFrame)
        {
            perso->jump_origin = perso->y; //stocke l'origine du saut du personnage
            perso->chara_state = flight; //passe le perso en state vol
        }
        else
        {
            setFrame(perso->anim, jump, getAnimation(perso->anim, jump)->frame +1 ); //joue l'animation
        }
    }

    // Contrôle de la hauteur du saut
    if (perso->chara_state == flight)
    {
        if (perso->y > perso->jump_origin - perso->jump_height) //si la hauteur du personnage est supérieur à l'origine de son saut - sa hauteur en y
        {
            if (!equals(x, y, world->map.map_structure, '0')) //il monte s'il ne se cogne pas à une structure 0 (limites de map)
            {
                perso->y -= HEIGHT_PER_FRAME;
            }
            else
            {
                perso->chara_state = fall; //sinon il chute
            }
        }
        else
        {
            perso->chara_state = fall; //si la hauteur du personnage est inférieur à l'origine de son saut - sa hauteur en y, il chute
        }
    }

    //Etat où les deux persos rentrent en collision
    if (perso->chara_state == knockback) 
    {
        if (!perso->mirror) //si perso n'est pas mirror
        {
            if (!equals(perso->x - perso->speed, perso->y + perso->h, world->map.map_structure, '0')) //on teste sa limite gauche, s'il peut bouger, on le bouge
            {
                perso->x -= perso->speed;
            }
            else
            {
                perso->chara_state = idle;
            }
        }

        else //si perso mirror
        {
            if (!equals(perso->x + perso->speed, perso->y + perso->h, world->map.map_structure, '0')) //on teste sa limite droite, s'il peut bouger, on le bouge
            {
                perso->x += perso->speed;
            }
            else
            {
                perso->chara_state = idle;
            }
        }

        //Animation du knockback
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
        if((equals(x + perso->speed, y + perso->h+HEIGHT_PER_FRAME, world->map.map_structure, '0') && equals(x + perso->speed + perso->w, y + perso->h+HEIGHT_PER_FRAME, world->map.map_structure, '0'))){ //On teste si le personnage entre en collision avec une limite
            perso->permibility = false; //si perso rencontre limite, on met sa perméabilité à false;
            perso->chara_state = idle; //si perso rencontre limite, on le repasse à idle
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
            if (equals(x, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ') && equals(x + perso->w, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ')||perso->permibility) //on teste si le perso est perméable et peut traverser une structure
            {
                perso->y += HEIGHT_PER_FRAME; //si oui il chute
               
            }
            else
            {
                perso->chara_state = landing; //sinon il atterit
                perso->permibility = false;
            }
            
        }
    }

    // Gestion de la gravité, le perso chute si pas de plateformes
    x = perso->x;
    y = perso->y;

    if (perso->chara_state != flight && equals(x, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ') && equals(x + perso->w, y + perso->h + HEIGHT_PER_FRAME, world->map.map_structure, ' ')) //si perso pas en flight et pas de structure pour le retenir, il chute
    {
        perso->chara_state = fall;
    }

    //Cas où le perso atterit
    if (perso->chara_state == landing)
    {
        //Joue les animations de l'aterissage
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

    //Gestion des coups
    if(perso->chara_state == lpunch){
        hits_anim(lpunch, 2, perso, adversaire, world); //gère les frames + activation de la fonction light_punch
    }

    if(perso->chara_state == hpunch){
        hits_anim(hpunch, 3, perso, adversaire, world); //gère les frames + activation de la fonction heavy_punch
    }

    if(perso->chara_state == kickstate){
        hits_anim(kickstate, 2, perso, adversaire, world); //gère les frames + activation de la fonction kick_hit
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
                perso->life_guard = MAX_GUARD;
            }
            perso->attack_launched = 0;
            perso->chara_state = idle;
        }
    }

/*THROWABLE*/
    if (perso->chara_state == fireball) //combo fireball
        {
            //getion des frames
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

    if (perso->chara_state == gravityball) //combo gravityball
    {   
        //gestion des frames
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
            setCounter(perso->anim, gravityball, getAnimation(perso->anim, gravityball)->counter +1);
        }
    }
}

/**
 * @brief Change la direction vers laquelle chaques persos regardent
 * 
 * @param p1 le perso 1
 * @param p2 le perso 2
 */
void change_directions(sprite_perso *p1, sprite_perso *p2)
{
    if (p1->x > p2->x)
    {
        p1->mirror = true; //passe le p1 en mirroir et p2 en non mirroir
        p2->mirror = false;
    }
    else
    {
        p1->mirror = false; //passe le p1 en non mirroir et p2 en mirroir
        p2->mirror = true;
    }
}

/**
 * @brief Vérifie si le perso peut bouger où s'il rentre en collision avec l'adversaire
 * 
 * @param perso Le perso qu'on teste
 * @param adversaire L'adversaire du perso testé
 * @return true si le perso peut bouger
 * @return false si le perso ne peut pas bouger
 */
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
    free(yAxis);
    return canMove;
}

/**
 * @brief Met les personnages en état de knockback s'ils se superposent
 * 
 * @param p1 le perso 1
 * @param p2 le perso 2
 * @param world les données du monde du jeu
 */
void sprites_collision(sprite_perso *p1, sprite_perso *p2, jeu *world)
{
    int tete = p1->y;
    int pieds = p1->y - p1->h;
    if (p1->chara_state != jump || p2->chara_state != jump || p1->chara_state != flight || p2->chara_state != flight || p1->chara_state != flight_control || p2->chara_state != flight_control) //permet de laisser les persos passer l'un à travers l'autre dans certains états
    {
        if (tete >= p2->y && pieds <= p2->y - p2->h) //test de la hauteur
        {
            if (!p1->mirror && p2->mirror) //Cas où p1 n'est pas mirror
            {
                if (p1->x + p1->w >= p2->x && p1->backwards != 1) //test de la superpostion
                {
                    if (!equals(p1->x + p1->speed, p1->y + p1->h, world->map.map_structure, '0')) //si p1 supperposé et qu'il peut se déplacer vers la gauche, il passe en knockback
                    {
                        p1->chara_state = knockback;
                    }
                    else
                    {
                        p1->chara_state = idle;
                    }
                }
            }

            else //Cas où p2 n'est pas mirror
            {
                if (p1->x <= p2->x + p2->w && p1->backwards == 1) // test de la superposition
                {
                    if (!equals(p1->x + p1->speed, p1->y + p1->h, world->map.map_structure, '0')) //si p1 supperposé et qu'il peut se déplacer vers la droite, il passe en knockback
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