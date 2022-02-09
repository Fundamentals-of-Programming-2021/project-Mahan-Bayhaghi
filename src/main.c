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

        condition = StartMenu( sdlWindow ,  sdlRenderer ) ;
        if ( condition == -1 )
        {
            // quit the game
            printf("quitted the game") ;
            ShallExitGame = SDL_TRUE ;
            return 0 ;
        }

        if ( condition == 1 )
        {
            // new game
            printf("new game\n") ;
            NewGame(sdlWindow , sdlRenderer ) ;
        }

        if ( condition == 2 )
        {
            // show leaderboard
            printf("show leaderboard\n") ;
        }

        if ( condition == 0 )
        {
            // continue already saved game
            printf("continue\n") ;
        }

    }

    printf("out of loop\n") ;
    SDL_DestroyRenderer(sdlRenderer) ;
    printf("renderer destroyed\n") ;
    SDL_DestroyWindow(sdlWindow) ;
    printf("window destroyed\n") ;


//    StartNewGame( 3 , WIDTH , HEIGHT , 80 , 28 , 18 , 16 ) ;

//
//    // game main sdlWindow
//    SDL_Window *sdlWindow = SDL_CreateWindow("State.io" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;
//
//    // game main sdlRenderer
//    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;
//
//    SDL_Texture *castle_texture = getImageTexture(sdlRenderer , "../img/castle.bmp") ;
//
//    // creating white screen for entrance
//    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff ) ;
//    SDL_RenderClear(sdlRenderer) ;
//
//    // Generating map_arr
//    land* map_arr = malloc(sizeof(land) * (NUM_OF_ROWS * NUM_OF_COLS) ) ;
//    if ( map_arr == NULL ) {
//        fprintf(stderr, "map allocation error");
//        return 0;
//    }
//
////    int NUM_OF_CELLS ;
////    ImportMap(map_arr , "../dat/map/map_sample.dat" , &NUM_OF_CELLS) ;
//    map_arr = GENERATE_HEXAGON_RANDOM_MAP(NUMBER_OF_PLAYERS , NUM_OF_COLS , NUM_OF_ROWS , map_arr , HEXAGON_A ) ;
//    int NUM_OF_CELLS = ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_ROWS*NUM_OF_COLS , HEXAGON_A , castle_texture) ;
//
////    char* name = "../dat/map/map_sample.dat" ;
////    ExportMap(map_arr , name , NUM_OF_CELLS) ;
//
//
//    printf("num of cells is : %d\n" , NUM_OF_CELLS) ;
//    // an array to save number of cells owned by each owner_id
//    int* CELLS_OWNED = calloc( NUMBER_OF_PLAYERS , sizeof(int)) ;
//
//    // an array to save counter of each owner_id lands
//    int** LANDS_OWNED_COUNTERS = malloc(  NUMBER_OF_PLAYERS * sizeof(int*) ) ;
//    for ( int i=0 ; i<NUMBER_OF_PLAYERS ; i++)
//        LANDS_OWNED_COUNTERS[i] = calloc( NUM_OF_CELLS , sizeof(int)) ;
//
//    // updating map info
//    UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS ) ;
//
//    // getting background picture as texture
//    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
//    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};
//
//    // getting go_back_to_menu picture as texture
//    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
//    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};
//
//    // Initializing potion textures
//    InitializePotionGraphics(sdlRenderer , POTION_GRAPHIC) ;
//    SDL_Rect potion_rect = {.x=0, .y=0, .w=30 , .h=30};
//
//    /////////////////variable definitions/////////////////
//
//    // game primary counter
//    int i=1 ;
//    SDL_bool shallExit = SDL_FALSE ;
//    SDL_bool shallShowMenu = SDL_TRUE ;
//    // variable to check mouse status
//    int click_status = 0 ;
//    // bunch of needed variables for user interaction
//    Sint16 Origin_x = 0 ;
//    Sint16 Origin_y = 0 ;
//    int Origin_counter = 0 ;
//    Sint16 Destination_x = 0 ;
//    Sint16 Destination_y = 0 ;
//    // variable containing data of potion that is on screen
//    Potion live_time_potion = {.potion_id=-1} ;
//
//    //////////////////////////////////////////////////////
//    // a very important array to save moving soldiers data
//    OneSoldier** AllSoldiersArray = calloc(2*NUM_OF_CELLS , sizeof(OneSoldier*) ) ;
//
//    // a very important array to save activated Potions data
//    OnePotionEffect* AllPotionsArray = calloc(NUMBER_OF_PLAYERS , sizeof(OnePotionEffect)) ;
//    for ( int temp=0 ; temp<NUMBER_OF_PLAYERS ; temp++)
//        AllPotionsArray[temp].potion_id = -1 ;
//
//    //////////////// game /////////////////
//    while ( !shallExit )
//    {
//        UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS) ;
//        i %= 200000 ;
//        while ( shallShowMenu )
//        {
//            int condition = ShowMenu(sdlWindow , sdlRenderer , WIDTH , HEIGHT , img) ;
//            if ( condition ==1  )
//                shallShowMenu = SDL_FALSE ;
//            else if ( condition == 0 )
//            {
//                printf("User quited the game successfully \n") ;
//                shallExit = SDL_TRUE ;
//                break;
//            }
//        }
//
//        DrawBackground(sdlRenderer , img , HEIGHT , WIDTH) ;
//        ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_CELLS , HEXAGON_A , castle_texture) ;
//
//        if ( i%50 == 20 ) {
//            for ( int j=2 ; j<NUMBER_OF_PLAYERS ; j++)
//                SystemMakeMovement(j, AllSoldiersArray, map_arr, CELLS_OWNED
//                                   , LANDS_OWNED_COUNTERS, 5
//                                   , SOLDIERS_POWER_ARRAY , IMMUNE_LANDS_ARRAY );
//        }
//
//        if ( i%80 == 0 )
//            AddSoldiers(map_arr , NUM_OF_CELLS , PRODUCTION_RATE_ARRAY ) ;
//
//        if ( i%550 == 50 )
//            live_time_potion = CreatePotion(WIDTH , HEIGHT) ;
//
//
//        CheckForSoldierPotionConflict(AllSoldiersArray, &live_time_potion, AllPotionsArray);
//        UpdatePotionEffectArray(AllPotionsArray , NUMBER_OF_PLAYERS) ;
//        ApplyPotionEffect(AllPotionsArray , SPEED_ARRAY , SOLDIERS_POWER_ARRAY , PRODUCTION_RATE_ARRAY , IMMUNE_LANDS_ARRAY , NUMBER_OF_PLAYERS) ;
//        RenderPotion(sdlRenderer , live_time_potion , POTION_GRAPHIC ) ;
//
//        SDL_Event sdlEvent;
//        land clicked_cell_info ;
//
//        if ( click_status == 1 )
//            AimAssist(sdlRenderer , sdlEvent , map_arr , NUM_OF_CELLS , HEXAGON_A , Origin_x , Origin_y ) ;
//
//
//        while (SDL_PollEvent(&sdlEvent)) {
//            Sint32 x = sdlEvent.motion.x ;
//            Sint32 y = sdlEvent.motion.y ;
//            switch (sdlEvent.type)
//            {
//                case SDL_QUIT:
//                    printf("User quited the game successfully !\n") ;
//                    shallExit = SDL_TRUE;
//                    break;
//                case SDL_MOUSEMOTION:
//                    break;
//                case SDL_MOUSEBUTTONDOWN:
//                    if ( x<40 && y<40 )
//                        shallShowMenu = SDL_TRUE ;
//
//                    clicked_cell_info = GiveClickedCellInfo(x , y , map_arr , NUM_OF_CELLS , HEXAGON_A) ;
//                    if ( clicked_cell_info.owner_id == 1 && click_status == 0 )
//                    {
//                        click_status += 1 ;
//                        click_status %= 2 ;
//                        Origin_x = clicked_cell_info.x ;
//                        Origin_y = clicked_cell_info.y ;
//                        Origin_counter = clicked_cell_info.counter ;
//                    }
//                    else if ( clicked_cell_info.owner_id != -1 && click_status == 1 )
//                    {
//                        click_status += 1 ;
//                        click_status %= 2 ;
//                        Destination_x = clicked_cell_info.x ;
//                        Destination_y = clicked_cell_info.y ;
//                        // should send soldiers from origin to destination //
//                        if ( Origin_counter != clicked_cell_info.counter && map_arr[Origin_counter].soldiers_number != 0
//                            && IMMUNE_LANDS_ARRAY[clicked_cell_info.owner_id] != 1 && map_arr[Origin_counter].owner_id == 1 )
//                            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , clicked_cell_info , SOLDIERS_POWER_ARRAY) ;
//                    }
//                    SDL_Delay(150) ;
//                    break;
//                case SDL_MOUSEBUTTONUP:
//                    break;
//            }
//        }
//
//
//        SoldierConflictSolver(AllSoldiersArray ) ;
//        ShowLinesOfSoldiers(sdlRenderer , AllSoldiersArray , HEXAGON_A , map_arr , SPEED_ARRAY) ;
//
//        UpdateScore(map_arr , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY) ;
//        if ( CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS) != -1 )
//        {
//            printf("player %d has won \n" , CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)) ;
//            GLOBAL_POINTS_ARRAY[CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)] += 400 ;
//            for ( int i=1 ; i<NUMBER_OF_PLAYERS ; i++)
//                printf("user %d with score of %d\n" , i , GLOBAL_POINTS_ARRAY[i]) ;
//            return 0 ;
//        }
//        DisplayScores(sdlRenderer , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY ) ;
//        DisplayPotionsEffect(sdlRenderer , AllPotionsArray , POTION_GRAPHIC , NUMBER_OF_PLAYERS) ;
//        SDL_RenderPresent(sdlRenderer) ;
//        SDL_Delay(1000/FPS) ;
//        i++;
//    }
//
//
//
//
//    // Freeing some memory for god's sake
//    SDL_free(img) ;
//    SDL_free(back_to_menu) ;
//    free(AllSoldiersArray) ;
//    free(map_arr) ;
//
//
//    // Destroying window and program //
//    SDL_DestroyTexture(img) ;

//    SDL_DestroyRenderer(sdlRenderer) ;
//    SDL_DestroyWindow(sdlWindow) ;

    // Quitting SDL and subsystems
    SDL_Quit() ;
    printf("all done") ;
    return  0 ;
}

