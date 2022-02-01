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
const int LAND_WIDTH = 30 ;
const int LAND_HEIGHT = 40 ;

Uint32 COLORS[4] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffb00500 };
                        // grey          // blue          // green        // red

int main()
{

    // Initializing SDL for video and timer and audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    // Initializing and displaying available audio drivers
    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
        const char* driver_name = SDL_GetAudioDriver(i);
        printf("audio driver name : %s\n" , driver_name) ;
        if (SDL_AudioInit(driver_name)) {
            printf("Audio driver failed to initialize: %s\n", driver_name);
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
    land* map_arr = malloc(sizeof(land) * 7*12 ) ;
    map_arr = GENERATE_HEXAGON_RANDOM_MAP(sdlWindow , sdlRenderer , 3 , WIDTH , HEIGHT , map_arr , 35 ) ;
    int NUM_OF_CELLS = ShowHexagonBackground(sdlWindow , sdlRenderer , map_arr , 84 , 35) ;


    // getting background picture as texture
    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};


    // getting go_back_to_menu picture as texture
    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};

    int i=1 ;
    SDL_bool shallExit = SDL_FALSE ;
    SDL_bool shallShowMenu = SDL_TRUE ;

    int little_circle = 0 ;
    while ( !shallExit )
    {
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

        i %= 200000 ;

        SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff ,0xff) ;
        SDL_RenderClear(sdlRenderer) ;

        SDL_RenderCopy(sdlRenderer, img , NULL, &texture_rect);
        SDL_RenderCopy(sdlRenderer, back_to_menu , NULL, &back_to_menu_texture_rect);

        ShowHexagonBackground(sdlWindow , sdlRenderer , map_arr , NUM_OF_CELLS , 35) ;

        if ( i%100 == 0 )
            AddSoldiers(map_arr , NUM_OF_CELLS ) ;

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            Sint16 x = sdlEvent.motion.x ;
            Sint16 y = sdlEvent.motion.y ;
            switch (sdlEvent.type)
            {
                case SDL_QUIT:
                    printf("User quited the game successfully !\n") ;
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEMOTION:
                    if ( little_circle == 1)
                        filledCircleColor(sdlRenderer , x , y , 5 , 0xff4f5500) ;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    little_circle = 1 ;
                    printf("mouse is being pushed at (%d , %d)\n" , x , y) ;
                    if ( x<40 && y<40 )
                        shallShowMenu = SDL_TRUE ;

                    land clicked_cell_info = GiveClickedCellInfo(x , y , map_arr , NUM_OF_CELLS , 35) ;
                    printf("clicked cell info owner is : %d\n" , clicked_cell_info.owner_id) ;

                    SDL_Delay(300) ;
                    break;
                case SDL_MOUSEBUTTONUP:
                    printf("mouse is up at (%d , %d)\n" , x , y) ;
                    little_circle = 0 ;
                    break;
            }
        }
        SDL_RenderPresent(sdlRenderer) ;
        SDL_Delay(1000/FPS) ;
        i++;
    }

    // Quitting window and program //
    SDL_DestroyWindow(sdlWindow) ;

    SDL_Quit() ;
    printf("all done") ;
    return  0 ;
}

