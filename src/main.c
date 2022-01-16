#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int WIDTH = 800 ;
const int HEIGHT = 600 ;
const int FPS = 60 ;
const int PRO_RATE = 3 ;
const int INIT_SOLDIER = 25 ;

Uint32 COLORS[4] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffffffff };

typedef  struct properties {
    int state_condition  ;          // -1 --> null // 0 --> neutral // 1 --> in_use //
    int owner ;                     // a number between 0 to (num_of_players) // 0 for neutral // other nums for players //
    int soldiers ;                  // always between 0 and 120 //
    int soldier_production_rate ;   // 0 for non-padegans and PRO_RATE for others //
} properties ;

void create_map_array ( const char* map_name , properties arr[10][20])
{
    FILE *map_txt = fopen("../logs/template1.txt" , "r") ;
    char c = ' ' ;
    for ( int i=0 ; c != EOF ; i++)
    {
        for ( int j=0 ; j<20 ; j++)
        {
            c = fgetc(map_txt) ;
            if ( c == '0' )   // BLACK IN MAP
            {
                arr[i][j].state_condition = -1 ;
                arr[i][j].owner = -1 ;
                arr[i][j].soldiers = 0 ;
                arr[i][j].soldier_production_rate = 0 ;
            }
            else if ( c=='L' )  // FREE LAND WITH MILITARY BASE
            {
                arr[i][j].state_condition = 0 ;
                arr[i][j].owner = 0 ;
                arr[i][j].soldiers = INIT_SOLDIER ;
                arr[i][j].soldier_production_rate = PRO_RATE ;
            }
            else if ( c =='M' ) // FREE LAND WITH NO MILITARY BASE
            {
                arr[i][j].state_condition = 0 ;
                arr[i][j].owner = 0 ;
                arr[i][j].soldiers = INIT_SOLDIER ;
                arr[i][j].soldier_production_rate = 0 ;
            }
            c = fgetc(map_txt) ;
        }
    }
    printf("map is \n") ;
    for ( int i=0 ; i<10 ; i++)
    {
        for ( int j=0 ; j<20 ; j++)
            printf("%d " , arr[i][j].soldiers ) ;
        printf("\n") ;
    }
}

void DRAW_LAND ( SDL_Renderer *renderer , properties land , int length , int height , Sint16 x , Sint16 y )
{
//    SDL_RenderDrawLine( renderer , x-length/2 , y-height/2 , x+length/2 , y-height/2) ;
    SDL_Rect border = { .x=x-length/2 , .y=y-height/2 , .w = length , .h=height } ;

    if ( land.state_condition == -1 )
        boxColor(renderer , x-length/2 , y-height/2 , x+length/2 , y+height/2 , 0xffffff00 ) ;
    else
    {
        boxColor(renderer, x - length / 2, y - height / 2, x + length / 2, y + height / 2, COLORS[land.owner]);
        if ( land.soldier_production_rate != 0 )
            filledCircleColor(renderer , x-length/4 , y-height/4 ,  2 , 0xff000000) ;
    }
    SDL_SetRenderDrawColor(renderer , 0x00 , 0x00 , 0x00 , 0xff) ;
    SDL_RenderDrawRect(renderer , &border ) ;
}

void GAME_BACKGROUND (SDL_Renderer *renderer , properties arr[10][20] , int length , int height )
{
    int x = length/2 ;
    int y = height/2 ;
    for ( int i=0 ; i<10 ; i++)
    {
        x = length/2 ;
        for ( int j=0 ; j<20 ; j++)
        {
            DRAW_LAND(renderer , arr[i][j] , length , height , x , y ) ;
            x += length/2 ;
        }
        y += length/2 ;
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    properties map_array[10][20] ;
    create_map_array("hello" , map_array) ;

    SDL_Window *sdlWindow = SDL_CreateWindow("Stateio" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;

    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;

    int i=0 ;
    int x = 400 ;
    int y = 300 ;
    while ( i<1000 )
    {
        SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff ,0xff) ;
        SDL_RenderClear(sdlRenderer) ;
        GAME_BACKGROUND(sdlRenderer , map_array , 65 , 60) ;
        SDL_RenderPresent(sdlRenderer) ;
        SDL_Delay(1000/FPS) ;
        i++;
    }

    SDL_DestroyWindow(sdlWindow) ;
    SDL_Quit() ;
    printf("all done") ;
    return  0 ;
}

