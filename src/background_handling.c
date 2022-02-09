//
// Created by Mahan on 1/30/2022.
//

#include "background_handling.h"
#include "map_handling.h"


void test_func()
{
    printf("this is a test from my header\n") ;
}

SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) {
    SDL_Surface *image = SDL_LoadBMP(image_path);
    /* Let the user know if the file failed to load */
    if (!image) {
        printf("Failed to load image at %s: %s\n", image_path, SDL_GetError());
        return 0;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, image);
    SDL_FreeSurface(image);
    image = NULL;
    return texture;
}

void InitializePotionGraphics ( SDL_Renderer* sdlRenderer , SDL_Texture** POTION_GRAPHIC)
{
    POTION_GRAPHIC[0] = getImageTexture(sdlRenderer , "../img/red_potion.bmp") ;
    POTION_GRAPHIC[1] = getImageTexture(sdlRenderer , "../img/red_potion.bmp") ;
    POTION_GRAPHIC[2] = getImageTexture(sdlRenderer , "../img/blue_potion.bmp") ;
    POTION_GRAPHIC[3] = getImageTexture(sdlRenderer , "../img/orange_potion.bmp") ;
    POTION_GRAPHIC[4] = getImageTexture(sdlRenderer , "../img/gold_potion.bmp")  ;
    POTION_GRAPHIC[5] = getImageTexture(sdlRenderer , "../img/silver_potion.bmp") ;
    POTION_GRAPHIC[6] = getImageTexture(sdlRenderer , "../img/green_potion.bmp") ;
    POTION_GRAPHIC[7] = getImageTexture(sdlRenderer , "../img/shield_potion.bmp") ;
}

void DrawBackground ( SDL_Renderer * sdlRenderer , SDL_Texture* Background_image , int HEIGHT , int WIDTH){
    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff ,0xff) ;
    SDL_RenderClear(sdlRenderer) ;
    SDL_Rect back_rect = {.x=0 , .y=0 , .h = HEIGHT , .w=WIDTH} ;
    SDL_RenderCopy( sdlRenderer , Background_image , NULL , &back_rect) ;
}

void IntroScreen ( SDL_Renderer* sdlRenderer , int WIDTH , int HEIGHT , SDL_Texture* intro_back)
{
    SDL_Rect back_rect = { .x=0 , .y=0 , .w=WIDTH , .h=HEIGHT} ;
    SDL_RenderCopy(sdlRenderer , intro_back , NULL , &back_rect) ;
    SDL_RenderPresent(sdlRenderer) ;
    SDL_Delay(5000) ;
}

// main menu
int StartMenu ( SDL_Window *sdlWindow , SDL_Renderer* sdlRenderer )
{
    SDL_Texture *back_img = getImageTexture(sdlRenderer , "../back.bmp" ) ;
    SDL_Rect back_rect = {.x=0 , .y=0 , .w=960 , .h=640};

    SDL_Texture *continue_img = getImageTexture(sdlRenderer , "../continue.bmp" ) ;
    SDL_Rect continue_rect = {.x = 400 , .y=300 , .w=160 , .h=60};

    SDL_Texture *start_img = getImageTexture(sdlRenderer , "../start.bmp" ) ;
    SDL_Rect start_rect = {.x=400 , .y=380 , .w=160 , .h=60 };

    SDL_Texture *leaderboard_img = getImageTexture(sdlRenderer , "../leaderboard.bmp" ) ;
    SDL_Rect leaderboard_rect = {.x=400 , .y=460 , .w=160 , .h=60 };

    SDL_Texture *quit_img = getImageTexture(sdlRenderer , "../start.bmp" ) ;
    SDL_Rect quit_rect = {.x=400 , .y=540 , .w=160 , .h=60 };

    int cond = 1 ;
    int return_code ;

    char* file_name = "../dat/tmp/cond.txt" ;
    FILE* should_con_file = fopen( file_name , "r") ;
    int to_continue = fgetc(should_con_file) ;
    fclose(should_con_file) ;


    while ( cond ) {

        SDL_RenderCopy(sdlRenderer, back_img, NULL, &back_rect);
        if ( to_continue == '1')
            SDL_RenderCopy(sdlRenderer, continue_img, NULL, &continue_rect);
        SDL_RenderCopy(sdlRenderer, start_img, NULL, &start_rect);
        SDL_RenderCopy(sdlRenderer, leaderboard_img, NULL, &leaderboard_rect);
        SDL_RenderCopy(sdlRenderer, quit_img, NULL, &quit_rect);

        SDL_Event sdlEvent ;
        while (SDL_PollEvent(&sdlEvent))
        {
            Sint32 x = sdlEvent.motion.x ;
            Sint32 y = sdlEvent.motion.y ;
            switch ( sdlEvent.type )
            {
                case SDL_MOUSEBUTTONDOWN:
                    if ( x>400 && x<560 && y>300 && y<360 && to_continue=='1')  // continue game
                    {
                        return_code = 0 ;
                        cond = 0 ;
                    }
                    else if ( x>400 && x<560 && y>380 && y<440 ) {              // new game
                        return_code = 1;
                        cond = 0;
                    }
                    else if ( x>400 && x<560 && y>460 && y<520 ) {              // leaderboard
                        return_code = 2;
                        cond = 0;
                    }
                    else if ( x>400 && x<560 && y>540 && y<600 ){               // quit game
                        return_code = -1 ;
                        cond = 0 ;
                    }
                    break;
                case SDL_QUIT:
                    return_code = -1 ;
                    cond = 0 ;
                    break;
            }
        }

        SDL_RenderPresent(sdlRenderer);
    }


    SDL_free(back_img) ;
    SDL_free(continue_img) ;
    SDL_free(start_img) ;
    SDL_free(leaderboard_img) ;
    SDL_free(quit_img) ;


    return return_code ;
}


// choosing map and starting new game
void NewGameSelection ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer )
{
    SDL_Texture *back_img = getImageTexture(sdlRenderer , "../back.bmp") ;
    SDL_Rect back_rect = {.x=0 , .y=0 , .w=960 , .h=640 };

    SDL_Texture *generate = getImageTexture(sdlRenderer , "../img/map-selection/create.bmp") ;


    int y = 300 ;
    SDL_Texture *small_3x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-small.bmp") ;
    SDL_Rect small_3x_rect  = {.x=240 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr1 = {.x = 240 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *normal_3x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-normal.bmp") ;
    SDL_Rect normal_3x_rect  = {.x=420 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr2 = {.x = 420 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *huge_3x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-huge.bmp") ;
    SDL_Rect huge_3x_rect = {.x=600 , .y=y , .w=120 , .h = 30 };
    SDL_Rect cr3 = {.x = 600 , .y=y+40 , .w = 120 , .h=30 };

    y+= 120 ;
    SDL_Texture *small_4x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-small.bmp") ;
    SDL_Rect small_4x_rect  = {.x=240 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr4 = {.x = 240 , .y=y+40 , .w = 120 , .h=30 };


    SDL_Texture *normal_4x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-normal.bmp") ;
    SDL_Rect normal_4x_rect  = {.x=420 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr5 = {.x = 420 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *huge_4x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-huge.bmp") ;
    SDL_Rect huge_4x_rect = {.x=600 , .y=y , .w=120 , .h = 30 };
    SDL_Rect cr6 = {.x = 600 , .y=y+40 , .w = 120 , .h=30 };

    y+= 120 ;
    SDL_Texture *small_5x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-small.bmp") ;
    SDL_Rect small_5x_rect  = {.x=240 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr7 = {.x = 240 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *normal_5x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-normal.bmp") ;
    SDL_Rect normal_5x_rect  = {.x=420 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr8 = {.x = 420 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *huge_5x_img = getImageTexture(sdlRenderer , "../img/map-selection/3x-huge.bmp") ;
    SDL_Rect huge_5x_rect = {.x=600 , .y=y , .w=120 , .h = 30 };
    SDL_Rect cr9 = {.x = 600 , .y=y+40 , .w = 120 , .h=30 };


    int cond = 1 ;
    while ( cond ) {
        SDL_RenderCopy(sdlRenderer, back_img, NULL, &back_rect);
        SDL_RenderCopy(sdlRenderer, small_3x_img, NULL, &small_3x_rect);
        SDL_RenderCopy(sdlRenderer, normal_3x_img, NULL, &normal_3x_rect);
        SDL_RenderCopy(sdlRenderer , huge_3x_img , NULL , &huge_3x_rect) ;

        SDL_RenderCopy(sdlRenderer, small_4x_img, NULL, &small_4x_rect);
        SDL_RenderCopy(sdlRenderer, normal_4x_img, NULL, &normal_4x_rect);
        SDL_RenderCopy(sdlRenderer , huge_4x_img , NULL , &huge_4x_rect) ;

        SDL_RenderCopy(sdlRenderer, small_5x_img, NULL, &small_5x_rect);
        SDL_RenderCopy(sdlRenderer, normal_5x_img, NULL, &normal_5x_rect);
        SDL_RenderCopy(sdlRenderer , huge_5x_img , NULL , &huge_5x_rect) ;

        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr1) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr2) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr3) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr4) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr5) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr6) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr7) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr8) ;
        SDL_RenderCopy(sdlRenderer , generate , NULL , &cr9) ;


        y -= 240 ;

        SDL_Event sdlEvent ;
        while (SDL_PollEvent(&sdlEvent))
        {
            Sint32 xcoord = sdlEvent.motion.x ;
            Sint32 ycoord = sdlEvent.motion.y ;
            y = 300 ;
            switch (sdlEvent.type) {
                case SDL_MOUSEBUTTONDOWN:
                    printf("%d %d\n" , xcoord , ycoord ) ;
                    printf("y is : %d\n" , y ) ;
                    if ( ycoord>y && ycoord<y+30 )          // ready 3x maps
                    {
                        if ( xcoord>240 && xcoord<360 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/3x-small-map.dat" , 960 , 640 , 60 ) ;
                        else if ( xcoord>420 && xcoord<540 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/3x-normal-map.dat" , 960 , 640 , 60 ) ;
                        else if ( xcoord>600 && xcoord<720 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/3x-huge-map.dat" , 960 , 640 , 60 ) ;
                    }

                    if ( ycoord>y+40 && ycoord<y+70 )       // create 3x map
                    {
                        printf("create\n") ;
                        if ( xcoord>240 && xcoord<360 )
                            StartNewGame(sdlWindow , sdlRenderer , 4 , 960 , 640 , 60 , 33 , 14 , 14 ) ;

                        else if ( xcoord>420 && xcoord<540 )
                            StartNewGame(sdlWindow , sdlRenderer , 4 , 960 , 640 , 60 , 30 , 16 , 16 ) ;

                        else if ( xcoord>600 && xcoord<720 )
                            StartNewGame(sdlWindow , sdlRenderer , 4 , 960 , 640 , 60 , 27 , 20 , 20 ) ;

                    }

                    if ( ycoord>y+120 && ycoord<y+150 )          // ready 4x maps
                    {
                        if ( xcoord>240 && xcoord<360 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/4x-small-map.dat" , 960 , 640 , 60 ) ;
                        else if ( xcoord>420 && xcoord<540 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/4x-normal-map.dat" , 960 , 640 , 60 ) ;
                        else if ( xcoord>600 && xcoord<720 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/4x-huge-map.dat" , 960 , 640 , 60 ) ;
                    }

                    if ( ycoord>y+160 && ycoord<y+190 )       // create 4x map
                    {
                        printf("create\n") ;
                        if ( xcoord>240 && xcoord<360 )
                            StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 33 , 14 , 14 ) ;

                        else if ( xcoord>420 && xcoord<540 )
                            StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 30 , 16 , 16 ) ;

                        else if ( xcoord>600 && xcoord<720 )
                            StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 27 , 20 , 20 ) ;

                    }

                    if ( ycoord>y+240 && ycoord<y+270 )          // ready 5x maps
                    {
                        if ( xcoord>240 && xcoord<360 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/5x-small-map.dat" , 960 , 640 , 60 ) ;
                        else if ( xcoord>420 && xcoord<540 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/5x-normal-map.dat" , 960 , 640 , 60 ) ;
                        else if ( xcoord>600 && xcoord<720 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/5x-huge-map.dat" , 960 , 640 , 60 ) ;
                    }

                    if ( ycoord>y+280 && ycoord<y+410 )       // create 5x map
                    {
                        printf("create\n") ;
                        if ( xcoord>240 && xcoord<360 )
                            StartNewGame(sdlWindow , sdlRenderer , 6 , 960 , 640 , 60 , 33 , 14 , 14 ) ;

                        else if ( xcoord>420 && xcoord<540 )
                            StartNewGame(sdlWindow , sdlRenderer , 6 , 960 , 640 , 60 , 30 , 16 , 16 ) ;

                        else if ( xcoord>600 && xcoord<720 )
                            StartNewGame(sdlWindow , sdlRenderer , 6 , 960 , 640 , 60 , 27 , 20 , 20 ) ;

                    }

                    break;
                case SDL_QUIT:
                    return;
            }
        }

        SDL_RenderPresent(sdlRenderer) ;
    }
}