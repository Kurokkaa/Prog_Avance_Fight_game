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
                    world->menu_set.index_menu == 3 ? world->menu_set.index_menu = 0 : world->menu_set.index_menu++;
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
                    world->menu_set.index_menu == 0 ? world->menu_set.index_menu = 3 : world->menu_set.index_menu--;
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
            world->p2.chara_state = idle;
        }
        if (keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT])
        {
            // world->p1.speed = -CHARA_SPEED;
            
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
            if (keystates[SDL_SCANCODE_LEFT] != world->keystates_pre[SDL_SCANCODE_LEFT])
            {
                add_input_buffer(&world->p2, left, world->timestamp_w);
            }
        }

        if (!keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT])
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
            if (keystates[SDL_SCANCODE_RIGHT] != world->keystates_pre[SDL_SCANCODE_RIGHT])
            {
                add_input_buffer(&world->p2, right, world->timestamp_w);
            }
        }

        if (!keystates[SDL_SCANCODE_UP] && keystates[SDL_SCANCODE_DOWN])
        {
            if (world->p2.chara_state == idle || world->p2.chara_state == walk)
            {
                world->p2.permibility = true;
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
                        setFrame(world->p2.anim, flight, 0);
                        setFrame(world->p1.anim, flight_control, 0);
                        world->p2.chara_state = flight;
                        world->p2.permibility = false;  
                        world->p2.double_jump = false;
                    }
            }
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
    // On stocke l'état de toutes les touches du clavier pour éviter les appuyés prolongés
    for (int i = 0; i < 123; i++)
    {
        world->keystates_pre[i] = keystates[i];
    }
}
-------------------------
#include "data.h"
#include "../constante.h"
#include "../init/init.h"
#include "../controller/controller.h"
#include "../lootbox/lootbox.h"
#include "../liste/liste.h"


/**
 * @brief lit le fichier de la carte afin de gérér collisions et l'affichage des plateformes
 * 
 * @param nom le nom du fichier
 * @return char**
 */
char **read_file_map(char *nom)
{
    //on ouvre le ficher en lecture binaire
    FILE *file = fopen(nom, "rb");
    //la map est divisé en 40 ligne de 18 pixels
    char **map_struct = malloc(sizeof(char *) * LINES);
    for (int i = 0; i < 40; i++)
    {
        //chaque ligne à 21 collones dont 20 effectifs de 64 pixels
        map_struct[i] = malloc(sizeof(char) * COLUMNS);
        fread(map_struct[i], COLUMNS, 1, file);
    }
    if (map_struct == NULL)
    {
        printf("Erreur de chargement de structure");
    }
    fclose(file);
    return map_struct;

}

/**
 * @brief charge l'image et crée la texture qui associé au renderer
 * 
 * @param path le nom du fichier
 * @return la texture de l'image
 */
SDL_Texture *load_image(char* path, SDL_Renderer *renderer)
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

/**
 * @brief lit les coups dans le buffer et recherche si le combo est dedans
 * 
 * @param player le perso où le buffer est lu
 * @return un combo à t-il été trouvé ?
 */
bool read_combo(sprite_perso *player, int val, jeu * world)
{
    // 1er combo
    bool found = false;
    bool equal;
    int j, k;
    int depart;
    // on essaye tous les points départ jusqu'a la position dans le buffer - le nombres de coups du combo testé 
    for (int i = 0; i <= player->pos_tab_combo - player->tab_combo[val].nb_coups && !found; i++)
    {
        //on estime que tous les coups suivants sont égaux
        equal = true;
        // on sauvegarde le point de départ de la lecture
        depart = i;
        //variable qui va parcourir les inputs du combo souhaité
        k = 0;
        //on regarde un nombre de coups égals à la taille du combo souhaité tant que les coups du buffer sont égaux
        for (j = i; j < i + player->tab_combo[val].nb_coups && equal; j++)
        {
            //si l'input du buffer et celui du combo testé ne sont pas égaux
            if (player->tab_combo[val].input[k] != player->buffer[j].input)
            {
                //on indique qu'il ne sont pas égaux pour arréter la lecture
                equal = false;
            }
            //on avance dans le combo
            k++;
        }
        //si tous les coups, le combo a été trouvé 
        if (equal)
        {
            //si le combo a été fait assez rapidement
            if (player->buffer[depart + player->tab_combo[val].nb_coups - 1].timestamp - player->buffer[depart].timestamp <= player->tab_combo[val].frame_between)
            {
                //on indique que ça a été trouvé 
                found = true;
            }
        }
    }
    if (found)
    {
        //on vide le buffer
        for (int i = 0; i < player->pos_tab_combo; i++)
        {
            player->buffer[i].input = 0;
            player->buffer[i].timestamp = 0;
        }
        //on repart au début
        player->pos_tab_combo = 0;
        if (player->special_bar >= player->tab_combo[val].required)
        {
            player->special_bar -= player->tab_combo[val].required;
            //selon le combo trouvé, on applique un effet 
            switch (val)
            {
            case 0:
                //on lance la boule feu en regardant ou se situe le joueur par rapport à l'autre
                player->chara_state = fireball;
                player->fireball.multiplicateur = player->mirror ? -1 : 1;
                break;
            case 2:
                //on lance la gravityball en regardant ou se situe le joueur par rapport à l'autre et s'il à déjà la moitié de l'ecran
                player->chara_state = gravityball;
                player->gravityball.multiplicateur = player->mirror || player->x > 630 ? -1 : 1;
                break;
            case 1:
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


/**
 * @brief décremente le timer du jeu
 * 
 * @param world le jeu 
 */
void check_timer(jeu *world)
{
    //si le timer n'est pas en pause et que le compteur est comptée
    if (world->timer.start && !world->timer.pause && !world->timer.inf)
    {

        if ((SDL_GetTicks() - world->timer.startTime) / 1000 >= 1)
        {
            world->timer.startTime = SDL_GetTicks();
            world->timer.timer--;
        }
    }
}

/**
 * @brief sauvegarde le compteur choisi 
 * 
 * @param select la sélection

 */
void save_counter(char *select)
{
    /*ecriture du compteur dans le fichier*/
    FILE *file = fopen("build/counter.bin", "wb");
    fwrite(select, strlen(select), 1, file);
    fclose(file);
}

/**
 * @brief lit le compteur choisi dans un fichier et l'applique à la partie
 * 
 * @param world le monde
 */
void read_counter(jeu *world)
{
    char *File_name = "build/counter.bin";
    //si le fichier n'existe pas on le crée avec une valeur par défaut
    if(!file_exists(File_name)){
        save_counter("90");
    }
    struct stat stat_file;
    FILE *file = fopen(File_name, "rb"); // ouverture en mode lecture
    stat(File_name, &stat_file);
    int sz = stat_file.st_size;
    char text_file[sz + 1];
    fread(text_file, 1, sz, file);
    text_file[sz] = '\0';
    world->timer.timer = atoi(text_file);
    //si le nombre est 999 on le considère comme infini
    if(world->timer.timer == 999){
        world->timer.inf = true;
    }
    else{
        world->timer.inf = false;
    }
    fclose(file);  
}

/**
 * FONCTIONS MOUVEMENTS + COUPS
 */

/**
 * @brief reset la frame de toutes les animation excepté celle actuel
 * 
 * @param perso le personnage
 * @param state l'etat actuel 
 * 
 */
void reset_state(sprite_perso *perso, enum character_state state)
{
    for (int i = 0; i <= gravityball; i++)
    {   //on ne reset le state où le perso est actuellement
        if (i != state)
        {
            setFrame(perso->anim, i, 0);
        }
    }
}

/**
 * @brief gère l'animation de l'aura
 * 
 * @param player le perso où le buffer est lu
 * 
 */
void manage_aura(sprite_perso *perso, jeu * world)
{
    anim * animation = getAnimation(perso->anim, 19);
    anim * animation2 = getAnimation(perso->anim, 20);
    //vitesse de l'animation
    if (animation->counter >=5)
    {
        //si l'aura est activé mais pas le bonus de dégats on veut la finir
        if (animation->aura && !perso->damage_bonus)
        {
            if (animation->frame != animation->nbFrame)
            {
                animation->frame++;
                animation2->frame++;
            }
            else
            {
                animation->aura = 0;
                animation->frame = 0;
                animation2->frame = 0;
                Mix_Pause(perso->audiochan.aura);
            }
        }
        // si le bonus de dégat et activé mais pas l'animation, on la lance
        if (!animation->aura && perso->damage_bonus)
        {
            animation->frame++;
            animation2->frame++;
            if (animation->frame == 2)
            {
                animation->aura = 1;
                Mix_PlayChannel(perso->audiochan.aura, world->music.aura, 0);
            }
        }
        // si les deux sont activé on la faire rebouclé
        if (perso->damage_bonus && animation->aura)
        {
            if (animation->frame == 4)
            {
                animation->frame = 2;
                animation2->frame = 2;
            }
            else
            {
                animation->frame++;
                animation2->frame++;
            }
        }
        animation->counter = 0;
    }
    else
    {
        animation->counter++;
    }
}

/**
 * @brief test sur quelle case du quadrillage 
 * 
 * @param x la coordonnée en x
 * @param y la coordonnée en y
 * @param map_point le quadrillage 
 * @param test le caractère à tester
 * @return un combo à t-il été trouvé ?
 */
bool equals(int x, int y, char **map_point, char test)
{
    int height_factor, width_factor;
    height_factor = CELL_HEIGHT;
    width_factor = CELL_WIDTH;
    return (map_point[y / height_factor][x / width_factor] == test);
}

/**
 * @brief ajout d'un input dans le buffer
 * 
 * @param player le perso où se trouve 
 * @return un combo à t-il été trouvé ?
 */
// fonction d'ajout de l'input dans le buffer
void add_input_buffer(sprite_perso *player, enum combos_inputs touche_appui, int timestamp)
{   //si le buffer est déjà plein, on décale le buffer pour accepter la nouvelle valeur
    if (player->pos_tab_combo == BUFFER_SIZE)
    {
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            player->buffer[i] = player->buffer[i + 1];
        }
        player->pos_tab_combo = BUFFER_SIZE - 1;
    }
    //et on inscrit l'input à la dernière position
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

/**
 * @brief libère les fonts
 * 
 * @param world le monde
 */
void TTF_free_police(jeu* world){
    free(world->font.police_compteur);
    free(world->font.police_victoire);
}
/**
 * @brief libère les musiques
 * 
 * @param world le monde
 */
void free_music(jeu *world)
{
    if(world->in_game){
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
        Mix_FreeChunk(world->music.punch_air);
        Mix_FreeChunk(world->music.win_laugh);
    }
    Mix_FreeChunk(world->music.menu);
    Mix_FreeChunk(world->music.menu_selector);
}
/**
 * @brief libération de la mémoire
 * @param fenetre la SDL_windows
 * @param renderer le renderer
 * @param world le monde
 */
void quit_game(jeu *world, SDL_Window **fenetre, SDL_Renderer **renderer)
{
    if(world->in_game){
        free_hits(world);
        TTF_free_police(world);
        free_map_structure(world->map.map_structure);
        destroy_textures(world);
    }
    
    free_menu(world);
    free_music(world);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_DestroyWindow(*fenetre);
    SDL_DestroyRenderer(*renderer);
    SDL_Quit();
}
/**
 * @brief libère les textures des menus
 * 
 * @param world le monde
 */
void free_menu(jeu* world){
    SDL_DestroyTexture(world->menu_set.menu);
    SDL_DestroyTexture(world->menu_set.selection_maps_menu);
    SDL_DestroyTexture(world->menu_set.options_menu);
}

/**
 * @brief libère le quadrillage de la map
 * 
 * @param world le monde
 */

void free_map_structure(char **map_structure)
{
    for (int i = 0; i < 40; i++)
    {
        free(map_structure[i]);
    }
    free(map_structure);
}
/**
 * @brief libère toutes les animations
 * 
 * @param world le monde
 */
void destroy_anim(sprite_perso *perso)
{
    freeList(perso->anim);
}
/**
 * @brief libère les miniatures de la séléction de la map
 * 
 * @param world le monde
 */
void destroy_map(jeu *world)
{
    for(int i = 0 ; i<NB_MAPS ; i++){
        SDL_DestroyTexture(world->menu_set.tab_map[i]);
    }
}
/**
 * @brief libère les textures de la partie
 * 
 * @param world le monde
 */
void destroy_textures(jeu *world)
{
    destroy_anim(&world->p1);
    destroy_anim(&world->p2);
    SDL_DestroyTexture(world->map.image_fond);
    SDL_DestroyTexture(world->map.plateformes);
    SDL_DestroyTexture(world->menu_set.cadreVie);
    for (int i = 0; i < special_bonus; i++)
    {
        SDL_DestroyTexture(world->lootbox.texture[i]);
    }
    destroy_map(world);
}
/**
 * @brief met en pause le chronomètre 
 * 
 * @param world le monde
 */
void unpause(compteur *chrono)
{
    if (chrono->start)
    {
        //on souhaite recupérer la même différence à la reprise pour calculer le temps écoulé
        chrono->startTime = SDL_GetTicks() - chrono->pauseTime;
        chrono->pause = false;
    }
}
/**
 * @brief remet en route le chronomètre
 * 
 * @param world le monde
 */
void pauseChrono(compteur *chrono)
{
    if (chrono->start)
    {
        chrono->pause = true;
        // on remet le même temps qui c'est écoulé avant la pause
        chrono->pauseTime = SDL_GetTicks() - chrono->startTime;
    }
}
/**
 * @brief régenère la garde du perso 
 * 
 * @param perso le personnage
 */
void regenerate_shield(sprite_perso *perso)

{  
    //si le chrono n'est pas en pause,qu'il c'est écoulé une seconde, qu'elle n'est pas à fond et qu'il n'est pas en garde
    if (!perso->chrono_guard.pause && !perso->broken_guard && (SDL_GetTicks() - perso->chrono_guard.startTime) / 1000 >= 1 && perso->life_guard < MAX_GUARD && !perso->guard)
    {
        //on augmente la garde
        perso->life_guard += 10;
        //on réinitialise le chronomètre
        perso->chrono_guard.startTime = SDL_GetTicks();
    }
}
/**
 * @brief régenère la jauge de spécial 
 * 
 * @param perso le personnage
 */
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
/**
 * @brief détermine le nombre de chiffre d'un nombre
 * 
 * @param n la valeur 
 */
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
/**
 * @brief on écrit le nouveaux score
 * 
 * @param nb1 le nombre de victoire du joueur 1
 * @param nb2 le nombre de victoire du joueur 2
 */
void write_victory(int nb1, int nb2)
{
    char new_score[8 + numDigits(nb1) + numDigits(nb2)];
    FILE *file = fopen("build/victory.bin", "wb");
    sprintf(new_score, "p1:%d\np2:%d\n", nb1, nb2);
    fwrite(new_score, 7 + numDigits(nb1) + numDigits(nb2), 1, file);
    fclose(file);
}
/**
 * @brief vérifie si un fichier existe
 * 
 * @param bool le fichier existe il ?
 */
bool file_exists (char *filename) 
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

/**
 * @brief sauvegarde les victoires 
 * 
 * @param player le personnage
 */
void save_victory(int player)
{
    char *File_name = "build/victory.bin";
    if(!file_exists(File_name)){
        write_victory(0,0);
    }
    struct stat stat_file;
    FILE *file = fopen(File_name, "rb"); // ouverture en mode lecture
    stat(File_name, &stat_file);
    int sz = stat_file.st_size;
    char* text_file=malloc(sizeof(char)*sz+1);
    
    fread(text_file, 1, sz, file);
    text_file[sz] = '\0';

    int line = 1;
    int size;
    int nb1 = 0;
    int nb2 = 0;
    int i;
    for (i = 0; i < sz; i++)
    {
        //si un caractère de fin de ligne est prèsent on à changé de 
        if (text_file[i] == '\n')
        {
            line++;
        }
        //si c'est un :
        if (text_file[i] == ':')
        {
            size = 0;
            //on détermine la taille du conteneur du score
            while (text_file[i + size + 1] != '\n')
            {
                size++;
            }
            // on passe au caractère d'après
            i += 1;
            int index = 0;
            char* nombre[size+1];
            int pos = 0;
            nombre[size] = '\0';
            // on recopie tous dans le conteneur du score
            while (text_file[i + index] != '\n' && index < size)
            {

                nombre[pos++] = text_file[i + index];

                index++;
            }
            //selon la ligne, on met dans le bon variable
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
    //on ecrit le nouveau
    write_victory(nb1, nb2);
    free(text_file);
    
}
/**
 * @brief vérifie les limites des stats
 * 
 * @param perso le personnage
 */
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
/**
 * @brief gère la fin de partie 
 * 
 * @param world le monde
 */
void compute_game(jeu *world)
{
    if (!world->game_over)
    {
        if (world->timer.timer == 0)
        {
            world->timer.startTime = SDL_GetTicks();
            world->timer.timer = ENDGAME_TIMER;
            //P1 à gagné
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
            //P2 à gagné
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
            //le timer est à 0 et P1 à gagné
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
            //le timer est à 0 et P2 à gagné
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
    anim * animation;
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
        setFrame(world->p1.anim, winner, getAnimation(world->p1.anim, winner)->frame + 1);

        if (getAnimation(world->p1.anim, winner)->frame == getAnimation(world->p1.anim, winner)->nbFrame)
        {
            setFrame(world->p1.anim, winner, 5);
        }
        setFrame(world->p2.anim, looser, getAnimation(world->p2.anim, looser)->frame + 1);
        if (getAnimation(world->p2.anim, looser)->frame == getAnimation(world->p2.anim, looser)->nbFrame)
        {
            setFrame(world->p2.anim, looser, getAnimation(world->p2.anim, looser)->nbFrame - 1);
        }
    }

    else
    {
        setFrame(world->p2.anim, winner, getAnimation(world->p2.anim, winner)->frame + 1);

        if (getAnimation(world->p2.anim, winner)->frame == getAnimation(world->p2.anim, winner)->nbFrame)
        {
            setFrame(world->p2.anim, winner, 5);
        }
        setFrame(world->p1.anim, looser, getAnimation(world->p1.anim, looser)->frame + 1);
        
        if (getAnimation(world->p1.anim, looser)->frame == getAnimation(world->p1.anim, looser)->nbFrame)
        {
            setFrame(world->p1.anim, looser, getAnimation(world->p1.anim, looser)->nbFrame - 1);
        }
    }
}