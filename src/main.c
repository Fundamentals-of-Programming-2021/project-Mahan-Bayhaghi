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

#define 	GFX_FONTDATAMAX   (8*256)

const int WIDTH = 800 ;
const int HEIGHT = 600 ;
const int FPS = 60 ;
const int PRO_RATE = 1 ;
const int INIT_SOLDIER = 25 ;
const int NUMBER_OF_PLAYERS = 3 ;
const int HEXAGON_A = 28 ;
const int NUM_OF_ROWS = 8 ;
const int NUM_OF_COLS = 16 ;

float SPEED_ARRAY [4] =             { 1 , 1 , 1 , 1 };
int PRODUCTION_RATE_ARRAY [4] =     { 0 , 2 , 2 , 2};
float SOLDIERS_POWER_ARRAY [4] =    { 2 , 1 , 1, 1 };


Uint32 COLORS[4] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffb00500 };
                        // grey          // blue          // green        // red


Uint32 POTIONS_COLORS[4] = { 0xff00ff00 , 0xff000000 , 0xff00ffff , 0xff0000ff};

int main()
{
    // Initializing SDL for video and timer and audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr , "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ) ;
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

    // testing headers
    test_func() ;
    map_handling_test_func() ;


    // creating sdlWindow and sdlRenderer
    SDL_Window *sdlWindow = SDL_CreateWindow("State.io" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;
    // creating white screen for entrance
    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff ) ;
    SDL_RenderClear(sdlRenderer) ;


    // Generating map_arr
    land* map_arr = malloc(sizeof(land) * NUM_OF_COLS*NUM_OF_ROWS ) ;
    map_arr = GENERATE_HEXAGON_RANDOM_MAP(sdlWindow , sdlRenderer , 3 , WIDTH , HEIGHT , map_arr , HEXAGON_A ) ;
    int NUM_OF_CELLS = ShowHexagonBackground(sdlWindow , sdlRenderer , map_arr , 84 , HEXAGON_A) ;

    // an array to save number of cells owned by each owner_id
    int* CELLS_OWNED = calloc( 4 , sizeof(int)) ;
    // an array to save counter of each owner_id lands
    int** LANDS_OWNED_COUNTERS = malloc(sizeof(int*) * 4 ) ;
    for ( int i=0 ; i<4 ; i++)
        LANDS_OWNED_COUNTERS[i] = calloc( NUM_OF_CELLS , sizeof(int)) ;
    // updating map info
    UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS ) ;


    // getting background picture as texture
    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};


    // getting go_back_to_menu picture as texture
    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};

    SDL_Texture *potion = getImageTexture(sdlRenderer , "../start.bmp") ;
    SDL_Rect potion_rect = {.x=0, .y=0, .w=50 , .h=50};

    int i=1 ;
    SDL_bool shallExit = SDL_FALSE ;
    SDL_bool shallShowMenu = SDL_TRUE ;

    int click_status = 0 ;

    Sint16 Origin_x = 0 ;
    Sint16 Origin_y = 0 ;
    int Origin_counter = 0 ;
    Sint16 Destination_x = 0 ;
    Sint16 Destination_y = 0 ;

    int Potion_condition = 0 ;
    int Potion_time_to_exist = 0 ;

    Potion live_time_potion = {.potion_id=-1} ;


    // a very important array to save moving soldiers data
    OneSoldier** AllSoldiersArray = calloc(2*NUM_OF_CELLS , sizeof(OneSoldier*) ) ;

    // a very important array to save activated Potions data
    OnePotionEffect* AllPotionsArray = calloc(4 , sizeof(OnePotionEffect)) ;
    for ( int temp=0 ; temp<4 ; temp++)
    {
        AllPotionsArray[temp].potion_id = -1 ;
    }

    while ( !shallExit )
    {
        i %= 200000 ;

        while ( shallShowMenu )
        {
            int condition = ShowMenu(sdlWindow , sdlRenderer , WIDTH , HEIGHT , img) ;
            if ( condition ==1  )
                shallShowMenu = SDL_FALSE ;
            else if ( condition == 0 )
            {
                printf("User quited the game successfully \n") ;
                shallExit = SDL_TRUE ;
                break;
            }
        }

        UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS ) ;


        SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff ,0xff) ;
        SDL_RenderClear(sdlRenderer) ;
        SDL_RenderCopy(sdlRenderer, img , NULL, &texture_rect);
        SDL_RenderCopy(sdlRenderer, back_to_menu , NULL, &back_to_menu_texture_rect);

        ShowHexagonBackground(sdlWindow , sdlRenderer , map_arr , NUM_OF_CELLS , HEXAGON_A) ;

        if ( i%50 == 20 )
            SystemMakeMovement(2 , AllSoldiersArray , map_arr , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS , SOLDIERS_POWER_ARRAY) ;

        if ( i%50 == 49 )
            SystemMakeMovement(3 , AllSoldiersArray , map_arr , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS , SOLDIERS_POWER_ARRAY) ;

        if ( i%100 == 0 )
            AddSoldiers(map_arr , NUM_OF_CELLS , PRODUCTION_RATE_ARRAY ) ;


        SDL_Event sdlEvent;
        land clicked_cell_info ;

        if ( i%400 == 50 )
            live_time_potion = CreatePotion(WIDTH , HEIGHT) ;

        CheckForSoldierPotionConflict(AllSoldiersArray, &live_time_potion, AllPotionsArray);
        UpdatePotionEffectArray(AllPotionsArray) ;
        ApplyPotionEffect(AllPotionsArray , SPEED_ARRAY , SOLDIERS_POWER_ARRAY , PRODUCTION_RATE_ARRAY) ;

        if ( live_time_potion.potion_id != -1 )
            filledCircleColor(sdlRenderer, live_time_potion.x, live_time_potion.y, 10, POTIONS_COLORS[live_time_potion.potion_id]);



        if ( click_status == 1 )    // aim helper //
        {
            land mouse_cell = GiveClickedCellInfo(sdlEvent.motion.x , sdlEvent.motion.y , map_arr , NUM_OF_CELLS , HEXAGON_A ) ;
            SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xff);
            if ( mouse_cell.owner_id != -1 ) {
                SDL_RenderDrawLine(sdlRenderer, Origin_x, Origin_y, mouse_cell.x, mouse_cell.y);
                filledCircleColor(sdlRenderer , mouse_cell.x , mouse_cell.y , 2 , 0xff000000) ;
            }
            else if ( sdlEvent.motion.x != 0 && sdlEvent.motion.y != 0)
                SDL_RenderDrawLine(sdlRenderer, Origin_x, Origin_y, sdlEvent.motion.x , sdlEvent.motion.y);
        }

        while (SDL_PollEvent(&sdlEvent)) {
            Sint32 x = sdlEvent.motion.x ;
            Sint32 y = sdlEvent.motion.y ;
            switch (sdlEvent.type)
            {
                case SDL_QUIT:
                    printf("User quited the game successfully !\n") ;
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEMOTION:
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("mouse is being pushed at (%d , %d)\n" , x , y) ;
                    if ( x<40 && y<40 )
                        shallShowMenu = SDL_TRUE ;

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
                        printf("origin (%d , %d) -- destination (%d , %d)\n" , Origin_x , Origin_y , Destination_x , Destination_y ) ;
                        // should send soldiers from origin to destination //
                        if ( Origin_counter != clicked_cell_info.counter && map_arr[Origin_counter].soldiers_number != 0 )
                            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , clicked_cell_info , SOLDIERS_POWER_ARRAY) ;

                    }
                    SDL_Delay(150) ;
                    break;

                case SDL_MOUSEBUTTONUP:
                    break;
            }
        }


        SoldierConflictSolver(AllSoldiersArray) ;
        ShowLinesOfSoldiers(sdlRenderer , AllSoldiersArray , HEXAGON_A , map_arr , SPEED_ARRAY ) ;

        if ( CheckWinState(CELLS_OWNED) )
        {
            printf("game over\n") ;
            return 0 ;
        }


        SDL_RenderPresent(sdlRenderer) ;
        SDL_Delay(1000/FPS) ;
        i++;
    }





    // Freeing some memory for god's sake
    SDL_free(img) ;
    SDL_free(back_to_menu) ;
    free(AllSoldiersArray) ;
    free(map_arr) ;

    // Destroying window and program //
    SDL_DestroyTexture(img) ;
    SDL_DestroyRenderer(sdlRenderer) ;
    SDL_DestroyWindow(sdlWindow) ;

    // Quitting SDL and subsystems
    SDL_Quit() ;
    printf("all done") ;
    return  0 ;
}

