#include "lootbox.h"

/**
 * GESTION DES LOOTBOX
 */

/**
 * @brief Génère un nombre aléatoire entre la borne a et b (exclus)
 * 
 * @param a borne minimum
 * @param b borne maximum (exclus)
 * @return int compris entre a et b
 */
int generate_number(int a, int b)
{
    return rand() % (b - a) + a;
}

/**
 * @brief Reset et active une lootbox
 * 
 * @param lootbox lootbox à reset et activer
 */
void reset_activate_lootbox(lootbox *lootbox)
{
    lootbox->active = true; // on active la lootbox 
    lootbox->falling = true; //on active la chute de la lootbox
    lootbox->x = generate_number(65, 950); // la lootbox doit tomber dans les zones accessibles au joueur
    lootbox->y = 0 - lootbox->h; //placement y de la lootbox
    lootbox->collided = 0; //initalisation non collisionné
    lootbox->bonus = generate_number(health_bonus, special_bonus + 1); // tirage aléatoire du bonus
}

/**
 * @brief Test si les lootbox entre en collision ou continuent à chuter
 *
 * @param world les données du monde du jeu
 */
void update_lootbox(jeu *world)
{
    if (world->lootbox.falling) //Si lootbox en état de chute
    {
        if ((equals(world->lootbox.x, world->lootbox.y + world->lootbox.h + world->lootbox.fallspeed, world->map.map_structure, ' ') || (equals(world->lootbox.x + world->lootbox.w, world->lootbox.y + world->lootbox.h + world->lootbox.fallspeed, world->map.map_structure, ' ') || world->lootbox.y < CELL_HEIGHT + 20) && world->lootbox.y < 667)) //on test si la lootbox tombe sur une plateforme ou au point le plus bas de la map
        {

            world->lootbox.y += world->lootbox.fallspeed; //vitesse de chute de la lootbox
        }
        else //si la lootbox touche une plateforme ou limite, elle ne tombe plus
        {
            world->lootbox.falling = false; 
        }
    }
}

/**
 * @brief Test si un joueur a ramassé une lootbox
 * 
 * @param player un sprite d'un joueur
 * @param lootbox une structure lootbox
 * @param player_number le numéro du joueur, permet d'attribuer un bonus
 */
void check_lootbox_pickup(sprite_perso *player, lootbox *lootbox, int player_number)
{
    if (lootbox->active) //si lootbox active
    {
        int tete = player->y;
        int pieds = player->y + player->h;

        if (lootbox->y >= tete && lootbox->y <= pieds) //test de collision entre joueur et lootbox en y
        {
            if (player->x + player->w >= lootbox->x && player->x <= lootbox->x + lootbox->w) //test de collision entre joueur et lootbox en x
            {
                if (lootbox->collided != 0) //si lootbox est touché par un autre joueur, elle passe au numéro du joueur. Si les deux la touche, elle passe à 3 pour appliquer le bonus aux deux
                {
                    lootbox->collided = 3;
                }
                else
                {
                    lootbox->collided = player_number; //donne le bonus au joueur qui attrape la lootbox
                }
            }
        }
    }
}

/**
 * @brief Applique le bonus de la lootbox
 * 
 * @param lootbox une structure lootbox
 * @param player sprite du joueur
 */
void apply_bonus(lootbox *lootbox, sprite_perso *player)
{
    int x = 50; // taille du bonus de vie ou spécial


    if (lootbox->bonus == health_bonus) //bonus vie
    {
        player->life += x;
    }

    if (!player->damage_bonus && lootbox->bonus == damage_bonus && !getAnimation(player->anim, 19)->aura) //Bonus dommage
    {
        player->damage_bonus = true; //activation du bonus dommage
        player->dmg_bonus_timer.start = true; //on lance le compteur 
        player->dmg_bonus_timer.startTime = SDL_GetTicks(); //on définit sa valeur de référence
        player->dmg_bonus_timer.pause = false;
    }

    if (lootbox->bonus == special_bonus) //Bonus de barre spécial
    {
        player->special_bar += x;
    }
}

/**
 * @brief Loop qui gère le fonctionnement des lootbox
 * 
 * @param world les données du monde du jeu
 */
void lootbox_loop(jeu *world)
{
    if (world->lootbox.active == 0) // On ne fait pas apparaitre de lootbox s'il y en a déjà une
    { 
        int n = generate_number(0, RANDOM_SPAWN_CHANCE); //tirage aléatoire en fonction de la constante RANDOM_SPAWN_CHANCE

        if (n == 1)// Si la fonction renvoie 1, alors on créé une lootbox
        {
            reset_activate_lootbox(&world->lootbox); // reset et réactive une lootbox à un endroit et avec un bonus aléatoire
        }
    }

    if (world->lootbox.active == 1) // Cas où une lootbox est active
    { 
        update_lootbox(world);                                // mouvement de la lootbox
        check_lootbox_pickup(&world->p1, &world->lootbox, 1); // vérifie si le joueur 1 a ramassé la lootbox
        check_lootbox_pickup(&world->p2, &world->lootbox, 2); // vérifie si le joueur 2 a ramassé la lootbox

        if (world->lootbox.collided == 1) //Le joueur 1 a ramassé la lootbox
        {
            apply_bonus(&world->lootbox, &world->p1);
            world->lootbox.active = false;
        }

        if (world->lootbox.collided == 2)
        {
            apply_bonus(&world->lootbox, &world->p2); //Le joueur 2 a ramassé la lootbox
            world->lootbox.active = false;
        }

        if (world->lootbox.collided == 3) //Les 2 joueurs ont ramassé la lootbox
        {
            apply_bonus(&world->lootbox, &world->p1);
            apply_bonus(&world->lootbox, &world->p2);
            world->lootbox.active = false;
        }
    }
}

/**
 * @brief Vérifie si le temps du bonus de damage est expiré
 * 
 * @param perso le perso dont on teste la durée du bonus
 */
void check_bonus(sprite_perso *perso)
{
    if (perso->damage_bonus && !perso->dmg_bonus_timer.pause && (SDL_GetTicks() - perso->dmg_bonus_timer.startTime) / 1000 >= DMG_DURATION) //vérifie si le joueur a encore du temps pour son bonus, si on rentre dans la condition, on le désactive
    {
        perso->damage_bonus = false; //désactivation du dommage
        perso->dmg_bonus_timer.startTime = SDL_GetTicks();
        perso->berserk = false; //désactivation du berserk si nécessaire
    }
}