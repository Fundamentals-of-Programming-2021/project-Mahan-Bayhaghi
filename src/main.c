#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int WIDTH = 800 ;
const int HEIGHT = 600 ;
const int FPS = 80 ;
const int PRO_RATE = 3 ;
const int INIT_SOLDIER = 25 ;
const int NUMBER_OF_PLAYERS = 3 ;

Uint32 COLORS[4] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffb00500 };

typedef  struct properties {
    int state_condition  ;          // -1 --> null // 0 --> neutral // 1 --> in_use //
    int owner ;                     // a number between 0 to (num_of_players) // 0 for neutral // other nums for players //
    int soldiers ;                  // always between 0 and 120 //
    int soldier_production_rate ;   // 0 for non-military and PRO_RATE for others //
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
            printf("%4d" , arr[i][j].soldiers ) ;
        printf("\n") ;
    }
}

void RANDOMIZE_MAP ( properties arr[10][20] , int players_num )
{
    // 0 --> NEUTRAL LAND WITH NO MILITARY BASE
    // 1 TO players_num --> PLAYERS' LAND WITH MILITARY BASE AT THE BEGINNING OF THE GAME
    // ALL PLAYERS WILL HAVE AT LEAST 1 AND AT MAX 3 LANDS AT THE BEGINNING OF THE GAME
    srand(time(0)) ;
    int land_num ;
    int rand_x ;
    int rand_y ;
    for ( int i=1 ; i<=players_num ; i++ )
    {
        land_num = rand() % 2 + 1 ;
        for ( land_num ; land_num>0 ; land_num-- )
        {
            rand_x = rand() % 20 ;
            rand_y = rand() % 10 ;
            while ( arr[rand_y][rand_x].owner!=0 || arr[rand_y][rand_x].soldier_production_rate==0 )
            {
                rand_x = rand() % 20 ;
                rand_y = rand() % 10 ;
            }
            arr[rand_y][rand_x].owner = i ;
            arr[rand_y][rand_x].state_condition = 0 ;
        }
    }
}

void DRAW_LAND ( SDL_Renderer *renderer , properties land , int length , int height , Sint16 x , Sint16 y )
{
//    if ( land.state_condition == -1 )
//        boxColor(renderer , x-length/2 , y-height/2 , x+length/2 , y+height/2 , 0xffffffff ) ;
    if ( land.state_condition != -1 )
    {
        SDL_Rect border = { .x=x-length/2 , .y=y-height/2 , .w = length , .h=height } ;
        boxColor(renderer, x-length/2, y-height/2, x+length/2, y+height/2, COLORS[land.owner]);
        if ( land.soldier_production_rate != 0 ) {
            filledCircleColor(renderer, (x), (y), 2, 0xff000000);
            char* buffer = malloc(sizeof (char) * 20) ;
            sprintf(buffer , "%d" , land.soldiers) ;
            stringRGBA(renderer , x-5 , y+5 , buffer , 0 , 0 , 0 , 255) ;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderDrawRect(renderer, &border);
    }
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
            x += length ;
        }
        y += height ;
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Window *sdlWindow = SDL_CreateWindow("State.io" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;

    properties map_array[10][20] ;
    create_map_array("hello" , map_array) ;
    RANDOMIZE_MAP(map_array , NUMBER_OF_PLAYERS) ;


    int i=0 ;

    SDL_bool shallExit = SDL_FALSE ;

    int little_circle = 0 ;
    while ( !shallExit )
    {
        i++ ;
        SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff ,0xff) ;
        SDL_RenderClear(sdlRenderer) ;

        GAME_BACKGROUND(sdlRenderer , map_array , 65 , 45) ;

//        char* buffer = malloc(sizeof (char) * 50) ;
//        sprintf(buffer , "my score is %d " , i/75) ;
//        stringRGBA(sdlRenderer , 5 , 5 , buffer , 0 , 0 , 0 , 255) ;


        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            int x = sdlEvent.motion.x ;
            int y = sdlEvent.motion.y ;
            switch (sdlEvent.type)
            {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEMOTION:
                    printf("mouse is moving\n") ;
                    printf("x is : %d   y is : %d\n" , sdlEvent.motion.x , sdlEvent.motion.y);
                    if ( little_circle == 1)
                        filledCircleColor(sdlRenderer , x , y , 5 , 0xff4f5500) ;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    little_circle = 1 ;
                    printf("mouse is being pushed at (%d , %d)\n" , x , y) ;
                    printf("dar khane (%d,%d) array hastim " , x/65 , y/45 ) ;
                    break;
                case SDL_MOUSEBUTTONUP:
                    printf("mouse is up now\n") ;
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

