//
// Created by Mahan on 1/31/2022.
//

#include "map_handling.h"


void map_handling_test_func()
{
    printf("this is a test from map_handling header\n") ;
}

void GENERATE_RANDOM_MAP ( int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map )
{
    int random_x ;
    int random_y ;

    int counter = 0 ;

    srand(time(0)) ;
    for ( int i=0 ; i<NUM_PLAYERS ; i++)
    {
        random_x = rand() % WIDTH ;
        random_y = rand() % HEIGHT ;

        if ( random_x > 20 && random_y > 20 && random_x<WIDTH-20 && random_y<HEIGHT-20)
        {
            map[counter].x = random_x ;
            map[counter].y = random_y ;
            map[counter].owner_id = i ;
            map[counter].type = HEXAGON ;
        }

    }
}

int GENERATE_HEXAGON_RANDOM_MAP ( int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map , int HEXAGON_a)
{
    srand(time(0)) ;
    int start_x_pos = rand()%40 + 20 ;
    int start_y_pos = rand()%40 + 20 ;

    int center_x ;
    int center_y = start_y_pos ;

    int counter = 0 ;

    for ( int i=0 ; i<14 ; i++){
        if (i%2==0)
            center_x = start_x_pos ;
        else
            center_x = start_x_pos+HEXAGON_a ;

        for ( int j=0 ; j<30 ; j++) {
            if (rand() % 8 == 0 || rand()%21 == 2 || rand()%40 == 0 ) {
                map[counter].x = center_x;
                map[counter].y = start_y_pos;
                counter++ ;
            }
            center_x += 2.3*HEXAGON_a ;
        }
        start_y_pos += 2.3*HEXAGON_a+3;
    }
    return  counter ;
}