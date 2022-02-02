//
// Created by Mahan on 1/31/2022.
//

#include "map_handling.h"
#include "background_handling.h"

Uint32 MAP_HANDLING_COLORS[4] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffb00500 };
// grey          // blue          // green        // red

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

int ShowHexagonBackground ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , land* map , int counter , Sint16 a)
{
    for ( int i=0 ; i<counter ; i++)
    {
        if ( map[i].x == 0 || map[i].y == 0 )
            return i ;
        Sint16 center_x = map[i].x ;
        Sint16 center_y = map[i].y ;
        Sint16 *vx = malloc(6 * sizeof(Sint16)) ;
        Sint16 *vy = malloc(6 * sizeof(Sint16)) ;
        vx[0] = (center_x-a) ;  vx[1] = (center_x-a/2) ; vx[2] = (center_x+a/2) ;
        vx[3] = (center_x+a) ;  vx[4] = (center_x+a/2) ; vx[5] =  (center_x-a/2) ;
        vy[0] = (center_y) ;  vy[1] = (center_y-(sqrt(3)*a/2)) ; vy[2] = (center_y-(sqrt(3)*a/2)) ;
        vy[3] = (center_y) ;  vy[4] = (center_y+(sqrt(3)*a/2)) ; vy[5] = (center_y+(sqrt(3)*a/2)) ;
        filledPolygonColor(sdlRenderer , vx , vy , 6 , MAP_HANDLING_COLORS[map[i].owner_id]) ;
        polygonColor(sdlRenderer , vx , vy , 6 , 0xffffffff) ;

        char* soldiers_number = malloc(sizeof(char) * 3) ;
        sprintf(soldiers_number , "%d" , map[i].soldiers_number) ;
        stringColor(sdlRenderer , center_x-5 , center_y+5 , soldiers_number , 0xff000000) ;

    }
}

land* GENERATE_HEXAGON_RANDOM_MAP (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map , int HEXAGON_a)
{
    int* ownership_watcher = calloc(4 , sizeof(int)) ;

    srand(time(0)) ;
    Sint16 start_x_pos = rand()%40 + 50 ;
    Sint16 start_y_pos = rand()%40 + 100 ;

    Sint16 center_x ;
    Sint16 center_y = start_y_pos ;

    int counter = 0 ;

    for ( int i=0 ; i<16 ; i++)
    {
        if ( i%2==0 )
            center_x = start_x_pos ;
        else
            center_x = start_x_pos+ 1.5*HEXAGON_a ;

        Sint16 a = HEXAGON_a ;
        for ( int j=0 ; j<8 ; j++) {
            if ( rand()%7 == 0 || rand()%5 == 0 || rand()%8 == 0 )
            {
                map[counter].x = center_x ;
                map[counter].y = center_y ;

                if ( rand()%7 == 0 || rand()%10 == 0 || rand()%16 == 0 )   // USER or SYSTEM land //
                {
                    map[counter].owner_id = rand() % 3 + 1;
                    map[counter].soldiers_number = 25;
                    if (map[counter].owner_id == 1)
                        map[counter].type = USER;
                    else
                        map[counter].type = SYSTEM;
                    map[counter].production_rate = 2;
                }
                                                                            // NEUTRAL land //
                else {
                    map[counter].owner_id = 0;
                    map[counter].soldiers_number = 25;
                    map[counter].type = NEUTRAL;
                    map[counter].production_rate = 0;
                }
                ownership_watcher[map[counter].owner_id] += 1 ;
                map[counter].counter = counter ;
                counter++;
            }
            center_x += 3*HEXAGON_a ;
        }
        center_y += (Sint16) (sqrt(3)*HEXAGON_a/2) ;
    }

    for ( int i=1 ; i<4 ; i++)
    {
        if ( ownership_watcher[i] < 4 )
        {
            int temp_counter = 0 ;
            while ( map[temp_counter].owner_id != 0 && temp_counter<counter )
                temp_counter++ ;

            map[temp_counter].owner_id = i ;
            map[temp_counter].soldiers_number = 25 ;
            if ( i== 1 )
                map[temp_counter].type = USER ;
            else
                map[temp_counter].type = SYSTEM ;
            map[temp_counter].production_rate = 2 ;
            ownership_watcher[i] += 1 ;
        }
    }

    return  map ;
}

// temp generate_hexagon_random_map
//land* GENERATE_HEXAGON_RANDOM_MAP (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map , int HEXAGON_a)
//{
//    int* ownership_watcher = calloc(4 , sizeof(int)) ;
//
//    srand(time(0)) ;
//    Sint16 start_x_pos = rand()%40 + 50 ;
//    Sint16 start_y_pos = rand()%40 + 100 ;
//
//    Sint16 center_x ;
//    Sint16 center_y = start_y_pos ;
//
//    int counter = 0 ;
//
//    for ( int i=0 ; i<16 ; i++)
//    {
//        if ( i%2==0 )
//            center_x = start_x_pos ;
//        else
//            center_x = start_x_pos+ 1.5*HEXAGON_a ;
//
//        Sint16 a = HEXAGON_a ;
//        for ( int j=0 ; j<8 ; j++) {
//            if ( rand()%7 == 0 || rand()%5 == 0 || rand()%8 == 0 )
//            {
//                map[counter].x = center_x ;
//                map[counter].y = center_y ;
//                map[counter].production_rate = 0 ;
//                map[counter].soldiers_number = 25 ;
//                map[counter].owner_id = 0 ;
//                map[counter].counter = counter ;
//                counter++;
//            }
//            center_x += 3*HEXAGON_a ;
//        }
//        center_y += (Sint16) (sqrt(3)*HEXAGON_a/2) ;
//    }
//
//    for ( int i=0 ; i<counter ; i++)
//    {
//        if ( rand()%6 == 0 )
//        {
//            int new_ownership = rand()%3 + 1 ;
//            map[i].owner_id = new_ownership ;
//            map[i].production_rate = 2 ;
//
//            int temp_counter = i ;
//
//            // up-left land //
//
//            // going forward enough 2 lands //
//
//            // going up enough 3 lands //
//
//            // goint down enough 3 lands //
//
//        }
//    }
//
//    return  map ;
//}


void AddSoldiers ( land* map , int counter )
{
    for ( int i=0 ; i<counter ; i++)
    {
        if ( map[i].soldiers_number < 120 ) {
            map[i].soldiers_number += map[i].production_rate;
            if (map[i].soldiers_number >= 120)
                map[i].soldiers_number = 120;
        }
    }
}

land GiveClickedCellInfo ( Sint16 x , Sint16 y , land* map , int counter , int Hexagon_a )
{
    for ( int i=0 ; i<counter ; i++ )
    {
        Sint16 abs_delta_x = abs(x-map[i].x) ;
        Sint16 abs_delta_y = abs(y-map[i].y) ;
        if ( abs_delta_x < Hexagon_a  &&  abs_delta_y < Hexagon_a )
        {
            if ( abs_delta_x*abs_delta_x + abs_delta_y*abs_delta_y <= 3*Hexagon_a*Hexagon_a/4 )
                return map[i] ;
        }
    }
    land temp = {.x=0 , .y=0 , .owner_id=-1 };
    return  temp ;
}

SOLDIER_LINE* CreatSoldierLine ( soldier soldier_info )
{
    SOLDIER_LINE* header = malloc(sizeof(SOLDIER_LINE*)) ;
    header->next = NULL ;
    header->prev = NULL ;
    header->information = soldier_info ;

    return  header ;
}

void AddToSoldierLine ( SOLDIER_LINE* header , struct soldier soldier_info )
{
    while ( header->next != NULL )
        header = header->next ;

    header->information = soldier_info ;
    header->next = malloc(sizeof(SOLDIER_LINE*)) ;
    header->next->prev = header->prev ;
}