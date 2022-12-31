#ifndef constante_h
#define constante_h

    #define SCREEN_WIDTH 1280
    #define SCREEN_HEIGHT 720
    #define LINES 40
    #define COLUMNS 20
    #define CELL_HEIGHT 18
    #define CELL_WIDTH 64
    #define CHARA_SPEED CELL_WIDTH * 0.1
    #define NB_MAPS 3
    #define KNOCKBACK_SPEED CELL_WIDTH * 0.1
    #define BUFFER_SIZE 60
    #define NB_COMBOS 3
    #define RANDOM_SPAWN_CHANCE 500
    #define MAX_LIFE 300
    #define MAX_SPECIAL 300
    #define DMG_DURATION 5
    #define BROKEN_GUARD_STUN 400
    #define MAX_GUARD 150
    #define DISTANCE_MAX 1280
    #define BLACK_HOLE_STRENGH 4

    enum game_state{combat, main_menu, pause, options, selection_perso, selection_map, endgame};
    enum character_state{idle, walk, jump, crouch, fall, backwards, flight, flight_control, fall_control, landing, knockback, lpunch , kickstate,hpunch,stun,fireball,gravityball, winner , looser };
    enum game_map{city_night, forest, japan};
    enum combos_inputs{up, down, left, right, kick, light_p, heavy_p, special};
    enum bonus_type{health_bonus, damage_bonus, special_bonus};
    enum son{music, selector};

#endif