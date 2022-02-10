#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_mixer.h>
#include <SDL_audio.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#include "map_handling.h"
#include "background_handling.h"


const int WIDTH = 960 ;
const int HEIGHT = 640 ;
const int FPS = 60 ;
const int HEXAGON_A = 26 ;
const int NUM_OF_ROWS = 20 ;
const int NUM_OF_COLS = 18 ;

const int NUMBER_OF_PLAYERS = 6 ;

float SPEED_ARRAY [6] =             { 1 , 1 , 1 , 1 , 1 , 1};
int PRODUCTION_RATE_ARRAY [6] =     { 0 , 2 , 2 , 2 , 2 , 2};
float SOLDIERS_POWER_ARRAY [6] =    { 1 , 1 , 1 , 1 , 1 , 1};
int IMMUNE_LANDS_ARRAY [6] =        { 0 , 0 , 0 , 0 , 0 , 0};
int GLOBAL_POINTS_ARRAY[6] =        { 0 , 0 , 0 , 0 , 0 , 0 };
SDL_Texture* POTION_GRAPHIC[8] ;

int main()
{
    ////////////////////Initialization//////////////////////////
    // Initializing SDL for video and timer and audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr , "SDL could not initialize: SDL_Error: %s\n", SDL_GetError() ) ;
        return 0;
    }

    // Initializing and displaying available audio drivers
    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
        const char* driver_name = SDL_GetAudioDriver(i);
        printf("audio driver name : %s\n" , driver_name) ;
        if (SDL_AudioInit(driver_name)) {
            fprintf(stderr , "Audio driver failed to initialize: %s\n" , driver_name) ;
            continue;
        }
    }
    /////////////////////////////////////////////////////////////

    SDL_Window *sdlWindow = SDL_CreateWindow("state io" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;

    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;

    int user_id =  Login ( sdlWindow , sdlRenderer ) ;
    if ( user_id == -1 )
    {
        SDL_DestroyRenderer(sdlRenderer) ;
        SDL_DestroyWindow(sdlWindow) ;
        SDL_Quit() ;
        return 0 ;
    }
    printf("user logged in as user id [%d]\n" , user_id) ;


    int condition ;
    SDL_bool ShallExitGame = SDL_FALSE ;
    while ( !ShallExitGame )
    {
        // SHOW START MENU
        // AWAITS FOR COMMAND --> CONTINUE
        //                              LOADS TEMP MAP AND SOLDIERS AND POTION IN tmp
        //                    --> NEW GAME
        //                              CHOOSE BETWEEN READY MAPS OR GENERATE RANDOM ONE
        //                    --> LEADERBOARD
        //                               READS SCORE FROM FILE
        //                    --> EXIT --> ShallExitGame = SDL_TRUE
        //                               QUIT GAME

        condition = StartMenu( sdlWindow ,  sdlRenderer , user_id ) ;
        if ( condition == 0 )           // continue already saved game
            LoadGame(sdlWindow , sdlRenderer , "../dat/tmp/map_data.dat" , 960 , 640 , 60 , user_id ) ;

        if ( condition == 1 )            // go to new game selection page
            NewGameSelection(sdlWindow , sdlRenderer , user_id ) ;

        if ( condition == 2 )            // show leaderboard
        {
            ShowLeaderboard(sdlWindow , sdlRenderer ) ;
            printf("show leaderboard\n") ;
        }

        if ( condition == -1 )          // quit the game
            ShallExitGame = SDL_TRUE ;
    }
    SDL_DestroyRenderer(sdlRenderer) ;
    SDL_DestroyWindow(sdlWindow) ;
    SDL_Quit() ;
    return 0 ;
}

