//
// Created by Mahan on 1/31/2022.
//

#ifndef MAIN_C_MAP_HANDLING_H
#define MAIN_C_MAP_HANDLING_H

#endif //MAIN_C_MAP_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum OWNERSHIP { NEUTRAL , USER , SYSTEM };
enum LANDTYPE { HEXAGON , RECTANGLE };


typedef struct land {
    enum LANDTYPE type ;
    enum OWNERSHIP owner ;
    int x ;
    int y ;
    int owner_id ;
    int soldiers_number ;
    int production_rate ;
} land ;

void map_handling_test_func () ;

int GENERATE_HEXAGON_RANDOM_MAP ( int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map , int HEXAGON_a) ;