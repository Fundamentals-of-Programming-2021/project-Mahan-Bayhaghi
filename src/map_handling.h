#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

//
// Created by Mahan on 1/31/2022.
//

#ifndef MAIN_C_MAP_HANDLING_H
#define MAIN_C_MAP_HANDLING_H
#endif //MAIN_C_MAP_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


enum OWNERSHIP { NEUTRAL , USER , SYSTEM };
enum LANDTYPE { HEXAGON , RECTANGLE };

typedef struct land {
    enum LANDTYPE type ;
    enum OWNERSHIP owner ;
    Sint16 x ;
    Sint16 y ;
    int owner_id ;
    int soldiers_number ;
    int production_rate ;
    int counter ;
} land ;

typedef struct soldier {
    Sint16 origin_x ;
    Sint16 origin_y ;
    Sint16 destination_x ;
    Sint16 destination_y ;
    int speed ;
} soldier ;

typedef struct SOLDIER_LINE {
    soldier information ;
    struct SOLDIER_LINE* next ;
    struct SOLDIER_LINE* prev ;
} SOLDIER_LINE ;

void map_handling_test_func () ;    // A simple test function to see if header works


// function to create randomized hexagon map
// returns pointer to out game map ( pointer to an array of lands )
land* GENERATE_HEXAGON_RANDOM_MAP (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map , int HEXAGON_a) ;


// function to display hexagon map
// returns number of cells in the game
int ShowHexagonBackground ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , land* map , int counter , Sint16 a) ;

// function to add soldiers to lands //
void AddSoldiers ( land* map , int counter ) ;


// function to specify which cell is being clicked on
land GiveClickedCellInfo ( Sint16 x , Sint16 y , land* map , int counter , int Hexagon_a ) ;


// a function to creat a line of soldiers
SOLDIER_LINE* CreatSoldierLine ( soldier soldier_info ) ;

// a function to add a soldier to line
void AddToSoldierLine ( SOLDIER_LINE* header , soldier soldier_info ) ;