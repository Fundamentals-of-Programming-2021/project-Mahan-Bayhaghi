//
// Created by Mahan on 1/31/2022.
//

#include "map_handling.h"
#include "background_handling.h"

Uint32 MAP_HANDLING_COLORS[6] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffb00500 , 0xff0059ff , 0xff00ffff};
                                    // grey          // blue          // green        // red        // orange       // yellow

float MAIN_SPEED = 6.0 ;

int ShowHexagonBackground ( SDL_Renderer* sdlRenderer , land* map , int counter , Sint16 a)
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

        filledPolygonColor(sdlRenderer , vx , vy , 6 , MAP_HANDLING_COLORS[map[i].owner_id] ) ;
        polygonColor(sdlRenderer , vx , vy , 6 , 0xffffffff) ;

        char* soldiers_number = malloc(sizeof(char) * 4) ;
        sprintf(soldiers_number , "%3.0f" , map[i].soldiers_number) ;
        stringColor(sdlRenderer , center_x-12 , center_y+5 , soldiers_number , 0x22000000) ;

    }
}

land* GENERATE_HEXAGON_RANDOM_MAP (int NUM_PLAYERS , int NUM_COLS , int NUM_ROWS , land* map , int HEXAGON_a)
{
    int* ownership_watcher = calloc( NUM_PLAYERS , sizeof(int)) ;
    srand(time(0)) ;
    Sint16 start_x_pos = rand()%40 + 40 ;
    Sint16 start_y_pos = rand()%40 + 100 ;

    Sint16 center_x ;
    Sint16 center_y = start_y_pos ;

    int counter = 0 ;

    for ( int i=0 ; i<NUM_ROWS ; i++)
    {
        if ( i%2==0 )
            center_x = start_x_pos ;
        else
            center_x = start_x_pos+ 1.5*HEXAGON_a ;

        for ( int j=0 ; j<NUM_COLS/2 ; j++) {
            int num_rand = rand() ;
            if ( num_rand%7 == 0 || num_rand%5 == 0 || num_rand%8 == 0 || num_rand%3 == 0 )
            {
                map[counter].x = center_x ;
                map[counter].y = center_y ;

                if ( num_rand%7 == 0 || num_rand%10 == 0 || num_rand%16 == 0 )   // USER or SYSTEM land //
                {
                    map[counter].owner_id = rand()%(NUM_PLAYERS-1) + 1;
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

    for ( int i=1 ; i<NUM_PLAYERS ; i++)
    {
        while ( ownership_watcher[i] < 4 )
        {
            int temp_counter = 0 ;
            while ( map[temp_counter].owner_id != 0 && temp_counter<counter )
                temp_counter++ ;

            map[temp_counter].owner_id = i ;
            map[temp_counter].soldiers_number = 25 ;
            map[temp_counter].production_rate = 2 ;
            ownership_watcher[i] += 1 ;
        }
    }

    return  map ;
}

void AddSoldiers ( land* map , int counter , int* PRODUCTION_RATE_ARRAY )
{
    for ( int i=0 ; i<counter ; i++)
    {
        if ( map[i].soldiers_number < 120 && map[i].owner_id != 0 ) {
            map[i].soldiers_number += PRODUCTION_RATE_ARRAY[map[i].owner_id];
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


void CreateLineOfSoldiers ( OneSoldier** AllSoldiersArray , land* map_arr , int Origin_counter , land Destination_cell_info
                            , float* SOLDIERS_POWER_ARRAY )
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
    AllSoldiersArray[i][0].owner_id = map_arr[Origin_counter].owner_id ;
    AllSoldiersArray[i][0].power = SOLDIERS_POWER_ARRAY[AllSoldiersArray[i][0].owner_id] ;
    AllSoldiersArray[i][0].soldier_id = 1 ;
    AllSoldiersArray[i][0].num_of_all_soldiers = map_arr[Origin_counter].soldiers_number ;

    // angel management //
    double delta_y =  Destination_cell_info.y - (map_arr[Origin_counter].y) ;
    double delta_x = Destination_cell_info.x - (map_arr[Origin_counter].x) ;
    double theta = delta_y / delta_x ;

    AllSoldiersArray[i][0].verticalSpeed = MAIN_SPEED * sin(atan(theta) ) ;
    if ( delta_y > 0 )
        AllSoldiersArray[i][0].verticalSpeed = fabs(AllSoldiersArray[i][0].verticalSpeed) ;
    else if ( delta_y <0 )
        AllSoldiersArray[i][0].verticalSpeed = -1 * fabs(AllSoldiersArray[i][0].verticalSpeed) ;
    AllSoldiersArray[i][0].horizontalSpeed = MAIN_SPEED * cos(atan(theta) ) ;
    if ( delta_x > 0 )
        AllSoldiersArray[i][0].horizontalSpeed = fabs(AllSoldiersArray[i][0].horizontalSpeed) ;
    else if ( delta_x <0 )
        AllSoldiersArray[i][0].horizontalSpeed = -1 * fabs(AllSoldiersArray[i][0].horizontalSpeed) ;

    AllSoldiersArray[i][0].x = map_arr[Origin_counter].x ;
    AllSoldiersArray[i][0].y = map_arr[Origin_counter].y ;

    int c = map_arr[Origin_counter].soldiers_number ;
    for ( int temp_c = 1 ; temp_c<c ; temp_c++ )
    {
        AllSoldiersArray[i][temp_c].x = AllSoldiersArray[i][temp_c-1].x - 1.6*AllSoldiersArray[i][0].horizontalSpeed ;
        AllSoldiersArray[i][temp_c].y = AllSoldiersArray[i][temp_c-1].y - 1.6*AllSoldiersArray[i][0].verticalSpeed ;
        AllSoldiersArray[i][temp_c].destination_x = AllSoldiersArray[i][0].destination_x ;
        AllSoldiersArray[i][temp_c].destination_y = AllSoldiersArray[i][0].destination_y ;
        AllSoldiersArray[i][temp_c].destination_counter = AllSoldiersArray[i][0].destination_counter ;
        AllSoldiersArray[i][temp_c].owner_id = AllSoldiersArray[i][0].owner_id ;
        AllSoldiersArray[i][temp_c].power = SOLDIERS_POWER_ARRAY[AllSoldiersArray[i][0].owner_id] ;
        AllSoldiersArray[i][temp_c].soldier_id = temp_c+1 ;
        AllSoldiersArray[i][temp_c].num_of_all_soldiers = AllSoldiersArray[i][0].num_of_all_soldiers ;
        AllSoldiersArray[i][temp_c].origin_x = AllSoldiersArray[i][0].origin_x ;
        AllSoldiersArray[i][temp_c].origin_y = AllSoldiersArray[i][0].origin_y ;
    }

    map_arr[Origin_counter].soldiers_number = 0  ;
}


int ShouldConsiderSoldier ( OneSoldier soldier_data , float verticalSpeed , float horizontalSpeed)
{
    if ( horizontalSpeed>0 && soldier_data.x<soldier_data.origin_x )
        return 0;
    else if ( horizontalSpeed<0 && soldier_data.x>soldier_data.origin_x )
        return 0;

    else if ( verticalSpeed>0 && soldier_data.y<soldier_data.origin_y )
        return 0;

    else if ( verticalSpeed<0 && soldier_data.y>soldier_data.origin_y )
        return 0;

    return 1 ;
}

void ShowLinesOfSoldiers ( SDL_Renderer* sdlRenderer , OneSoldier** AllSoldiersArray , int HEXAGON_A , land* map_arr
                            , float* SPEED_ARRAY )
{
    for ( int i=0 ; i<50 ; i++)
    {
        // to check only used line of soldiers
        if ( AllSoldiersArray[i] != 0 || AllSoldiersArray[i] != NULL )
        {
            for ( int temp_c=0 ; temp_c<AllSoldiersArray[i][0].num_of_all_soldiers ; temp_c++ ) {
                if ( AllSoldiersArray[i][temp_c].x >= -9999) {
                    AllSoldiersArray[i][temp_c].x += AllSoldiersArray[i][0].horizontalSpeed * SPEED_ARRAY[AllSoldiersArray[i][0].owner_id] ;
                    AllSoldiersArray[i][temp_c].y += AllSoldiersArray[i][0].verticalSpeed * SPEED_ARRAY[AllSoldiersArray[i][0].owner_id] ;

                    // drawing soldiers on screen if they are actually out of their land
                    // or if they have any power (A.K.A power != 0 )
                    float r = fabs(AllSoldiersArray[i][temp_c].power) * 1.5 ;
                    int should_draw = ShouldConsiderSoldier(AllSoldiersArray[i][temp_c] , AllSoldiersArray[i][0].verticalSpeed ,
                                                            AllSoldiersArray[i][0].horizontalSpeed) ;

                    if ( should_draw && r != 0 ) {
                        aacircleColor(sdlRenderer, AllSoldiersArray[i][temp_c].x, AllSoldiersArray[i][temp_c].y,
                                      2 * r ,0xffffffff);
                        filledCircleColor(sdlRenderer, AllSoldiersArray[i][temp_c].x, AllSoldiersArray[i][temp_c].y,
                                          2 * r, MAP_HANDLING_COLORS[AllSoldiersArray[i][0].owner_id]);
                        filledCircleColor(sdlRenderer, AllSoldiersArray[i][temp_c].x, AllSoldiersArray[i][temp_c].y,
                                          2 * r, 0x0fffffff);
                    }

                    // conflict checker
                    if (fabs(AllSoldiersArray[i][temp_c].x-AllSoldiersArray[i][temp_c].destination_x)/HEXAGON_A <= 0.5 &&
                        fabs(AllSoldiersArray[i][temp_c].y-AllSoldiersArray[i][temp_c].destination_y)/HEXAGON_A <= 0.5)
                    {
                        AllSoldiersArray[i][temp_c].x = -10001;
                        // fighting between two rivals
                        if (map_arr[AllSoldiersArray[i][temp_c].destination_counter].owner_id != AllSoldiersArray[i][temp_c].owner_id)
                        {
                            map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number -= AllSoldiersArray[i][temp_c].power ;
                            if ( map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number < 0 ) {
                                map_arr[AllSoldiersArray[i][temp_c].destination_counter].owner_id = AllSoldiersArray[i][temp_c].owner_id;
                                // need
                                map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number = 1;
                            }
                        }

                        // joining to friendly forces
                        else if ( AllSoldiersArray[i][temp_c].power != 0 )
                            map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number += 1 ;

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


void UpdateMapInfo(land *map_arr , int NUM_OF_CELLS , int* CELLS_OWNED , int** LANDS_OWNED_COUNTERS , int NUM_PLAYERS)
{

    for ( int i=0 ; i<NUM_PLAYERS ; i++)
        CELLS_OWNED[i] = 0 ;

    for ( int i=0 ; i<NUM_OF_CELLS ; i++)
    {
        CELLS_OWNED[map_arr[i].owner_id] += 1 ;
        LANDS_OWNED_COUNTERS[ map_arr[i].owner_id ][ CELLS_OWNED[map_arr[i].owner_id]-1 ] = map_arr[i].counter ;
    }
}


void SystemMakeMovement ( int owner_id , OneSoldier** AllSoldiersArray , land* map_arr
        , int* CELLS_OWNED , int** LANDS_OWNED_COUNTERS , int NUM_OF_PLAYERS
        , float* SOLDIERS_POWER_ARRAY , int* IMMUNE_LANDS_ARRAY )
{
    srand(time(0)) ;

    if ( CELLS_OWNED[owner_id] == 0 )   // already lost
        return;

    else if ( rand() % 2 == 0 )
    {
        int Origin_counter =LANDS_OWNED_COUNTERS[owner_id][rand()%CELLS_OWNED[owner_id]] ;
        int Opponent_id = rand()%NUM_OF_PLAYERS ;
        while ( Opponent_id == owner_id )
            Opponent_id = rand()%NUM_OF_PLAYERS ;

        if ( CELLS_OWNED[Opponent_id] == 0 )
            return;

        int Opponent_counter = LANDS_OWNED_COUNTERS[Opponent_id][rand()%CELLS_OWNED[owner_id]] ;
        land destination = map_arr[Opponent_counter] ;

        if ( map_arr[Origin_counter].soldiers_number > 5 + map_arr[Opponent_counter].soldiers_number
             && IMMUNE_LANDS_ARRAY[Opponent_id] != 1 )
            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , destination , SOLDIERS_POWER_ARRAY ) ;
        else
            return;
    }
}


int CheckWinState ( int* CELLS_OWNED , int NUM_PLAYERS )
{
    int number_of_zeroes = 0 ;
    for ( int i=1 ; i<NUM_PLAYERS ; i++)
    {
        if ( CELLS_OWNED[i] == 0 )
            number_of_zeroes += 1 ;
    }
    return (number_of_zeroes == (NUM_PLAYERS-1) ) ;
}


void SoldierConflictSolver ( OneSoldier** AllSoldiersArray , float* SOLDIERS_POWER_ARRAY)
{
    for ( int i=0 ; i<50 ; i++ )
    {
        if ( AllSoldiersArray[i] != NULL && AllSoldiersArray[i] != 0 )
        {
            for ( int j=i+1 ; j<50 ; j++)
            {
                if ( (AllSoldiersArray[j] != NULL || AllSoldiersArray[j] != 0)  &&
                     AllSoldiersArray[i][0].owner_id != AllSoldiersArray[j][0].owner_id )
                {
                    for ( int from_first = 0 ; from_first<AllSoldiersArray[i][0].num_of_all_soldiers ; from_first++)
                    {
                        for ( int from_second = 0 ; from_second<AllSoldiersArray[j][0].num_of_all_soldiers ; from_second++)
                        {
                            if ( fabs(AllSoldiersArray[i][from_first].x - AllSoldiersArray[j][from_second].x)<5 &&
                                 fabs(AllSoldiersArray[i][from_first].y - AllSoldiersArray[j][from_second].y)<5
                                 && ShouldConsiderSoldier(AllSoldiersArray[i][from_first] , AllSoldiersArray[i][0].verticalSpeed ,
                                                          AllSoldiersArray[i][0].horizontalSpeed)
                                 && ShouldConsiderSoldier(AllSoldiersArray[j][from_second] , AllSoldiersArray[j][0].verticalSpeed ,
                                                          AllSoldiersArray[j][0].horizontalSpeed) )
                            {
                                float f = SOLDIERS_POWER_ARRAY[AllSoldiersArray[i][from_first].owner_id] ;
                                float s = SOLDIERS_POWER_ARRAY[AllSoldiersArray[j][from_second].owner_id] ;
                                AllSoldiersArray[i][from_first].power -= s ;
                                AllSoldiersArray[j][from_second].power -= f ;
                                AllSoldiersArray[i][from_first].power = AllSoldiersArray[i][from_first].power<0 ? 0 : AllSoldiersArray[i][from_first].power ;
                                AllSoldiersArray[j][from_second].power = AllSoldiersArray[j][from_second].power<0 ? 0 : AllSoldiersArray[j][from_second].power ;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}


Potion CreatePotion ( int WIDTH , int HEIGHT )
{
    srand(time(0)) ;

    Potion returning_potion ;
    if ( rand() % 2 == 0 )
    {
        returning_potion.x = rand()%WIDTH + 50 ;
        while ( returning_potion.x < 100 || returning_potion.x > 500 )
            returning_potion.x = rand()%WIDTH + 50 ;
        returning_potion.y = rand()%HEIGHT + 80 ;
        while ( returning_potion.y < 100 || returning_potion.y > 400 )
            returning_potion.y = rand()%HEIGHT + 50 ;

        returning_potion.potion_id = rand()%6 ;
        return returning_potion ;
    }

    returning_potion.potion_id = -1 ;
    return returning_potion ;
}


void CheckForSoldierPotionConflict ( OneSoldier** AllSoldiersArray , Potion *live_time_potion
        , OnePotionEffect* AllPotionsEffect )
{
    if ( live_time_potion->potion_id == -1 )
        return;

    for ( int i=0 ; i<50 ; i++)
    {
        if ( AllSoldiersArray[i] != 0 || AllSoldiersArray[i] != NULL )
        {
            for ( int sol_counter=0 ; sol_counter<AllSoldiersArray[i][0].num_of_all_soldiers ; sol_counter++ )
            {
                if ( fabs(AllSoldiersArray[i][sol_counter].x - live_time_potion->x) <= 20 &&
                     fabs(AllSoldiersArray[i][sol_counter].y - live_time_potion->y) <= 20)
                {
                    if ( CreatePotionEffect( *live_time_potion , AllPotionsEffect , AllSoldiersArray[i][sol_counter].owner_id ) == 1 )
                    {
                        live_time_potion->potion_id = -1 ;
                        return;
                    }
                }
            }
        }
    }
}


int CreatePotionEffect ( Potion potion_info , OnePotionEffect* AllPotionsArray , int Destination_owner )
{
    if ( AllPotionsArray[Destination_owner].potion_id == -1 )   // should create owner
    {
        AllPotionsArray[Destination_owner].potion_id = potion_info.potion_id ;
        switch (potion_info.potion_id) {
            case 0 :    // 2X SPEED
            case 1 :    // 0.5X SPEED
                AllPotionsArray[Destination_owner].time_to_exist = 600 ;
                break;
            case 2 :    // 0.5X POWER
            case 3 :    // 2X POWER
                AllPotionsArray[Destination_owner].time_to_exist = 500 ;
                break;
            case 4 :    // GOLDEN POTION
            case 5 :    // SILVER POTION
                AllPotionsArray[Destination_owner].time_to_exist = 300 ;
                break;
            case 6:
            case 7:
                AllPotionsArray[Destination_owner].time_to_exist = 400 ;
                break;
        }
        AllPotionsArray[Destination_owner].owner_id = Destination_owner ;
        return 1 ;
    }
    return 0 ;
}

void UpdatePotionEffectArray ( OnePotionEffect* AllPotionsEffect , int NUM_PLAYERS )
{
    for ( int i=0 ; i<NUM_PLAYERS ; i++)
    {
        if ( AllPotionsEffect[i].potion_id != -1 )
            AllPotionsEffect[i].time_to_exist -= 1 ;

        if ( AllPotionsEffect[i].time_to_exist == 0 )
            AllPotionsEffect[i].potion_id = -1 ;
    }
}

void ApplyPotionEffect ( OnePotionEffect* AllPotionsEffect , float* SPEED_ARRAY, float* SOLDIERS_POWER_ARRAY
                         , int* PRODUCTION_RATE_ARRAY , int* IMMUNE_LANDS_ARRAY , int NUM_PLAYERS)
{
    int global_act = 0 ;
    for ( int i=0 ; i<NUM_PLAYERS ; i++)
    {
        if ( AllPotionsEffect[i].potion_id != -1 ) {
            switch ( AllPotionsEffect[i].potion_id ) {
                case 0 :    // 2X SPEED     // RED POTION
                    SPEED_ARRAY[i] = 2 ;
                    break;
                case 1 :    // 0.5X SPEED   // RED POTION
                    SPEED_ARRAY[i] = 0.5 ;
                    break;
                case 2 :    // 0.5X POWER   // BLUE POTION
                    SOLDIERS_POWER_ARRAY[i] = 0.5 ;
                    break;
                case 3 :    // 2X POWER
                    SOLDIERS_POWER_ARRAY[i] = 2 ;   // ORANGE POTION
                    break;
                case 4 :    // 0X SPEED for all enemies     // GOLD POTION
                    for ( int temp=1 ; temp<NUM_PLAYERS ; temp++ )
                    {
                        if ( temp != i && AllPotionsEffect[temp].potion_id == -1 )
                            SPEED_ARRAY[temp] = 0 ;
                    }
                    global_act = 1 ;
                    break;
                case 5 :    // 0.5X SPEED for all enemies   // SILVER POTION
                    for ( int temp=1 ; temp<NUM_PLAYERS ; temp++ )
                    {
                        if ( temp != i && AllPotionsEffect[temp].potion_id == -1 )
                            SPEED_ARRAY[temp] = 0.5 ;
                    }
                    global_act = 1 ;
                    break;
                case 6 :    // 2X PRODUCTION  SPEED     // GREEN POTION
                    PRODUCTION_RATE_ARRAY[i] = 4 ;
                    break;
                case 7 :    // Immunity against attack  // SHIELD POTION
                    IMMUNE_LANDS_ARRAY[i] = 1 ;
                    break;
            }
        }
        else if ( global_act != 1 ){
            SPEED_ARRAY[i] = 1;
            SOLDIERS_POWER_ARRAY[i] = 1;
            IMMUNE_LANDS_ARRAY[i] = 0 ;
            PRODUCTION_RATE_ARRAY[i] = 2 ;
        }
    }
}


void DisplayPotionsEffect ( SDL_Renderer* sdlRenderer , OnePotionEffect* AllPotionsEffect , SDL_Texture* POTION_GRAPHIC[8]
, int NUM_PLAYERS)
{
    char* EFFECTS[8] = { "SUPER FAST !" , "HALF SPEED" , "HALF POWER" , "SUPER STRONG !"
                         , "JUST WOW !" , "GOOD CHANCE !" , "HARDWORKING" , "IMMUNE"} ;
    int y_to_write = 20 ;
    for ( int i=0 ; i<NUM_PLAYERS; i++)
    {
        if ( AllPotionsEffect[i].potion_id != -1 )
        {
            SDL_Rect potion_rect = { .x=200 , .y=y_to_write , .w=10 , .h=10 } ;
            SDL_RenderCopy(sdlRenderer , POTION_GRAPHIC[AllPotionsEffect[i].potion_id] , NULL ,  &potion_rect) ;
            Sint16 vx[4] = { 220 , 220+AllPotionsEffect[i].time_to_exist , 220+AllPotionsEffect[i].time_to_exist , 220  } ;
            Sint16 vy[4] = { y_to_write , y_to_write , y_to_write+10 , y_to_write+10  };
            filledPolygonColor(sdlRenderer , vx , vy , 4 , MAP_HANDLING_COLORS[AllPotionsEffect[i].owner_id]) ;
            stringColor(sdlRenderer , 220+AllPotionsEffect[i].time_to_exist + 10 , 20 , EFFECTS[AllPotionsEffect[i].potion_id] , 0xff000000) ;
            y_to_write += 20 ;
        }
    }
}


void AimAssist ( SDL_Renderer* sdlRenderer , SDL_Event sdlEvent
                 , land* map_arr , int NUM_OF_CELLS , int HEXAGON_A , int Origin_x , int Origin_y )
{
    land mouse_cell = GiveClickedCellInfo(sdlEvent.motion.x , sdlEvent.motion.y , map_arr , NUM_OF_CELLS , HEXAGON_A ) ;
    SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0x99);
    if ( mouse_cell.owner_id != -1 ) {
        SDL_RenderDrawLine(sdlRenderer, Origin_x, Origin_y, mouse_cell.x, mouse_cell.y);
        filledCircleColor(sdlRenderer , mouse_cell.x , mouse_cell.y , 2 , 0x99000000) ;
    }
    else if ( sdlEvent.motion.x != 0 && sdlEvent.motion.y != 0)
        SDL_RenderDrawLine(sdlRenderer, Origin_x, Origin_y, sdlEvent.motion.x , sdlEvent.motion.y);
}

void RenderPotion ( SDL_Renderer *sdlRenderer , Potion live_time_potion , SDL_Texture** POTION_GRAPHIC)
{
    if ( live_time_potion.potion_id == -1 )
        return;
    SDL_Rect potion_rect = {.x = live_time_potion.x - 20 , .y = live_time_potion.y - 20 , .w=40 , .h=40} ;
    SDL_RenderCopy(sdlRenderer, POTION_GRAPHIC[live_time_potion.potion_id], NULL, &potion_rect);
}
