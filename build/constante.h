#ifndef constante_h
#define constante_h
    #define SCREEN_WIDTH 1280
    #define SCREEN_HEIGHT 720
    #define LINES 40
    #define COLUMNS 20
    #define CELL_HEIGHT SCREEN_HEIGHT/LINES
    #define CELL_WIDTH SCREEN_WIDTH/COLUMNS

enum game_state{combat, menu, pause, options};
enum character_state{rest, walking, jump, falling};
enum map_choice{dojo,russia};

#endif