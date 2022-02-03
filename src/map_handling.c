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

__attribute__((unused)) void GENERATE_RANDOM_MAP ( int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map )
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



void CreateLineOfSoldiers ( OneSoldier** AllSoldiersArray , land* map_arr , int Origin_counter , land Destination_cell_info )
{
    int i=0 ;
    while ( AllSoldiersArray[i] != NULL || AllSoldiersArray[i] != 0 )
        i++ ;

    // allocating a line of soldiers with proper length
    AllSoldiersArray[i] = malloc( sizeof(OneSoldier) * map_arr[Origin_counter].soldiers_number ) ;
    while ( AllSoldiersArray[i] == NULL )
    {
        printf("memory allocation for line of soldier incomplete !\nTrying again\n");
        AllSoldiersArray[i] = malloc(sizeof(OneSoldier) * map_arr[Origin_counter].soldiers_number);
    }

    // Initializing first soldier as sample
    AllSoldiersArray[i][0].origin_x = map_arr[Origin_counter].x ;
    AllSoldiersArray[i][0].origin_y = map_arr[Origin_counter].y ;
    AllSoldiersArray[i][0].destination_x = Destination_cell_info.x ;
    AllSoldiersArray[i][0].destination_y = Destination_cell_info.y ;
    AllSoldiersArray[i][0].destination_counter = Destination_cell_info.counter ;
    AllSoldiersArray[i][0].owner_id = 1 ;
    AllSoldiersArray[i][0].power = 1 ;
    AllSoldiersArray[i][0].soldier_id = 1 ;
    AllSoldiersArray[i][0].num_of_all_soldiers = map_arr[Origin_counter].soldiers_number ;

    // angel management //
    double delta_y =  Destination_cell_info.y - (map_arr[Origin_counter].y) ;
    double delta_x = Destination_cell_info.x - (map_arr[Origin_counter].x) ;
    double theta = delta_y / delta_x ;

    AllSoldiersArray[i][0].verticalSpeed = 8.0 * sin(atan(theta) ) ;
    if ( delta_y > 0 )
        AllSoldiersArray[i][0].verticalSpeed = fabs(AllSoldiersArray[i][0].verticalSpeed) ;
    else if ( delta_y <0 )
        AllSoldiersArray[i][0].verticalSpeed = -1 * fabs(AllSoldiersArray[i][0].verticalSpeed) ;
    AllSoldiersArray[i][0].horizontalSpeed = 8.0 * cos(atan(theta) ) ;
    if ( delta_x > 0 )
        AllSoldiersArray[i][0].horizontalSpeed = fabs(AllSoldiersArray[i][0].horizontalSpeed) ;
    else if ( delta_x <0 )
        AllSoldiersArray[i][0].horizontalSpeed = -1 * fabs(AllSoldiersArray[i][0].horizontalSpeed) ;

    AllSoldiersArray[i][0].x = map_arr[Origin_counter].x ;
    AllSoldiersArray[i][0].y = map_arr[Origin_counter].y ;

    int c = map_arr[Origin_counter].soldiers_number ;
    for ( int temp_c = 1 ; temp_c<c ; temp_c++ )
    {
        AllSoldiersArray[i][temp_c].x = AllSoldiersArray[i][temp_c-1].x - 1.5*AllSoldiersArray[i][0].horizontalSpeed ;
        AllSoldiersArray[i][temp_c].y = AllSoldiersArray[i][temp_c-1].y - 1.5*AllSoldiersArray[i][0].verticalSpeed ;
        AllSoldiersArray[i][temp_c].destination_x = AllSoldiersArray[i][0].destination_x ;
        AllSoldiersArray[i][temp_c].destination_y = AllSoldiersArray[i][0].destination_y ;
        AllSoldiersArray[i][temp_c].destination_counter = AllSoldiersArray[i][0].destination_counter ;
        AllSoldiersArray[i][temp_c].owner_id = AllSoldiersArray[i][0].owner_id ;
        AllSoldiersArray[i][temp_c].power = AllSoldiersArray[i][0].power ;
        AllSoldiersArray[i][temp_c].soldier_id = temp_c+1 ;
        AllSoldiersArray[i][temp_c].num_of_all_soldiers = AllSoldiersArray[i][0].num_of_all_soldiers ;
        AllSoldiersArray[i][temp_c].origin_x = AllSoldiersArray[i][0].origin_x ;
        AllSoldiersArray[i][temp_c].origin_y = AllSoldiersArray[i][0].origin_y ;
    }

    map_arr[Origin_counter].soldiers_number = 0  ;
}


void ShowLinesOfSoldiers ( SDL_Renderer* sdlRenderer , OneSoldier** AllSoldiersArray , int HEXAGON_A , land* map_arr )
{
    for ( int i=0 ; i<50 ; i++)
    {
        // to check only used line of soldiers
        if ( AllSoldiersArray[i] != 0 || AllSoldiersArray[i] != NULL )
        {
            for ( int temp_c=0 ; temp_c<AllSoldiersArray[i][0].num_of_all_soldiers ; temp_c++ ) {
                if ( AllSoldiersArray[i][temp_c].x >= -999) {
                    AllSoldiersArray[i][temp_c].x += AllSoldiersArray[i][0].horizontalSpeed;
                    AllSoldiersArray[i][temp_c].y += AllSoldiersArray[i][0].verticalSpeed;

                    // drawing soldiers on screen if they are actually out of their land
                    // or if they have any power (A.K.A power != 0 )
                    int r = AllSoldiersArray[i][temp_c].power * 2 ;
                    int should_draw = 1 ;
                    if ( AllSoldiersArray[i][0].horizontalSpeed>0 && AllSoldiersArray[i][temp_c].x<AllSoldiersArray[i][temp_c].origin_x )
                        should_draw = 0 ;
                    else if ( AllSoldiersArray[i][0].horizontalSpeed<0 && AllSoldiersArray[i][temp_c].x>AllSoldiersArray[i][temp_c].origin_x)
                        should_draw = 0 ;
                    if ( AllSoldiersArray[i][0].verticalSpeed>0 && AllSoldiersArray[i][temp_c].y<AllSoldiersArray[i][temp_c].origin_y)
                        should_draw = 0 ;
                    else if ( AllSoldiersArray[i][0].verticalSpeed<0 && AllSoldiersArray[i][temp_c].y>AllSoldiersArray[i][temp_c].origin_y)
                        should_draw = 0 ;
                    if ( should_draw )
                        filledCircleColor(sdlRenderer, AllSoldiersArray[i][temp_c].x,AllSoldiersArray[i][temp_c].y
                                          , 2 * r, MAP_HANDLING_COLORS[AllSoldiersArray[i][0].owner_id]);

                    // conflict checker
                    if (fabs(AllSoldiersArray[i][temp_c].x-AllSoldiersArray[i][temp_c].destination_x)/HEXAGON_A <= 0.5 &&
                        fabs(AllSoldiersArray[i][temp_c].y-AllSoldiersArray[i][temp_c].destination_y)/HEXAGON_A <= 0.5)
                    {
                        AllSoldiersArray[i][temp_c].x = -1001;
                        // fighting between two rivals
                        if (map_arr[AllSoldiersArray[i][temp_c].destination_counter].owner_id != AllSoldiersArray[i][temp_c].owner_id)
                        {
                            map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number -= AllSoldiersArray[i][temp_c].power;
                            if ( map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number < 0 ) {
                                map_arr[AllSoldiersArray[i][temp_c].destination_counter].owner_id = AllSoldiersArray[i][temp_c].owner_id;
                                map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number *= -1;
                            }
                        }

                        // joining to friendly forces
                        else
                            map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number += AllSoldiersArray[i][temp_c].power;

                        // checking last soldier arrival to destination
                        if ( AllSoldiersArray[i][temp_c].soldier_id == AllSoldiersArray[i][temp_c].num_of_all_soldiers )
                        {
                            AllSoldiersArray[i] = 0;
                            return;
                        }
                    }
                }
            }
        }
    }
}
