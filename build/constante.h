#ifndef constante_h
#define constante_h

    #define SCREEN_WIDTH 1280
    #define SCREEN_HEIGHT 720
    #define LINES 40
    #define COLUMNS 20
    #define CELL_HEIGHT SCREEN_HEIGHT/LINES
    #define CELL_WIDTH SCREEN_WIDTH/COLUMNS
    #define CHARA_SPEED CELL_WIDTH * 0.1
    #define NB_MAPS 3
    #define KNOCKBACK_SPEED CELL_WIDTH * 0.1
    #define BUFFER_SIZE 60
    #define NB_COMBOS 1 

    enum game_state{combat, main_menu, pause, options, selection_perso, selection_map};

    enum character_state{idle, walk, jump, crouch, fall, backwards, flight, flight_control, fall_control, landing, knockback, lpunch , kickstate};
    enum game_map{russia, forest, street_art};
    enum combos_inputs{up, down, left, right, kick, light_p, heavy_p, special};

#endif