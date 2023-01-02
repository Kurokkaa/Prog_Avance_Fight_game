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
 * @param lootbox à reset et activer
 */
void reset_activate_lootbox(lootbox *lootbox)
{
    lootbox->active = true;
    lootbox->falling = true;
    lootbox->x = generate_number(65, 950);
    lootbox->y = 0 - lootbox->h;
    lootbox->collided = 0;
    lootbox->bonus = generate_number(health_bonus, special_bonus + 1);
}

/**
 * @brief Test si les lootbox entre en collision ou continuent à chuter
 *
 * @param world les données du monde du jeu
 */
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

/**
 * @brief Test si un joueur a ramassé une lootbox
 * 
 * @param player un sprite d'un joueur
 * @param lootbox une lootbox
 * @param player_number le numéro du joueur, permet d'attribuer un bonus
 */
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

/**
 * @brief Applique le bonus de la lootbox
 * 
 * @param lootbox 
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
        player->damage_bonus = true;
        player->dmg_bonus_timer.start = true;
        player->dmg_bonus_timer.startTime = SDL_GetTicks();
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

void check_bonus(sprite_perso *perso)
{
    if (perso->damage_bonus && !perso->dmg_bonus_timer.pause && (SDL_GetTicks() - perso->dmg_bonus_timer.startTime) / 1000 >= DMG_DURATION)
    {
        perso->damage_bonus = false;
        perso->dmg_bonus_timer.startTime = SDL_GetTicks();
        perso->berserk = false;
    }
}