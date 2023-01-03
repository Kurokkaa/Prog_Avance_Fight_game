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
    IMG_Quit();
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