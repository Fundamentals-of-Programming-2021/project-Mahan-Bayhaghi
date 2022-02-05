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


// struct containing every land information
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

// struct containing every soldier information
typedef struct OneSoldier {
    int num_of_all_soldiers ;
    int soldier_id ;
    float origin_x ;
    float origin_y ;
    int owner_id ;
    int power ;
    float x ;
    float y ;
    float destination_x ;
    float destination_y ;
    int destination_counter ;
    float verticalSpeed ;
    float horizontalSpeed ;
} OneSoldier ;


enum PotionType { USER_SPEED_2X , SYSTEM_SPEED_0X , SYSTEM_SPEED_HALF , USER_POWER_2X };

// struct containing potion data
typedef struct Potion {
    int x ;
    int y ;
    int potion_id ;
    enum PotionType type ;
} Potion ;

// struct containing a potion's effect data
typedef struct OnePotionEffect {
    int time_to_exist ;
    int potion_id ;
    int owner_id ;
} OnePotionEffect  ;

// A simple test function to see if header is included correctly
void map_handling_test_func () ;

// function to create randomized hexagon map
// returns pointer to out game map ( pointer to an array of lands )
land* GENERATE_HEXAGON_RANDOM_MAP (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int NUM_PLAYERS , int WIDTH , int HEIGHT , land* map , int HEXAGON_a) ;


// function to display hexagon map
// returns number of cells in the game
int ShowHexagonBackground ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , land* map , int counter , Sint16 a) ;


// function to add soldiers to an array of lands
void AddSoldiers ( land* map , int counter , int* PRODUCTION_RATE_ARRAY ) ;


// function to specify which cell is being clicked or touched by user
// returns land properties
land GiveClickedCellInfo ( Sint16 x , Sint16 y , land* map , int counter , int Hexagon_a ) ;


// a function to create a line of soldiers from map_arr[Origin_counter] to map_arr[Destination_cell_info.counter] in
// first free place of AllSoldierArray which is going to be rendered on screen
void CreateLineOfSoldiers ( OneSoldier** AllSoldiersArray , land* map_arr , int Origin_counter , land Destination_cell_info
        , float* SOLDIERS_POWER_ARRAY ) ;

// a function to show soldiers if they haven't reached to their destination
// or to destroy a soldier if reached destination
void ShowLinesOfSoldiers ( SDL_Renderer* sdlRenderer , OneSoldier** AllSoldiersArray , int HEXAGON_A , land* map_arr
                            , float* SPEED_ARRAY ) ;


// a function to set conflicting soldiers power to zero
void SoldierConflictSolver ( OneSoldier** AllSoldiersArray );


// a function to update map info for managing attacks and winning condition
void UpdateMapInfo(land *map_arr , int NUM_OF_CELLS , int* CELLS_OWNED , int** LANDS_OWNED_COUNTERS) ;


// a function to update system and also user score
void UpdateScore ( ) ;


// a function to make System players make a proper attack to others
// they may or may not move according to situation
// this function uses CreateLineOfSoldiers just like what user does
void SystemMakeMovement ( int owner_id , OneSoldier** AllSoldiersArray , land* map_arr
        , int* CELLS_OWNED , int** LANDS_OWNED_COUNTERS , int NUM_OF_PLAYERS , float* SOLDIERS_POWER_ARRAY ) ;


// a function to check if any player has won
// returns 0 if not else returns 1
int CheckWinState ( int* CELLS_OWNED ) ;


// a function that may or may not create random accessible potion
// returns a potion structure
Potion CreatePotion ( int WIDTH , int HEIGHT ) ;


// a function to check if a potion is touched by a soldier or not
// if yes : activate potion effect on speed array or power array and returns activation information
// else : returns
void CheckForSoldierPotionConflict ( OneSoldier** AllSoldiersArray , Potion* live_time_potion
                                     , OnePotionEffect* AllPotionsEffect ) ;

// a function to create a potion effect in global array AllPotionsArray
// returns 1 if potion effect is created
// else returns 0
int CreatePotionEffect ( Potion potion_info , OnePotionEffect* AllPotionsArray , int Destination_owner ) ;


// a function to update potion effect data
void UpdatePotionEffectArray ( OnePotionEffect* AllPotionsEffect ) ;



// a function to apply potion effect
void ApplyPotionEffect ( OnePotionEffect* AllPotionsEffect , float* SPEED_ARRAY,
                         float* SOLDIERS_POWER_ARRAY , int* PRODUCTION_RATE_ARRAY) ;

