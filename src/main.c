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
const int HEXAGON_A = 30 ;
const int NUM_OF_ROWS = 8 ;
const int NUM_OF_COLS = 16 ;

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
    land* map_arr = malloc(sizeof(land) * NUM_OF_COLS*NUM_OF_ROWS ) ;
    map_arr = GENERATE_HEXAGON_RANDOM_MAP(sdlWindow , sdlRenderer , 3 , WIDTH , HEIGHT , map_arr , HEXAGON_A ) ;
    int NUM_OF_CELLS = ShowHexagonBackground(sdlWindow , sdlRenderer , map_arr , 84 , HEXAGON_A) ;


    // getting background picture as texture
    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};


    // getting go_back_to_menu picture as texture
    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};

    int i=1 ;
    SDL_bool shallExit = SDL_FALSE ;
    SDL_bool shallShowMenu = SDL_TRUE ;

    int click_status = 0 ;

    Sint16 Origin_x = 0 ;
    Sint16 Origin_y = 0 ;
    int Origin_counter = 0 ;
    Sint16 Destination_x = 0 ;
    Sint16 Destination_y = 0 ;


    OneSoldier** AllSoldiersArray = calloc(2*NUM_OF_CELLS , sizeof(OneSoldier*) ) ;

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

        ShowHexagonBackground(sdlWindow , sdlRenderer , map_arr , NUM_OF_CELLS , HEXAGON_A) ;

        if ( i%100 == 0 )
            AddSoldiers(map_arr , NUM_OF_CELLS ) ;

        SDL_Event sdlEvent;
        land clicked_cell_info ;

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
            Sint16 x = sdlEvent.motion.x ;
            Sint16 y = sdlEvent.motion.y ;

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

                    clicked_cell_info = GiveClickedCellInfo(x , y , map_arr , NUM_OF_CELLS , 35) ;

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

                        if ( Origin_counter != clicked_cell_info.counter )
                        {
                            AllSoldiersArray[0] = malloc(sizeof(OneSoldier) *
                                    map_arr[Origin_counter].soldiers_number) ;

                            AllSoldiersArray[0][0].destination_x = clicked_cell_info.x ;
                            AllSoldiersArray[0][0].destination_y = clicked_cell_info.y ;
                            AllSoldiersArray[0][0].destination_counter = clicked_cell_info.counter ;
                            AllSoldiersArray[0][0].owner_id = map_arr[Origin_counter].owner_id ;
                            AllSoldiersArray[0][0].power = 1 ;

                            // delta management //
                            float delta_y = clicked_cell_info.y - map_arr[Origin_counter].y ;
                            float delta_x = clicked_cell_info.x - map_arr[Origin_counter].x ;
                            double theta = delta_y / delta_x ;
                            AllSoldiersArray[0][0].verticalSpeed = 6.0 * sin(atan(theta) ) ;
                            if ( delta_y > 0 )
                                AllSoldiersArray[0][0].verticalSpeed = fabs(AllSoldiersArray[0][0].verticalSpeed) ;
                            else if ( delta_y <0 )
                                AllSoldiersArray[0][0].verticalSpeed = -1 * fabs(AllSoldiersArray[0][0].verticalSpeed) ;

                            AllSoldiersArray[0][0].horizontalSpeed = 6.0 * cos(atan(theta) ) ;
                            if ( delta_x > 0 )
                                AllSoldiersArray[0][0].horizontalSpeed = fabs(AllSoldiersArray[0][0].horizontalSpeed) ;
                            else if ( delta_x <0 )
                                AllSoldiersArray[0][0].horizontalSpeed = -1 * fabs(AllSoldiersArray[0][0].horizontalSpeed) ;


                            AllSoldiersArray[0][0].x = map_arr[Origin_counter].x + AllSoldiersArray[0][0].verticalSpeed ;
                            AllSoldiersArray[0][0].y = map_arr[Origin_counter].y + AllSoldiersArray[0][0].horizontalSpeed ;

                            for ( int temp_c = 1 ; temp_c<map_arr[Origin_counter].soldiers_number ; temp_c++ )
                            {
                                AllSoldiersArray[0][temp_c].x = AllSoldiersArray[0][temp_c-1].x - 2*AllSoldiersArray[0][0].horizontalSpeed ;
                                AllSoldiersArray[0][temp_c].y = AllSoldiersArray[0][temp_c-1].y - 2*AllSoldiersArray[0][0].verticalSpeed ;
                                AllSoldiersArray[0][temp_c].destination_x = AllSoldiersArray[0][0].destination_x ;
                                AllSoldiersArray[0][temp_c].destination_y = AllSoldiersArray[0][0].destination_y ;
                                AllSoldiersArray[0][temp_c].destination_counter = clicked_cell_info.counter ;
                                AllSoldiersArray[0][temp_c].owner_id = map_arr[Origin_counter].owner_id ;
                                AllSoldiersArray[0][temp_c].power = 1 ;
                            }
                            printf("\n-------------------\n%f %f\n" , AllSoldiersArray[0][0].x , AllSoldiersArray[0][0].y ) ;
                            printf("atan{delta_y / delta_x} is : %lf\n" , theta ) ;
                            printf("sin : %lf | cos : %lf\n" , sin(theta) , cos(theta) ) ;
                            printf("%f %f\n" , AllSoldiersArray[0][0].verticalSpeed , AllSoldiersArray[0][0].horizontalSpeed) ;
                        }

//                        if ( Origin_counter != clicked_cell_info.counter ) {
//                            printf("origin counter is : %d\n", Origin_counter);
//                            printf("destination counter is : %d\n", clicked_cell_info.counter);
//
//                            if ( map_arr[clicked_cell_info.counter].owner_id == map_arr[Origin_counter].owner_id )
//                                map_arr[clicked_cell_info.counter].soldiers_number += map_arr[Origin_counter].soldiers_number ;
//                            else if ( map_arr[Origin_counter].soldiers_number > map_arr[clicked_cell_info.counter].soldiers_number )
//                            {
//                                map_arr[clicked_cell_info.counter].soldiers_number = map_arr[Origin_counter].soldiers_number - map_arr[clicked_cell_info.counter].soldiers_number ;
//                                map_arr[clicked_cell_info.counter].owner_id = map_arr[Origin_counter].owner_id ;
//                            }
//                            else if ( map_arr[Origin_counter].soldiers_number <= map_arr[clicked_cell_info.counter].soldiers_number )
//                            {
//                                map_arr[clicked_cell_info.counter].soldiers_number = map_arr[clicked_cell_info.counter].soldiers_number - map_arr[Origin_counter].soldiers_number ;
//                            }
//
//                            map_arr[Origin_counter].soldiers_number = 0 ;
//                        }
                    }

                    SDL_Delay(200) ;
                    break;

                case SDL_MOUSEBUTTONUP:
                    break;
            }
        }

        for ( int temp_c=0 ; temp_c<25 ; temp_c++ ) {
            if (AllSoldiersArray[0] != NULL && AllSoldiersArray[0][temp_c].x >= 0) {
                AllSoldiersArray[0][temp_c].x += AllSoldiersArray[0][0].horizontalSpeed;
                AllSoldiersArray[0][temp_c].y += AllSoldiersArray[0][0].verticalSpeed;
                int r = AllSoldiersArray[0][temp_c].power * 2 ;

                filledCircleColor(sdlRenderer, AllSoldiersArray[0][temp_c].x, AllSoldiersArray[0][temp_c].y, 2 * r, 0xffff00ff);

                printf("[%d] --> x : %lf | des_x : %lf\n", temp_c ,
                       AllSoldiersArray[0][temp_c].x, AllSoldiersArray[0][temp_c].destination_x);

                if (fabs(AllSoldiersArray[0][temp_c].x / 40 - AllSoldiersArray[0][temp_c].destination_x / 40) < 0.2) {
                    AllSoldiersArray[0][temp_c].x = -10;
                    if (map_arr[AllSoldiersArray[0][temp_c].destination_counter].owner_id != AllSoldiersArray[0][temp_c].owner_id)
                        map_arr[AllSoldiersArray[0][temp_c].destination_counter].soldiers_number -= AllSoldiersArray[0][temp_c].power;
                    else
                        map_arr[AllSoldiersArray[0][temp_c].destination_counter].soldiers_number += AllSoldiersArray[0][temp_c].power;

                }
            }
        }


        SDL_RenderPresent(sdlRenderer) ;
        SDL_Delay(1000/FPS) ;
        i++;

    }





    // Freeing some memory for god's sake
    SDL_free(img) ;
    SDL_free(back_to_menu) ;
    free(AllSoldiersArray) ;

    // Destroying window and program //
    SDL_DestroyTexture(img) ;
    SDL_DestroyRenderer(sdlRenderer) ;
    SDL_DestroyWindow(sdlWindow) ;

    // Quitting SDL and subsystems
    SDL_Quit() ;
    printf("all done") ;
    return  0 ;
}

