//
// Created by Mahan on 1/31/2022.
//

#include "map_handling.h"
#include "background_handling.h"

Uint32 MAP_HANDLING_COLORS[6] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffb00500 , 0xff0059ff , 0xff00ffff};
                                    // grey          // blue          // green        // red        // orange       // yellow

float MAIN_SPEED = 6.0 ;


//////////////////// functions needed for game itself //////////////////////////
//////////////////// mostly logic of game and map //////////////////////////////
int ShowHexagonBackground ( SDL_Renderer* sdlRenderer , land* map , int counter , Sint16 a , SDL_Texture* castle_texture)
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
        if ( map[i].IS_MILITARY ) {
            SDL_Rect castle_rect = {.x = center_x - 10, .y=center_y - 20, .w=20, .h=20};
            SDL_RenderCopy(sdlRenderer, castle_texture, NULL, &castle_rect);
        }

        char* soldiers_number = malloc(sizeof(char) * 4) ;
        sprintf(soldiers_number , "%3.0f" , map[i].soldiers_number) ;
        stringColor(sdlRenderer , center_x-12 , center_y+5 , soldiers_number , 0x22ffffff) ;
    }
}

void GoUp ( const int counter , const int AllCounter , land* map_arr , int HEXAGON_a )
{
    int temp = counter ;
    int num ;
    int last_y = map_arr[counter].y ;
    while ( temp >= 0 )
    {
        if ( map_arr[temp].y < map_arr[counter].y &&
                (int) (fabs(map_arr[temp].x - map_arr[counter].x) )  < 1.7 * HEXAGON_a )
        {
            num = rand() % 4 ;
            if ( num != 2 && map_arr[temp].IS_MILITARY==0 && map_arr[temp].owner_id==0
                                                             && last_y - map_arr[temp].y <= HEXAGON_a )
            {
                map_arr[temp].owner_id = map_arr[counter].owner_id ;
                map_arr[temp].RELATED_MILITARY_COUNTER = counter ;
                map_arr[temp].soldiers_number = 25 ;
                last_y = map_arr[temp].y ;
            }
            else
                break;
        }
        temp-- ;
    }
}

void GoDown  ( const int counter , const int AllCounter , land* map_arr , int HEXAGON_a )
{
    int temp = counter ;
    int num ;
    int last_y = map_arr[counter].y ;
    while ( temp < AllCounter )
    {
        if ( map_arr[temp].y > map_arr[counter].y &&
                (int) (fabs(map_arr[temp].x - map_arr[counter].x)) < 1.7 * HEXAGON_a  )
        {
            num = rand() % 4 ;
            if ( num != 2 && map_arr[temp].IS_MILITARY==0 && map_arr[temp].owner_id==0
                 && map_arr[temp].y - last_y <= HEXAGON_a )
            {
                map_arr[temp].owner_id = map_arr[counter].owner_id ;
                map_arr[temp].RELATED_MILITARY_COUNTER = counter ;
                map_arr[temp].soldiers_number = 25 ;
                last_y = map_arr[temp].y ;
            }
            else
                break;
        }
        temp++ ;
    }
}

land* GENERATE_HEXAGON_RANDOM_MAP (int NUM_PLAYERS , int NUM_COLS , int NUM_ROWS , land* map , int HEXAGON_a)
{
    int* ownership_watcher = calloc( NUM_PLAYERS , sizeof(int)) ;
    Sint16 start_x_pos = rand()%40 + 40 ;
    Sint16 start_y_pos = rand()%40 + 100 ;

    Sint16 center_x ;
    Sint16 center_y = start_y_pos ;

    int counter = 0 ;
    // creating a simple random map with no ownership data
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
                map[counter].soldiers_number = 25 ;
                map[counter].IS_MILITARY = 0 ;
                map[counter].counter = counter ;
                map[counter].owner_id = 0 ;
                map[counter].RELATED_MILITARY_COUNTER = -1 ;
                counter++;
            }
            center_x += 3*HEXAGON_a ;
        }
        center_y += (Sint16) (sqrt(3)*HEXAGON_a/2) ;
    }

    // filling up the map MILITARY BASES
    int* MILITARY_BASE_WATCHER = calloc(NUM_PLAYERS , sizeof(int)) ;
    int num ;
    for ( int i=0 ; i<counter ; i++)
    {
        if (rand() % 5 == 0 ) {
            num = rand()%NUM_PLAYERS ;
            if ( MILITARY_BASE_WATCHER[num] <=3 ) {
                map[i].owner_id = num;
                MILITARY_BASE_WATCHER[num] += 1;
                map[i].IS_MILITARY = 1;
                map[i].soldiers_number = 25;
            }
        }
    }
    for ( int i=0 ; i<NUM_PLAYERS ; i++) {
        do {
            num = rand() % counter;
            if (map[num].IS_MILITARY == 0) {
                map[num].IS_MILITARY = 1;
                map[num].owner_id = i;
                map[num].soldiers_number = 25;
                MILITARY_BASE_WATCHER[i] += 1;
            }
        } while (MILITARY_BASE_WATCHER[i] <= 2);
    }

    // creating colony around each MILITARY BASE
    for ( int i=0 ; i<counter ; i++ )
    {
        if ( map[i].IS_MILITARY )
        {
            if ( i%2 == 0 ) {
                GoDown(i, counter, map, HEXAGON_a);
                GoUp(i, counter, map, HEXAGON_a);
            }
            else {
                GoUp(i , counter , map , HEXAGON_a) ;
                GoDown(i , counter , map , HEXAGON_a) ;
            }
        }
        else
            continue;
    }
    return map ;
}


void AddSoldiers ( land* map , int counter , int* PRODUCTION_RATE_ARRAY )
{
    for ( int i=0 ; i<counter ; i++)
    {
        if (map[i].soldiers_number >= 120 )
            return;

        if ( map[i].IS_MILITARY && map[i].owner_id != 0)
            map[i].soldiers_number += PRODUCTION_RATE_ARRAY[map[i].owner_id];
        else if ( map[i].IS_MILITARY == 0 && map[i].RELATED_MILITARY_COUNTER != -1 &&
                  map[map[i].RELATED_MILITARY_COUNTER].owner_id == map[i].owner_id && map[i].owner_id != 0 )
            map[i].soldiers_number += PRODUCTION_RATE_ARRAY[map[i].owner_id] ;

        map[i].soldiers_number = map[i].soldiers_number > 120 ? 120 : map[i].soldiers_number ;
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
        fprintf(stderr , "Couldn't allocate memory for soldiers . Trying again \n") ;
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

    for ( int temp_c = 1 ; temp_c<map_arr[Origin_counter].soldiers_number ; temp_c++ )
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

void ShowLinesOfSoldiers ( SDL_Renderer* sdlRenderer , OneSoldier** AllSoldiersArray
                           , int HEXAGON_A , land* map_arr , float* SPEED_ARRAY )
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
                    float r = AllSoldiersArray[i][temp_c].power * 1.5 ;
                    int should_draw = ShouldConsiderSoldier(AllSoldiersArray[i][temp_c] , AllSoldiersArray[i][0].verticalSpeed ,
                                                            AllSoldiersArray[i][0].horizontalSpeed) ;

                    if ( should_draw && r != 0 ) {
                        filledCircleColor(sdlRenderer, AllSoldiersArray[i][temp_c].x, AllSoldiersArray[i][temp_c].y,
                                          2.5 * r, MAP_HANDLING_COLORS[AllSoldiersArray[i][0].owner_id]);
                        filledCircleColor(sdlRenderer, AllSoldiersArray[i][temp_c].x, AllSoldiersArray[i][temp_c].y,
                                          2.5 * r, 0x044000000);
                    }

                    // conflict checker
                    if (fabs(AllSoldiersArray[i][temp_c].x-AllSoldiersArray[i][temp_c].destination_x)/HEXAGON_A <= 0.3 &&
                        fabs(AllSoldiersArray[i][temp_c].y-AllSoldiersArray[i][temp_c].destination_y)/HEXAGON_A <= 0.3)
                    {
                        AllSoldiersArray[i][temp_c].x = -10001;
                        // fighting between two rivals
                        if (map_arr[AllSoldiersArray[i][temp_c].destination_counter].owner_id != AllSoldiersArray[i][temp_c].owner_id)
                        {
                            map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number -= AllSoldiersArray[i][temp_c].power ;
                            if ( map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number < 0 ) {
                                map_arr[AllSoldiersArray[i][temp_c].destination_counter].owner_id = AllSoldiersArray[i][temp_c].owner_id;
                                map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number *= -1 ;
                            }
                        }

                        // joining to friendly forces
                        else if ( AllSoldiersArray[i][temp_c].power != 0 )
                            map_arr[AllSoldiersArray[i][temp_c].destination_counter].soldiers_number += 1 ;

                        // checking last soldier arrival to destination
                        if ( AllSoldiersArray[i][temp_c].soldier_id == AllSoldiersArray[i][temp_c].num_of_all_soldiers )
                        {
                            AllSoldiersArray[i] = NULL ;
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
    if ( CELLS_OWNED[owner_id] == 0 )   // already lost
        return;

    else
    {
        int Origin_counter =LANDS_OWNED_COUNTERS[owner_id][rand()%CELLS_OWNED[owner_id]] ;
        int Opponent_id = rand()%NUM_OF_PLAYERS ;
        while ( Opponent_id == owner_id )
            Opponent_id = rand()%NUM_OF_PLAYERS ;

        if ( CELLS_OWNED[Opponent_id] == 0 )
            return;

        int Opponent_counter = LANDS_OWNED_COUNTERS[Opponent_id][rand()%CELLS_OWNED[owner_id]] ;
        if ( rand()%2 == 0 )
        {
            int tries = 0 ;
            while ( !map_arr[Opponent_counter].IS_MILITARY &&  tries<CELLS_OWNED[owner_id] ) {
                Opponent_counter = LANDS_OWNED_COUNTERS[Opponent_id][rand() % CELLS_OWNED[owner_id]];
                tries += 1;
            }
        }

        land destination = map_arr[Opponent_counter] ;

        if ( map_arr[Origin_counter].soldiers_number > 5 + map_arr[Opponent_counter].soldiers_number
             && IMMUNE_LANDS_ARRAY[Opponent_id] != 1 )
            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , destination , SOLDIERS_POWER_ARRAY ) ;

    }
}


int CheckWinState ( int* CELLS_OWNED , int NUM_PLAYERS )
{
    int number_of_zeroes = 0 ;
    for ( int i=1 ; i<NUM_PLAYERS ; i++)
        number_of_zeroes += (CELLS_OWNED[i] == 0) ;

    if ( number_of_zeroes == (NUM_PLAYERS-2) )
    {
        for ( int i=1 ; i<NUM_PLAYERS ; i++)
        {
            if ( CELLS_OWNED[i] != 0 )
                return  i ;
        }
    }
    return -1 ;
}

void SoldierConflictSolver ( OneSoldier** AllSoldiersArray )
{
    for ( int i=0 ; i<50 ; i++ )
    {
        if ( AllSoldiersArray[i] != NULL && AllSoldiersArray[i] != 0 )
        {
            for ( int j=i+1 ; j<50 ; j++)
            {
                if ( AllSoldiersArray[j] != NULL && AllSoldiersArray[j] != 0  &&
                     AllSoldiersArray[i][0].owner_id != AllSoldiersArray[j][0].owner_id )
                {
                    for ( int from_first = 0 ; from_first<AllSoldiersArray[i][0].num_of_all_soldiers ; from_first++)
                    {
                        for ( int from_second = 0 ; from_second<AllSoldiersArray[j][0].num_of_all_soldiers ; from_second++)
                        {
                            if ( fabs(AllSoldiersArray[i][from_first].x - AllSoldiersArray[j][from_second].x)<5 &&
                                 fabs(AllSoldiersArray[i][from_first].y - AllSoldiersArray[j][from_second].y)<5 &&
                                 ShouldConsiderSoldier(AllSoldiersArray[i][from_first] , AllSoldiersArray[i][0].verticalSpeed ,
                                                          AllSoldiersArray[i][0].horizontalSpeed)
                                 && ShouldConsiderSoldier(AllSoldiersArray[j][from_second] , AllSoldiersArray[j][0].verticalSpeed ,
                                                          AllSoldiersArray[j][0].horizontalSpeed))
                            {
                                if ( AllSoldiersArray[i][from_first].power == AllSoldiersArray[j][from_second].power ) {
                                    AllSoldiersArray[i][from_first].power = 0;
                                    AllSoldiersArray[j][from_second].power = 0;
                                }
                                else if ( AllSoldiersArray[i][from_first].power > AllSoldiersArray[j][from_second].power )
                                {
                                    AllSoldiersArray[i][from_first].power -= AllSoldiersArray[j][from_second].power ;
                                    AllSoldiersArray[j][from_second].power = 0;
                                }
                                else if ( AllSoldiersArray[i][from_first].power < AllSoldiersArray[j][from_second].power )
                                {
                                    AllSoldiersArray[j][from_second].power -= AllSoldiersArray[i][from_first].power ;
                                    AllSoldiersArray[i][from_first].power = 0;
                                }
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
        while ( returning_potion.x < 200 || returning_potion.x > 450 )
            returning_potion.x = rand()%WIDTH + 50 ;
        returning_potion.y = rand()%HEIGHT + 80 ;
        while ( returning_potion.y < 100 || returning_potion.y > 400 )
            returning_potion.y = rand()%HEIGHT + 50 ;

        returning_potion.potion_id = rand()%8 ;
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
                if (ShouldConsiderSoldier(AllSoldiersArray[i][sol_counter] , AllSoldiersArray[i][0].verticalSpeed , AllSoldiersArray[i][0].horizontalSpeed)
                   &&fabs(AllSoldiersArray[i][sol_counter].x - live_time_potion->x) <= 20
                   &&fabs(AllSoldiersArray[i][sol_counter].y - live_time_potion->y) <= 20)
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
    int x_to_write = 180 ;
    for ( int i=0 ; i<NUM_PLAYERS; i++)
    {
        if ( AllPotionsEffect[i].potion_id != -1 )
        {
            SDL_Rect potion_rect = { .x=x_to_write-20 , .y=y_to_write , .w=10 , .h=10 } ;
            SDL_RenderCopy(sdlRenderer , POTION_GRAPHIC[AllPotionsEffect[i].potion_id] , NULL ,  &potion_rect) ;
            Sint16 vx[4] = { x_to_write , x_to_write+AllPotionsEffect[i].time_to_exist , x_to_write+AllPotionsEffect[i].time_to_exist , x_to_write  } ;
            Sint16 vy[4] = { y_to_write , y_to_write , y_to_write+10 , y_to_write+10  };
            filledPolygonColor(sdlRenderer , vx , vy , 4 , MAP_HANDLING_COLORS[AllPotionsEffect[i].owner_id]) ;
            stringColor(sdlRenderer , x_to_write+AllPotionsEffect[i].time_to_exist + 10 , y_to_write , EFFECTS[AllPotionsEffect[i].potion_id] , 0xff000000) ;
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


void UpdateScore (land *map_arr , int* CELLS_OWNED , int** LANDS_OWNED_COUNTERS , int NUM_PLAYERS , int* GLOBAL_POINTS_ARRAY)
{
    int temp_score = 0 ;
    for ( int i=1 ; i<NUM_PLAYERS ; i++ )
    {
        temp_score = 0 ;
        for ( int j_c=0 ; j_c<CELLS_OWNED[i] ; j_c++)
        {
            temp_score += 10 ;
            temp_score += 20 * map_arr[LANDS_OWNED_COUNTERS[i][j_c]].IS_MILITARY ;
            temp_score += map_arr[LANDS_OWNED_COUNTERS[i][j_c]].soldiers_number / 4 ;
        }
        GLOBAL_POINTS_ARRAY[i] = temp_score>GLOBAL_POINTS_ARRAY[i] ? temp_score : GLOBAL_POINTS_ARRAY[i] ;
    }
}


void DisplayScores ( SDL_Renderer* sdlRenderer , int NUM_PLAYERS , int* GLOBAL_POINTS_ARRAY )
{
    int x = 150 ; int y=40 ;
    for ( int i=1 ; i<NUM_PLAYERS ; i++ )
    {
        char* score = malloc(sizeof(char) * 20 ) ;
        sprintf(score , "id[%d] : %d" , i , GLOBAL_POINTS_ARRAY[i]) ;
        stringColor(sdlRenderer , x , y , score , 0xff000000) ;
        x += 100 ;
    }
}


// a function to export map data to a file in tmp
void ExportData ( int NUM_CELLS , int NUM_PLAYERS , int HEXAGON_A
                  ,  land* map_arr , OneSoldier** AllSoldiersArray ,OnePotionEffect* AllPotionsArray )
{
    FILE* tmp_data = fopen("../dat/tmp/map_data.dat" , "w+") ;
    if ( tmp_data == NULL )
        fprintf(stderr , "Couldn't open map_data.dat") ;

    fprintf(tmp_data , "%d~%d~%d\n" , NUM_PLAYERS , NUM_CELLS , HEXAGON_A ) ;
    for ( int i=0 ; i<NUM_CELLS ; i++)
        fprintf(tmp_data , "%d~%d~%d~%d~%d~%d~%f\n" , map_arr[i].counter , map_arr[i].owner_id , map_arr[i].x , map_arr[i].y
                                                    , map_arr[i].IS_MILITARY , map_arr[i].RELATED_MILITARY_COUNTER , map_arr[i].soldiers_number) ;
    for ( int i=0 ; i<NUM_PLAYERS ; i++)
        fprintf(tmp_data , "%d~%d~%d\n" , AllPotionsArray[i].potion_id , AllPotionsArray[i].time_to_exist , AllPotionsArray[i].owner_id) ;

    int line_counter = 0 ;
    for ( int i=0 ; i<50 ; i++)
        line_counter += (AllSoldiersArray[i] != 0) ;

    fprintf(tmp_data , "%d\n" , line_counter ) ;
    for ( int i=0 ; i<50 ; i++ ) {
        if ( AllSoldiersArray[i] != 0 ) {
            for (int temp = 0; temp < AllSoldiersArray[i][0].num_of_all_soldiers ; temp++ )
                fprintf(tmp_data , "%d~%f~%f~%f~%f~%f~%f~%f~%f~%f~%d~%d~%d\n"
                                                , AllSoldiersArray[i][temp].owner_id , AllSoldiersArray[i][0].power
                                                , AllSoldiersArray[i][temp].x , AllSoldiersArray[i][temp].y
                                                , AllSoldiersArray[i][temp].origin_x , AllSoldiersArray[i][temp].origin_y
                                                , AllSoldiersArray[i][temp].destination_x , AllSoldiersArray[i][temp].destination_y
                                                , AllSoldiersArray[i][temp].verticalSpeed , AllSoldiersArray[i][temp].horizontalSpeed
                                                , AllSoldiersArray[i][temp].num_of_all_soldiers , AllSoldiersArray[i][temp].soldier_id
                                                , AllSoldiersArray[i][temp].destination_counter ) ;
        }
    }
    fclose(tmp_data) ;
    FILE* cond_txt = fopen("../dat/tmp/cond.txt" , "w") ;
    fputc('1' , cond_txt) ;
    fclose(cond_txt) ;
}


// import tmp map
ImportStructure ImportData ( int* NUM_CELLS , int* NUM_PLAYERS ,
                  land* map_arr , OneSoldier** AllSoldiersArray ,OnePotionEffect* AllPotionsArray
                  , char* Address )
{
    ImportStructure output ;
    int HEXAGON_A ;
    FILE *map_file = fopen( Address , "r" ) ;
    fscanf(map_file , "%d~%d~%d\n" , NUM_PLAYERS , NUM_CELLS , &HEXAGON_A) ;
    int n = *NUM_CELLS ;
    int p = *NUM_PLAYERS ;

    for ( int i=0 ; i<n ; i++ ) {
        int co , ow_id , Is , Rel , x , y ;
        float sol_nu ;
        fscanf(map_file, "%d~%d~%d~%d~%d~%d~%f\n" , &co , &ow_id , &x , &y , &Is , &Rel , &sol_nu ) ;
        map_arr[i].counter = co ;   map_arr[i].owner_id = ow_id ;
        map_arr[i].x = x ;          map_arr[i].y = y ;
        map_arr[i].IS_MILITARY = Is ; map_arr[i].RELATED_MILITARY_COUNTER = Rel ;
        map_arr[i].soldiers_number = sol_nu ;
    }
    n = *NUM_PLAYERS ;
    for ( int i=0 ; i<n ; i++ )
        fscanf(map_file , "%d~%d~%d\n" ,
               &(AllPotionsArray[i].potion_id) , &(AllPotionsArray[i].time_to_exist) , &(AllPotionsArray[i].owner_id)) ;

    int line_counter ;
    fscanf(map_file , "%d\n" , &line_counter ) ;

    for ( int i=0 ; i<line_counter ; i++ )
    {
        int id , nu_a , so_id , dcou ;
        float pow , x , y , ox , oy , dx , dy , vs , hs  ;
        fscanf(map_file , "%d~%f~%f~%f~%f~%f~%f~%f~%f~%f~%d~%d~%d\n"
                 ,&id , &pow , &x , &y , &ox , &oy , &dx , &dy , &vs , &hs , &nu_a , &so_id , &dcou) ;
        AllSoldiersArray[i] = malloc( sizeof(OneSoldier) * nu_a ) ;
        AllSoldiersArray[i][0].owner_id = id ;  AllSoldiersArray[i][0].power = pow ;
        AllSoldiersArray[i][0].x = x ;          AllSoldiersArray[i][0].y = y ;
        AllSoldiersArray[i][0].origin_x = ox ;  AllSoldiersArray[i][0].origin_y = oy ;
        AllSoldiersArray[i][0].destination_x = dx ; AllSoldiersArray[i][0].destination_y = dy ;
        AllSoldiersArray[i][0].verticalSpeed = vs ; AllSoldiersArray[i][0].horizontalSpeed = hs ;
        AllSoldiersArray[i][0].num_of_all_soldiers = nu_a ; AllSoldiersArray[i][0].soldier_id = so_id ;
        AllSoldiersArray[i][0].destination_counter = dcou ;

        for ( int temp=1 ; temp<nu_a; temp++ ) {
            fscanf(map_file, "%d~%f~%f~%f~%f~%f~%f~%f~%f~%f~%d~%d~%d\n", &id, &pow, &x, &y, &ox, &oy, &dx, &dy, &vs, &hs,
                   &nu_a, &so_id , &dcou);
            AllSoldiersArray[i][temp].owner_id = id ;  AllSoldiersArray[i][temp].power = pow ;
            AllSoldiersArray[i][temp].x = x ;          AllSoldiersArray[i][temp].y = y ;
            AllSoldiersArray[i][temp].origin_x = ox ;  AllSoldiersArray[i][temp].origin_y = oy ;
            AllSoldiersArray[i][temp].destination_x = dx ; AllSoldiersArray[i][temp].destination_y = dy ;
            AllSoldiersArray[i][temp].verticalSpeed = vs ; AllSoldiersArray[i][temp].horizontalSpeed = hs ;
            AllSoldiersArray[i][temp].num_of_all_soldiers = nu_a ; AllSoldiersArray[i][temp].soldier_id = so_id ;
            AllSoldiersArray[i][temp].destination_counter = dcou ;
        }
        printf("step 5\n") ;
    }
    fclose(map_file) ;

    output.map = map_arr ;
    output.soldiers = AllSoldiersArray ;
    output.potions = AllPotionsArray ;
    return output ;
}

////////////////////// end of game logic functions //////////////////////////


///////////////////// starting and loading a game ///////////////////////////
// a function to create new game
int StartNewGame ( SDL_Window *sdlWindow , SDL_Renderer *sdlRenderer ,
                   int NUMBER_OF_PLAYERS , const int WIDTH , const int HEIGHT , const int FPS ,
                   const int HEXAGON_A , const int NUM_OF_ROWS , const int NUM_OF_COLS , int user_id )
{
    float SPEED_ARRAY [6] =             { 1 , 1 , 1 , 1 , 1 , 1};
    int PRODUCTION_RATE_ARRAY [6] =     { 0 , 2 , 2 , 2 , 2 , 2};
    float SOLDIERS_POWER_ARRAY [6] =    { 1 , 1 , 1 , 1 , 1 , 1};
    int IMMUNE_LANDS_ARRAY [6] =        { 0 , 0 , 0 , 0 , 0 , 0};
    int GLOBAL_POINTS_ARRAY[6] =        { 0 , 0 , 0 , 0 , 0 , 0 };
    SDL_Texture* POTION_GRAPHIC[8] ;

    SDL_Texture *castle_texture = getImageTexture(sdlRenderer , "../img/castle.bmp") ;

    // creating white screen for entrance
    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff ) ;
    SDL_RenderClear(sdlRenderer) ;

    // Generating map_arr
    land* map_arr = malloc(sizeof(land) * (NUM_OF_ROWS * NUM_OF_COLS) ) ;
    if ( map_arr == NULL ) {
        fprintf(stderr, "map allocation error");
        return 0;
    }


    map_arr = GENERATE_HEXAGON_RANDOM_MAP(NUMBER_OF_PLAYERS , NUM_OF_COLS , NUM_OF_ROWS , map_arr , HEXAGON_A ) ;
    int NUM_OF_CELLS = ShowHexagonBackground(sdlRenderer , map_arr , NUM_OF_COLS*NUM_OF_ROWS , HEXAGON_A , castle_texture ) ;

    printf("num players is : %d and num cells is : %d\n" , NUMBER_OF_PLAYERS , NUM_OF_CELLS) ;



    // an array to save number of cells owned by each owner_id
    int* CELLS_OWNED = calloc( NUMBER_OF_PLAYERS , sizeof(int)) ;

    // an array to save counter of each owner_id lands
    int** LANDS_OWNED_COUNTERS = malloc(  NUMBER_OF_PLAYERS * sizeof(int*) ) ;
    for ( int i=0 ; i<NUMBER_OF_PLAYERS ; i++)
        LANDS_OWNED_COUNTERS[i] = calloc( NUM_OF_CELLS , sizeof(int)) ;

    printf("pre update\n") ;
    // updating map info
    UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS ) ;

    // getting background picture as texture
    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};

    // getting go_back_to_menu picture as texture
    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};

    // Initializing potion textures
    InitializePotionGraphics(sdlRenderer , POTION_GRAPHIC) ;
    SDL_Rect potion_rect = {.x=0, .y=0, .w=30 , .h=30};

    /////////////////variable definitions/////////////////

    // game primary counter
    int i=1 ;
    SDL_bool shallExit = SDL_FALSE ;
    SDL_bool shallShowMenu = SDL_TRUE ;
    // variable to check mouse status
    int click_status = 0 ;
    // bunch of needed variables for user interaction
    Sint16 Origin_x = 0 ;
    Sint16 Origin_y = 0 ;
    int Origin_counter = 0 ;
    Sint16 Destination_x = 0 ;
    Sint16 Destination_y = 0 ;
    // variable containing data of potion that is on screen
    Potion live_time_potion = {.potion_id=-1} ;

    //////////////////////////////////////////////////////
    // a very important array to save moving soldiers data
    OneSoldier** AllSoldiersArray = calloc(2*NUM_OF_CELLS , sizeof(OneSoldier*) ) ;

    // a very important array to save activated Potions data
    OnePotionEffect* AllPotionsArray = calloc(NUMBER_OF_PLAYERS , sizeof(OnePotionEffect)) ;
    for ( int temp=0 ; temp<NUMBER_OF_PLAYERS ; temp++)
        AllPotionsArray[temp].potion_id = -1 ;

    //////////////// game /////////////////
    printf("pregame\n") ;
    while ( !shallExit )
    {
        UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS) ;
        i %= 200000 ;

        SDL_RenderCopy(sdlRenderer , img , NULL , &texture_rect ) ;
//        DrawBackground(sdlRenderer , img , HEIGHT , WIDTH) ;
        ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_CELLS , HEXAGON_A , castle_texture) ;

        if ( i%50 == 20 ) {
            for ( int j=2 ; j<NUMBER_OF_PLAYERS ; j++)
                SystemMakeMovement(j, AllSoldiersArray, map_arr, CELLS_OWNED
                        , LANDS_OWNED_COUNTERS, 5
                        , SOLDIERS_POWER_ARRAY , IMMUNE_LANDS_ARRAY );
        }

        if ( i%80 == 0 )
            AddSoldiers(map_arr , NUM_OF_CELLS , PRODUCTION_RATE_ARRAY ) ;


        if ( i%550 == 50 )
            live_time_potion = CreatePotion(WIDTH , HEIGHT) ;


        CheckForSoldierPotionConflict(AllSoldiersArray, &live_time_potion, AllPotionsArray);
        UpdatePotionEffectArray(AllPotionsArray , NUMBER_OF_PLAYERS) ;
        ApplyPotionEffect(AllPotionsArray , SPEED_ARRAY , SOLDIERS_POWER_ARRAY , PRODUCTION_RATE_ARRAY , IMMUNE_LANDS_ARRAY , NUMBER_OF_PLAYERS) ;
        RenderPotion(sdlRenderer , live_time_potion , POTION_GRAPHIC ) ;

        SDL_Event sdlEvent;
        land clicked_cell_info ;

        if ( click_status == 1 )
            AimAssist(sdlRenderer , sdlEvent , map_arr , NUM_OF_CELLS , HEXAGON_A , Origin_x , Origin_y ) ;

        while (SDL_PollEvent(&sdlEvent)) {
            Sint32 x = sdlEvent.motion.x ;
            Sint32 y = sdlEvent.motion.y ;
            switch (sdlEvent.type)
            {
                case SDL_QUIT:
                    ExportData(NUM_OF_CELLS , NUMBER_OF_PLAYERS , HEXAGON_A , map_arr , AllSoldiersArray , AllPotionsArray ) ;
                    printf("all data is saved successfully\n") ;
                    printf("User quited the game successfully!\n") ;
                    shallExit = SDL_TRUE;
//                    return 0 ;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if ( x<40 && y<40 ) {
                        shallExit = SDL_TRUE;
                        break;
                    }

                    clicked_cell_info = GiveClickedCellInfo(x , y , map_arr , NUM_OF_CELLS , HEXAGON_A) ;
                    if ( clicked_cell_info.owner_id == 1 && click_status == 0 )
                    {
                        click_status += 1 ;
                        click_status %= 2 ;
                        Origin_x = clicked_cell_info.x ;
                        Origin_y = clicked_cell_info.y ;
                        Origin_counter = clicked_cell_info.counter ;
                    }
                    else if ( clicked_cell_info.owner_id != -1 && click_status == 1 )
                    {
                        click_status += 1 ;
                        click_status %= 2 ;
                        Destination_x = clicked_cell_info.x ;
                        Destination_y = clicked_cell_info.y ;
                        // should send soldiers from origin to destination //
                        if ( Origin_counter != clicked_cell_info.counter && map_arr[Origin_counter].soldiers_number != 0
                             && IMMUNE_LANDS_ARRAY[clicked_cell_info.owner_id] != 1 && map_arr[Origin_counter].owner_id == 1 )
                            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , clicked_cell_info , SOLDIERS_POWER_ARRAY) ;
                    }
                    SDL_Delay(150) ;
                    break;

                default:
                    break;

            }
        }

        SoldierConflictSolver(AllSoldiersArray ) ;
        ShowLinesOfSoldiers(sdlRenderer , AllSoldiersArray , HEXAGON_A , map_arr , SPEED_ARRAY) ;

        UpdateScore(map_arr , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY) ;
        if ( CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS) != -1 )
        {
            printf("player %d has won \n" , CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)) ;
            GLOBAL_POINTS_ARRAY[CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)] += 400 ;
            for ( int i=1 ; i<NUMBER_OF_PLAYERS ; i++)
                printf("user %d with score of %d\n" , i , GLOBAL_POINTS_ARRAY[i]) ;
            UpdateLeaderboard(GLOBAL_POINTS_ARRAY , NUMBER_OF_PLAYERS , user_id) ;
            shallExit = SDL_TRUE ;
//            return 0 ;
        }
        DisplayScores(sdlRenderer , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY ) ;
        DisplayPotionsEffect(sdlRenderer , AllPotionsArray , POTION_GRAPHIC , NUMBER_OF_PLAYERS) ;
        SDL_RenderPresent(sdlRenderer) ;
        SDL_Delay(1000/FPS) ;
        i++;
    }


    // Freeing some memory for god's sake
    free(AllSoldiersArray) ;
    free(AllPotionsArray) ;
    free(map_arr) ;
    free(CELLS_OWNED) ;
    free(LANDS_OWNED_COUNTERS) ;

    // Destroying window and program //
    SDL_DestroyTexture(img) ;
    SDL_DestroyTexture(back_to_menu) ;
    SDL_DestroyTexture(castle_texture) ;

    return 0 ;
}

// a function to load pre saved game
int LoadGame ( SDL_Window *sdlWindow , SDL_Renderer *sdlRenderer ,
               char* Address , int WIDTH , int HEIGHT , int FPS , int user_id )
{
    int NUM_OF_CELLS ;
    int NUMBER_OF_PLAYERS ;
    int HEXAGON_A ;

    FILE* map_data = fopen(Address , "r") ;
    fscanf(map_data , "%d~%d~%d\n" , &NUMBER_OF_PLAYERS , &NUM_OF_CELLS , &HEXAGON_A )  ;
    fclose(map_data) ;


    land* map_arr = malloc(sizeof(land) * 400 ) ;
    OneSoldier **AllSoldiersArray = calloc(50 , sizeof(OneSoldier*)) ;
    OnePotionEffect *AllPotionsArray = calloc(NUMBER_OF_PLAYERS , sizeof(OnePotionEffect)) ;
    ImportStructure input = ImportData(&NUM_OF_CELLS , &NUMBER_OF_PLAYERS , map_arr , AllSoldiersArray , AllPotionsArray , Address) ;

    map_arr = input.map ;
    AllSoldiersArray  = input.soldiers ;
    AllPotionsArray = input.potions ;

    float SPEED_ARRAY [6] =             { 1 , 1 , 1 , 1 , 1 , 1};
    int PRODUCTION_RATE_ARRAY [6] =     { 0 , 2 , 2 , 2 , 2 , 2};
    float SOLDIERS_POWER_ARRAY [6] =    { 1 , 1 , 1 , 1 , 1 , 1};
    int IMMUNE_LANDS_ARRAY [6] =        { 0 , 0 , 0 , 0 , 0 , 0};
    int GLOBAL_POINTS_ARRAY[6] =        { 0 , 0 , 0 , 0 , 0 , 0 };
    SDL_Texture* POTION_GRAPHIC[8] ;

    SDL_Texture *castle_texture = getImageTexture(sdlRenderer , "../img/castle.bmp") ;

    // creating white screen for entrance
    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff ) ;
    SDL_RenderClear(sdlRenderer) ;

    // an array to save number of cells owned by each owner_id
    int* CELLS_OWNED = calloc( NUMBER_OF_PLAYERS , sizeof(int)) ;

    // an array to save counter of each owner_id lands
    int** LANDS_OWNED_COUNTERS = malloc(  NUMBER_OF_PLAYERS * sizeof(int*) ) ;
    for ( int i=0 ; i<NUMBER_OF_PLAYERS ; i++)
        LANDS_OWNED_COUNTERS[i] = calloc( NUM_OF_CELLS , sizeof(int)) ;

    printf("pre update\n") ;
    // updating map info
    UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS ) ;


    // getting background picture as texture
    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};

    // getting go_back_to_menu picture as texture
    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};

    // Initializing potion textures
    InitializePotionGraphics(sdlRenderer , POTION_GRAPHIC) ;
    SDL_Rect potion_rect = {.x=0, .y=0, .w=30 , .h=30};

    /////////////////variable definitions/////////////////
    // game primary counter
    int i=1 ;
    SDL_bool shallExit = SDL_FALSE ;
    SDL_bool shallShowMenu = SDL_TRUE ;
    // variable to check mouse status
    int click_status = 0 ;
    // bunch of needed variables for user interaction
    Sint16 Origin_x = 0 ;
    Sint16 Origin_y = 0 ;
    int Origin_counter = 0 ;
    Sint16 Destination_x = 0 ;
    Sint16 Destination_y = 0 ;
    // variable containing data of potion that is on screen
    Potion live_time_potion = {.potion_id=-1} ;
    //////////////////////////////////////////////////////


    //////////////// game /////////////////
    printf("pregame\n") ;
    while ( !shallExit )
    {
        UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS) ;
        i %= 200000 ;

        SDL_RenderCopy(sdlRenderer , img , NULL , &texture_rect) ;
//        DrawBackground(sdlRenderer , img , HEIGHT , WIDTH) ;
        ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_CELLS , HEXAGON_A , castle_texture) ;

        if ( i%50 == 20 ) {
            for ( int j=2 ; j<NUMBER_OF_PLAYERS ; j++)
                SystemMakeMovement(j, AllSoldiersArray, map_arr, CELLS_OWNED
                        , LANDS_OWNED_COUNTERS, NUMBER_OF_PLAYERS
                        , SOLDIERS_POWER_ARRAY , IMMUNE_LANDS_ARRAY );
        }

        if ( i%80 == 0 )
            AddSoldiers(map_arr , NUM_OF_CELLS , PRODUCTION_RATE_ARRAY ) ;

        if ( i%550 == 50 )
            live_time_potion = CreatePotion(WIDTH , HEIGHT) ;


        CheckForSoldierPotionConflict(AllSoldiersArray, &live_time_potion, AllPotionsArray);
        UpdatePotionEffectArray(AllPotionsArray , NUMBER_OF_PLAYERS) ;
        ApplyPotionEffect(AllPotionsArray , SPEED_ARRAY , SOLDIERS_POWER_ARRAY , PRODUCTION_RATE_ARRAY , IMMUNE_LANDS_ARRAY , NUMBER_OF_PLAYERS) ;
        RenderPotion(sdlRenderer , live_time_potion , POTION_GRAPHIC ) ;

        SDL_Event sdlEvent;
        land clicked_cell_info ;

        if ( click_status == 1 )
            AimAssist(sdlRenderer , sdlEvent , map_arr , NUM_OF_CELLS , HEXAGON_A , Origin_x , Origin_y ) ;

        while (SDL_PollEvent(&sdlEvent)) {
            Sint32 x = sdlEvent.motion.x ;
            Sint32 y = sdlEvent.motion.y ;
            switch (sdlEvent.type)
            {
                case SDL_QUIT:
                    ExportData(NUM_OF_CELLS , NUMBER_OF_PLAYERS , HEXAGON_A , map_arr , AllSoldiersArray , AllPotionsArray) ;
                    printf("all data is saved successfully\n") ;
                    printf("User quited the game successfully !\n") ;
//                    return 0 ;
                    shallExit = SDL_TRUE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if ( x<40 && y<40 )
                        return 0;

                    clicked_cell_info = GiveClickedCellInfo(x , y , map_arr , NUM_OF_CELLS , HEXAGON_A) ;
                    if ( clicked_cell_info.owner_id == 1 && click_status == 0 )
                    {
                        click_status += 1 ;
                        click_status %= 2 ;
                        Origin_x = clicked_cell_info.x ;
                        Origin_y = clicked_cell_info.y ;
                        Origin_counter = clicked_cell_info.counter ;
                    }
                    else if ( clicked_cell_info.owner_id != -1 && click_status == 1 )
                    {
                        click_status += 1 ;
                        click_status %= 2 ;
                        Destination_x = clicked_cell_info.x ;
                        Destination_y = clicked_cell_info.y ;
                        // should send soldiers from origin to destination //
                        if ( Origin_counter != clicked_cell_info.counter && map_arr[Origin_counter].soldiers_number != 0
                             && IMMUNE_LANDS_ARRAY[clicked_cell_info.owner_id] != 1 && map_arr[Origin_counter].owner_id == 1 )
                            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , clicked_cell_info , SOLDIERS_POWER_ARRAY) ;
                    }
                    SDL_Delay(150) ;
                    break;

                default:
                    break;
            }
        }

        SoldierConflictSolver(AllSoldiersArray ) ;
        ShowLinesOfSoldiers(sdlRenderer , AllSoldiersArray , HEXAGON_A , map_arr , SPEED_ARRAY) ;

        UpdateScore(map_arr , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY) ;
        if ( CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS) != -1 )
        {
            printf("player %d has won \n" , CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)) ;
            GLOBAL_POINTS_ARRAY[CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)] += 400 ;
            UpdateLeaderboard(GLOBAL_POINTS_ARRAY , NUMBER_OF_PLAYERS , user_id) ;
//            return 0 ;
            shallExit = SDL_TRUE ;
        }

        DisplayScores(sdlRenderer , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY ) ;
        DisplayPotionsEffect(sdlRenderer , AllPotionsArray , POTION_GRAPHIC , NUMBER_OF_PLAYERS) ;
        SDL_RenderPresent(sdlRenderer) ;
        SDL_Delay(1000/FPS) ;
        i++;
    }

    free(AllSoldiersArray) ;
    free(AllPotionsArray) ;
    free(map_arr) ;
    free(CELLS_OWNED) ;
    free(LANDS_OWNED_COUNTERS) ;

    // Freeing some memory for god's sake
    SDL_DestroyTexture(img) ;
    SDL_DestroyTexture(back_to_menu) ;
    SDL_DestroyTexture(castle_texture) ;

    return 0 ;
}

