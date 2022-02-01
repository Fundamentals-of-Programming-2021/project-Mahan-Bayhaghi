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

void ShowHexagonBackground ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , land* map , int counter , Sint16 a)
{
    for ( int i=0 ; i<counter ; i++)
    {
        Sint16 center_x = map[counter].x ;
        Sint16 center_y = map[counter].y ;
        Sint16 * vx = malloc(sizeof(Sint16) * 6 ) ;
        Sint16 * vy = malloc(sizeof(Sint16) * 6 ) ;
        vx[0] = (center_x-a) ;  vx[1] = (center_x-a/2) ; vx[2] = (center_x+a/2) ;
        vx[3] = (center_x+a) ;  vx[4] = (center_x+a/2) ; vx[5] =  (center_x-a/2) ;
//        vx = { (center_x-a) , (center_x-a/2) , (center_x+a/2) , (center_x+a) , (center_x+a/2) , (center_x-a/2) } ;
        vy[0] = (center_y) ;  vy[1] = (center_y-(sqrt(3)*a/2)) ; vy[2] = (center_y-(sqrt(3)*a/2)) ;
        vy[3] = (center_y) ;  vy[4] = (center_y+(sqrt(3)*a/2)) ; vy[5] = (center_y+(sqrt(3)*a/2)) ;
//        vy = { (center_y) , (center_y-(sqrt(3)*a/2)) , (center_y-(sqrt(3)*a/2)) , (center_y) , (center_y+(sqrt(3)*a/2)) , (center_y+(sqrt(3)*a/2))};
        polygonColor(sdlRenderer , vx , vy , 6 , 0xff0059ff) ;

    }
}


int GENERATE_HEXAGON_RANDOM_MAP (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map , int HEXAGON_a)
{
    srand(time(0)) ;
    Sint16 start_x_pos = rand()%40 + 40 ;
    Sint16 start_y_pos = rand()%40 + 60 ;

    Sint16 center_x ;
    Sint16 center_y = start_y_pos ;

    int counter = 0 ;

    for ( int i=0 ; i<12 ; i++)
    {
        if ( i%2==0 )
            center_x = start_x_pos ;
        else
            center_x = start_x_pos+ 1.5*HEXAGON_a ;

        Sint16 a = HEXAGON_a ;
        for ( int j=0 ; j<7 ; j++) {
            map[counter].x = center_x ;
            map[counter].y = center_y ;
            printf("%d --> %d %d\n" , counter , center_x , center_y ) ;
            Sint16 *vx = malloc(6 * sizeof(Sint16)) ;
            Sint16 *vy = malloc(6 * sizeof(Sint16)) ;
            vx[0] = (center_x-a) ;  vx[1] = (center_x-a/2) ; vx[2] = (center_x+a/2) ;
            vx[3] = (center_x+a) ;  vx[4] = (center_x+a/2) ; vx[5] =  (center_x-a/2) ;
            vy[0] = (center_y) ;  vy[1] = (center_y-(sqrt(3)*a/2)) ; vy[2] = (center_y-(sqrt(3)*a/2)) ;
            vy[3] = (center_y) ;  vy[4] = (center_y+(sqrt(3)*a/2)) ; vy[5] = (center_y+(sqrt(3)*a/2)) ;
            filledPolygonColor(sdlRenderer , vx , vy , 6 , 0xff7f59ff) ;
            polygonColor(sdlRenderer , vx , vy , 6 , 0xffffffff) ;
            counter++;
            center_x += 3*HEXAGON_a ;
        }
        center_y += (Sint16) (sqrt(3)*HEXAGON_a/2) ;
    }
    ShowHexagonBackground(sdlWindow , sdlRenderer , map , counter , HEXAGON_a ) ;
    return  counter ;
}

