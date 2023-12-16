#include "controller.h"

/**
 * MENU INPUTS
 */

/**
 * @brief Gestion des inputs dans les menus
 * 
 * @param event la liste des événemens
 * @param world le monde
 * @param renderer le renderer
 * 
 */
void handle_menu_inputs(SDL_Event *event, jeu *world, SDL_Renderer *renderer)
{
    
    
    if (!world->music.menuPlayed && !world->music.gameMusicPlayed)
        {
            Mix_PlayChannel(music, world->music.menu, -1);
            world->music.menuPlayed = true;
        }
    while (SDL_PollEvent(event))
    {
        
        if (event->type == SDL_QUIT)
        {
            world->terminer = true;
            
        }
        //Si une touche est appyue
        if (event->type == SDL_KEYDOWN)
        {
            //touche échap
            if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                //on enleve toutes les pause (timer, état,musique);
                if (world->state == pause)
                {
                    world->state = combat;
                    for(int channel = 0; channel < 9; channel++){
                        Mix_Resume(channel);
                    }
                    unpause(&world->timer);
                    unpause(&world->p1.chrono_guard);
                    unpause(&world->p2.chrono_guard);
                    unpause(&world->p1.chrono_special);
                    unpause(&world->p2.chrono_special);
                    unpause(&world->p1.dmg_bonus_timer);
                    unpause(&world->p2.dmg_bonus_timer);
                    unpause(&world->p1.gravityball.timer_throw);
                    unpause(&world->p2.gravityball.timer_throw);
                    unpause(&world->p1.gravityball.dmg_timer);
                    unpause(&world->p2.gravityball.dmg_timer);
                }
                //on revient au menu
                else if (world->state == options)
                {
                    world->state = main_menu;
                    world->menu_set.menu_fond = world->menu_set.menu; // le menu est affiché
                    world->menu_set.index_menu = 0; 
                }

                else if (world->state == selection_map)
                {
                    world->state = main_menu;
                    world->menu_set.menu_fond = world->menu_set.menu;
                    world->menu_set.index_menu = 0;
                }
            }

            if (event->key.keysym.sym == SDLK_z || event->key.keysym.sym == SDLK_UP)
            {
                //on passe à la séléction précédente du menu et on gère la limite
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

            if (event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_DOWN)
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

            if (event->key.keysym.sym == SDLK_d || event->key.keysym.sym == SDLK_RIGHT)
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
                    world->menu_set.index_menu == 5 ? world->menu_set.index_menu = 0 : world->menu_set.index_menu++;
                }
            }

            if (event->key.keysym.sym == SDLK_q || event->key.keysym.sym == SDLK_LEFT)
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
                    world->menu_set.index_menu == 0 ? world->menu_set.index_menu = 5 : world->menu_set.index_menu--;
                }
            }
            //Touche entrée
            if (event->key.keysym.sym == SDLK_RETURN)
            {
                //selon l'index l'action est différente
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
                    //on initialise la map selon celle choisie
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
                    
                    init_timer(world); //On initialise tous les chronomètre
                    world->game_over = false; // variable de fin de combat
                    world->state = combat;
                    world->timestamp_w = 0; //compteur de frame pour le buffer
                    world->music.menuPlayed = false;
                    Mix_PlayChannel(music, world->music.game_music, -1);
                    world->music.gameMusicPlayed = true;
                    /*initialisation du tableau des états de touche précédentes*/
                    for (int i = 0; i < 123; i++)
                    {
                        world->keystates_pre[i] = 0;  
                    }

                    init_lootbox(&world->lootbox, renderer);
                    world->menu_set.index_menu = 0;
                    world->in_game = true;
                }
                else if (world->state == options)
                {
                    //on paramètre le timer de jeu
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
                        save_counter("999");
                        break;
                    case 4:
                        set_IA(world, 1);
                        break;
                    case 5:
                        set_IA(world, 0);
                    }
                    world->menu_set.menu_fond = world->menu_set.menu;
                    world->state = main_menu;
                }
                else if (world->state == pause)
                {
                    switch (world->menu_set.index_menu)
                    {
                    case 0:
                        for(int channel = 0; channel < 9; channel++){
                            Mix_Resume(channel);
                        }
                        world->state = combat;
                        break;
                    case 1:
                        world->menu_set.menu_fond = world->menu_set.menu;
                        world->state = main_menu;
                        world->music.gameMusicPlayed = false;
                        break;
                    }
                }
                world->menu_set.index_menu = 0;
            }
           
        }
    }
}

/**
 * INPUTS GAMEPLAY COMBAT
 */
/**
 * @brief Gestion des inputs en combat
 * 
 * @param world le monde
 * @param renderer le renderer
 * @return void
 */
void gameplay_inputs(SDL_Event *event, jeu *world)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_JoystickUpdate();
    // checkJoystick(world->joysticks);
    /*variable qui indique si un combo à été trouvé*/
    bool combop1 = false;  
    bool combop2 = false;

    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            world->terminer = true;
        }
    }
    //si la touche échap est appuyé on met tout en pause
    if (keystates[SDL_SCANCODE_ESCAPE] && !world->keystates_pre[SDL_SCANCODE_ESCAPE])
    {
        world->state = pause;
        for(int channel = 0; channel < 9; channel++){
            Mix_Pause(channel);
        }

        pauseChrono(&world->timer);
        pauseChrono(&world->p1.chrono_guard);
        pauseChrono(&world->p2.chrono_guard);
        pauseChrono(&world->p1.dmg_bonus_timer);
        pauseChrono(&world->p2.dmg_bonus_timer);
        pauseChrono(&world->p1.gravityball.timer_throw);
        pauseChrono(&world->p2.gravityball.timer_throw);
        pauseChrono(&world->p1.gravityball.dmg_timer);
        pauseChrono(&world->p2.gravityball.dmg_timer);
    }
    // on bloque le joueur s'il est stun ou en guard ou s'il est déjà entrain d'attaquer
    if (world->p1.chara_state != stun && !world->p1.attack_launched && !world->p1.guard)
    {
         
        // aucune touche de déplacement appuyé
        if ((!keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D] && !keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D]) && (world->p1.chara_state == walk))
        {
            world->p1.chara_state = idle;
            world->p1.backwards = false;
        }
        // deplacement gauche
        if ((keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D]) /*|| SDL_GameControllerGetAxis(world->joysticks[0],0)<-(10000)*/)
        {
           //le perso doit reculer et change son état en fonction de l'etat actuelle 
            world->p1.backwards = true;
            if (world->p1.chara_state == idle)
            {
                world->p1.chara_state = walk;
                
            }
            if (world->p1.chara_state == flight)
            {
                world->p1.chara_state = flight_control;
                
            }
            if (world->p1.chara_state == fall)
            {
                world->p1.chara_state = fall_control;
            }
            //on ajoute l'input gauche au buffer si la touche n'a pas été appuyé sur la frame d'avant
            if (keystates[SDL_SCANCODE_A] != world->keystates_pre[SDL_SCANCODE_A])
            {
                add_input_buffer(&world->p1, left, world->timestamp_w);
            }
        }
        // deplacement droite
        if (!keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_D] /*|| SDL_GameControllerGetAxis(world->joysticks[0],0)>10000*/)
        {
            //le perso ne reculer pas et change son état en fonction de l'etat actuelle 
            world->p1.backwards = false;
            if (world->p1.chara_state == idle)
            {
                world->p1.chara_state = walk;
              
            }
            if (world->p1.chara_state == flight)
            {
                world->p1.chara_state = flight_control;
               
            }
            if (world->p1.chara_state == fall)
            {
                world->p1.chara_state = fall_control;
               
            }
            //on ajoute l'input droite au buffer si la touche n'a pas été appuyé sur la frame d'avant
            if (keystates[SDL_SCANCODE_D] != world->keystates_pre[SDL_SCANCODE_D])
            {
                add_input_buffer(&world->p1, right, world->timestamp_w);
            }
        }
        if (!keystates[SDL_SCANCODE_Z] && keystates[SDL_SCANCODE_S])
        {
            
            if (world->p1.chara_state == idle || world->p1.chara_state == walk)
            {
                world->p1.chara_state = idle;
                world->p1.permibility = true;
                
            }
            //si le personnage est en saut on annule le saut en le faisant tomber
            if (world->p1.chara_state == jump || world->p1.chara_state == flight || world->p1.chara_state == flight_control)
            {
                world->p1.chara_state = fall;
            }
            //on ajoute l'input bas au buffer si la touche n'a pas été appuyé sur la frame d'avant
            if (keystates[SDL_SCANCODE_S] != world->keystates_pre[SDL_SCANCODE_S])
            {
                add_input_buffer(&world->p1, down, world->timestamp_w);
            }
        }

        // sauts
        if ((keystates[SDL_SCANCODE_W] /*|| SDL_GameControllerGetAxis(world->joysticks[0],1)< (-10000)*/))
        {   
            //si le personnage est au sol, on le fait sauter
            if (world->p1.chara_state == idle || world->p1.chara_state == walk)
            {
                world->p1.chara_state = jump;
            }
            else
            {   
                //s'il est en l'air on lui donne un deuxieme saut disponible
                if ((world->p1.chara_state == jump || world->p1.chara_state == flight || world->p1.chara_state == flight_control || world->p1.chara_state == fall_control || world->p1.chara_state == fall)){
                    if (keystates[SDL_SCANCODE_W] != world->keystates_pre[SDL_SCANCODE_W] && world->p1.double_jump)
                    {

                        world->p1.jump_origin = world->p1.y;
                        setFrame(world->p1.anim, flight, 0);
                        setFrame(world->p1.anim, flight_control, 0);
                        world->p1.chara_state = flight;
                        world->p1.permibility = false;
                        world->p1.double_jump = false;
                    }
                }
                //on ajoute l'input haut si la touche n'etait pas appuyé
                if (keystates[SDL_SCANCODE_W] != world->keystates_pre[SDL_SCANCODE_W])
                {
                    add_input_buffer(&world->p1, up, world->timestamp_w);
                }
            }
        }

        // coups de poings legers
        if (keystates[SDL_SCANCODE_G] && !keystates[SDL_SCANCODE_Y] && !keystates[SDL_SCANCODE_J])
        {
            if (keystates[SDL_SCANCODE_G] != world->keystates_pre[SDL_SCANCODE_G])
            {
            
              
                //si le personnage est au sol
                if (world->p1.chara_state == idle || world->p1.chara_state == walk)
                {
                    //on ajoute i'input poings leger
                    add_input_buffer(&world->p1, light_p, world->timestamp_w);
                    //on continue tant qu'on à pas parcouru tous les combo où qu'on n'en a pas trouvé un
                    for (int i = 0; i < NB_COMBOS && !combop1; i++)
                    {
                        combop1 = read_combo(&world->p1, i, world);
                    }
                    //si aucun combo n'a été trouvé le coup de poing est lancé
                    if (!combop1)
                    {
                        world->p1.chara_state = lpunch;
                        //l'attaque est considéré comme lancée
                        world->p1.attack_launched = true;
                    }
                }
            }
        }

        // coups de poings lourds
        if (!keystates[SDL_SCANCODE_G] && keystates[SDL_SCANCODE_Y] && !keystates[SDL_SCANCODE_J])
        {
            if (keystates[SDL_SCANCODE_Y] != world->keystates_pre[SDL_SCANCODE_Y])
            {
                 //si le personnage est au sol
                if (world->p1.chara_state == idle || world->p1.chara_state == walk)
                {   
                    //on ajoute i'input poings lourds
                    add_input_buffer(&world->p1, heavy_p, world->timestamp_w);
                    //on continue tant qu'on à pas parcouru tous les combo où qu'on n'en a pas trouvé un
                    for (int i = 0; i < NB_COMBOS && !combop1; i++)
                    {
                        combop1 = read_combo(&world->p1, i, world);
                    }
                    //si aucun combo n'a été trouvé le coup de poing est lancée
                    if (!combop1)
                    {
                        world->p1.chara_state = hpunch;
                        //l'attaque est considéré comme lancée
                        world->p1.attack_launched = true;
                    }
                }
            }
        }
        if (!keystates[SDL_SCANCODE_Y] && !keystates[SDL_SCANCODE_G] && keystates[SDL_SCANCODE_J])
        {
            if (keystates[SDL_SCANCODE_J] != world->keystates_pre[SDL_SCANCODE_J])
            {
                //si le personnage est au sol
                if (world->p1.chara_state == idle || world->p1.chara_state == walk)
                {
                     //on ajoute i'input poings lourds
                    add_input_buffer(&world->p1, kick, world->timestamp_w);
                     //on continue tant qu'on à pas parcouru tous les combo où qu'on n'en a pas trouvé un
                    for (int i = 0; i < NB_COMBOS && !combop1; i++)
                    {
                        combop1 = read_combo(&world->p1, i, world);
                    }
                    //si aucun combo n'a été trouvé le coup de pied est lancée
                    if (!combop1)
                    {
                        world->p1.chara_state = kickstate;
                        //l'attaque est considéré comme lancée
                        world->p1.attack_launched = true;
                    }
                }
            }
        }
        //si h est appuyé et la garde n'est pas brisé
        if (keystates[SDL_SCANCODE_H] && (world->p1.chara_state == idle || world->p1.chara_state == walk) && !world->p1.broken_guard)
        {
            //on lance la garde
            world->p1.guard = true;
            world->p1.chara_state = idle;
        }
    }
    //si h n'est pas appuyé la garde est désactivé
    if (!keystates[SDL_SCANCODE_H] && (world->p1.chara_state == idle || world->p1.chara_state == walk))
    {
        world->p1.guard = false;
    }
    //On refait pareil pour le joueur 2
    if (world->p2.chara_state != stun && !world->p2.attack_launched && !world->p2.guard)
    {
        if ((!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT] && !keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]) && (world->p2.chara_state == walk))
        {
            if(world->ia != 1){
                world->p2.chara_state = idle;
            }
        }
        if (keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT])
        {
            // world->p1.speed = -CHARA_SPEED;
            if(world->ia != 1)
            {
                world->p2.backwards = true;
            
                if (world->p2.chara_state == idle)
            {
                    world->p2.chara_state = walk;
                            
            }
                if (world->p2.chara_state == flight)
            {
                    world->p2.chara_state = flight_control;
            }
            if (world->p2.chara_state == fall)
            {
                    world->p2.chara_state = fall_control;
                }
            }

            if (keystates[SDL_SCANCODE_LEFT] != world->keystates_pre[SDL_SCANCODE_LEFT])
            {
                if(world->ia != 1){
                    add_input_buffer(&world->p2, left, world->timestamp_w);
                }
            }
        }

        if (!keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT])
        {
            if(world->ia != 1){
                world->p2.backwards = false;
           
                if (world->p2.chara_state == idle)
                {
                    world->p2.chara_state = walk;
                
                }
                if (world->p2.chara_state == flight)
                {
                    world->p2.chara_state = flight_control;
                }
                if (world->p2.chara_state == fall)
                {
                world->p2.chara_state = fall_control;
                }
            }

            if (keystates[SDL_SCANCODE_RIGHT] != world->keystates_pre[SDL_SCANCODE_RIGHT])
            {
                if(world->ia != 1){
                    add_input_buffer(&world->p2, right, world->timestamp_w);
                }
            }
        }

        if (!keystates[SDL_SCANCODE_UP] && keystates[SDL_SCANCODE_DOWN])
        {
            if (world->p2.chara_state == idle || world->p2.chara_state == walk)
            {
                world->p2.permibility = true;
                if (keystates[SDL_SCANCODE_DOWN] != world->keystates_pre[SDL_SCANCODE_DOWN])
                {
                    if(world->ia != 1){
                        add_input_buffer(&world->p2, down, world->timestamp_w);
                    }
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
                        setFrame(world->p2.anim, flight, 0);
                        setFrame(world->p1.anim, flight_control, 0);
                        world->p2.chara_state = flight;
                        world->p2.permibility = false;  
                        world->p2.double_jump = false;
                    }
            }
            if (keystates[SDL_SCANCODE_UP] != world->keystates_pre[SDL_SCANCODE_UP])
            {
                if(world->ia != 1){
                    add_input_buffer(&world->p2, up, world->timestamp_w);
                }
            }
        }

        // coups
        if (keystates[SDL_SCANCODE_KP_4] && !keystates[SDL_SCANCODE_KP_8] && !keystates[SDL_SCANCODE_KP_9])
        {
            if (keystates[SDL_SCANCODE_KP_4] != world->keystates_pre[SDL_SCANCODE_KP_4])
            {

                if (world->p2.chara_state == idle || world->p2.chara_state == walk)
                {
                    if(world->ia != 1){
                        add_input_buffer(&world->p2, light_p, world->timestamp_w);
                        for (int i = 0; i < NB_COMBOS && !combop2; i++)
                        {
                            combop2 = read_combo(&world->p2, i, world);
                        }
                        if (!combop2)
                        {
                            world->p2.chara_state = lpunch;
                            world->p2.attack_launched = true;
                        }
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
                    if(world->ia!=1){
                    add_input_buffer(&world->p2, heavy_p, world->timestamp_w);
                    for (int i = 0; i < NB_COMBOS && !combop2; i++)
                    {
                        combop2 = read_combo(&world->p2, i, world);
                    }
                    if (!combop2)
                    {
                        world->p2.chara_state = hpunch;
                        world->p2.attack_launched = true;
                    }
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
                    if(world->ia != 1){
                    add_input_buffer(&world->p2, kick, world->timestamp_w);
                    for (int i = 0; i < NB_COMBOS && !combop2; i++)
                    {
                        combop2 = read_combo(&world->p2, i, world);
                    }
                    if (!combop2)
                    {
                        world->p2.chara_state = kickstate;
                        world->p2.attack_launched = true;
                    }
                    }
                }
            }
        }

        if (keystates[SDL_SCANCODE_KP_5] && !world->p2.broken_guard && (world->p2.chara_state == idle || world->p2.chara_state == walk))
        {
            if(world->ia != 1){
                world->p2.guard = true;
                world->p2.chara_state = idle;
            }
        }
    }
    if (!keystates[SDL_SCANCODE_KP_5] && (world->p2.chara_state == idle || world->p2.chara_state == walk))
    {
        if(world->ia != 1){
            world->p2.guard = false;
        }
    }
    //Les actions de l'ia si il n'y a qu'un joueur
    if(world->ia == 1)
    {
        if((world->p1.x+100 < world->p2.x && (world->p2.chara_state == idle || world->p2.chara_state == walk) && !world->p2.guard) || (world->p2.chara_state == stun && !world->p1.mirror))
        {
            world->p2.backwards = true;
            if (world->p2.chara_state == idle)
            {
                world->p2.chara_state = walk;
            
            }
            if (world->p2.chara_state == flight)
            {
                world->p2.chara_state = flight_control;
            }
            if (world->p2.chara_state == fall)
            {
                world->p2.chara_state = fall_control;
            }
            add_input_buffer(&world->p1, left, world->timestamp_w);
            }
       else if((world->p1.x > world->p2.x+100 && (world->p2.chara_state == idle || world->p2.chara_state == walk) && !world->p2.guard )|| (world->p2.chara_state == stun && !world->p1.mirror))
        {
            world->p2.backwards = false;
            if (world->p2.chara_state == idle)
            {
                world->p2.chara_state = walk;
            
            }
            if (world->p2.chara_state == flight)
            {
                world->p2.chara_state = flight_control;
            }
            if (world->p2.chara_state == fall)
            {
                world->p2.chara_state = fall_control;
            }
            add_input_buffer(&world->p1, right, world->timestamp_w);
        }
        else{
            if(world->p2.x+100 >= world->p1.x && world->p2.x <= world->p1.x && world->p1.mirror && world->p2.y == world->p1.y|| world->p2.x-100 <= world->p1.x && world->p2.x >= world->p1.x && !world->p1.mirror && world->p2.y == world->p1.y)
            {
                int random_value = (rand() % 4) + 1;
                switch (random_value)
                {
                case 1:
                    if(world->p2.chara_state == idle || world->p2.chara_state == walk && !world->p2.guard)
                    {
                        world->p2.guard = false;
                        add_input_buffer(&world->p2, light_p, world->timestamp_w);
                        world->p2.chara_state = lpunch;
                        world->p2.attack_launched = true;
                    }
                    break;
                case 2:
                    if(world->p2.chara_state == idle || world->p2.chara_state == walk && !world->p2.guard)
                    {
                        world->p2.guard = false;
                        add_input_buffer(&world->p2, heavy_p, world->timestamp_w);
                        world->p2.chara_state = hpunch;
                        world->p2.attack_launched = true;
                    }
                    break;
                case 3:
                    if(world->p2.chara_state == idle || world->p2.chara_state == walk && !world->p2.guard)
                    {
                        world->p2.guard = false;
                        add_input_buffer(&world->p2, kick, world->timestamp_w);
                        world->p2.chara_state = kickstate;
                        world->p2.attack_launched = true;
                    }
                        break;
                case 4:
                    if(world->p2.chara_state == idle || world->p2.chara_state == walk)
                    {
                        world->p2.guard = true;
                        world->p2.chara_state = idle;
                    }
                    break;
                default:

                    break;
                }
            }
        }
    }
                    
    
    // On stocke l'état de toutes les touches du clavier pour éviter les appuyés prolongés
    for (int i = 0; i < 123; i++)
    {
        world->keystates_pre[i] = keystates[i];
    }
}
