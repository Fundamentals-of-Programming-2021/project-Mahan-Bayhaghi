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

int ShowMenu (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int WIDTH , int HEIGHT , SDL_Texture* img)
{
    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};

    SDL_Texture *start_btn = getImageTexture(sdlRenderer , "../start.bmp") ;
    SDL_Rect start_rect = {.x=350 , .y=300 , .w=100 , .h=40};
    SDL_Rect bigger_start_rect = {.x=340 , .y=290 , .w=120 , .h=50};

    SDL_Texture *quit_btn = getImageTexture(sdlRenderer , "../start.bmp") ;
    SDL_Rect quit_rect = {.x=350 , .y=500 , .w=100 , .h=40};
    SDL_Rect bigger_quit_rect = {.x=340 , .y=490 , .w=120 , .h=50};

    SDL_bool shall_exit_menu = SDL_FALSE ;
    SDL_Event sdlEvent ;

    while ( !shall_exit_menu )
    {
        SDL_RenderClear(sdlRenderer) ;
        SDL_SetRenderDrawColor(sdlRenderer , 0x00 , 0x00 , 0x55 ,0xff) ;
        SDL_RenderCopy(sdlRenderer, img , NULL, &texture_rect);
        SDL_RenderCopy(sdlRenderer, start_btn , NULL, &start_rect);
        SDL_RenderCopy(sdlRenderer, quit_btn , NULL, &quit_rect);


        int x = sdlEvent.motion.x ;
        int y = sdlEvent.motion.y ;
        if ( x<450 && x>350 && y<350 && y>300)
            SDL_RenderCopy(sdlRenderer, start_btn , NULL, &bigger_start_rect);
        if ( x<450 && x>350 && y<540 && y>500 )
            SDL_RenderCopy(sdlRenderer , quit_btn , NULL , &bigger_quit_rect) ;

        while (SDL_PollEvent(&sdlEvent))
        {
            switch (sdlEvent.type) {
                case SDL_QUIT :
                    printf("user quitted game !") ;
                    SDL_DestroyRenderer(sdlRenderer) ;
                    SDL_DestroyWindow(sdlWindow) ;
                    SDL_Quit() ;
                    return 0;
                case SDL_MOUSEBUTTONUP :
                    if ( x<450 && x>350 && y<350 && y>300)
                    {
                        printf("game started !") ;
                        SDL_Delay(500) ;
                        return 1;
                    }
                    if ( x<450 && x>350 && y<540 && y>500 )
                    {
                        printf("user quitted game !") ;
                        SDL_DestroyRenderer(sdlRenderer) ;
                        SDL_DestroyWindow(sdlWindow) ;
                        SDL_Quit() ;
                        return 0;
                    }
                    break;
            }
        }
        SDL_RenderPresent(sdlRenderer) ;
    }
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
                    if ( x>400 && x<560 && y>300 && y<360 && to_continue=='1')
                    {
                        return_code = 0 ;
                        cond = 0 ;
                    }
                    else if ( x>400 && x<560 && y>380 && y<440 ) {
                        return_code = 1;
                        cond = 0;
                    }
                    else if ( x>400 && x<560 && y>460 && y<520 ) {
                        return_code = 2;
                        cond = 0;
                    }
                    else if ( x>400 && x<560 && y>540 && y<600 ){
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
void NewGame ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer )
{
    SDL_Texture *back_img = getImageTexture(sdlRenderer , "../back.bmp") ;
    SDL_Rect back_rect = {.x=0 , .y=0 , .w=960 , .h=640 };

    SDL_Texture *map1_img = getImageTexture(sdlRenderer , "../start.bmp") ;
    SDL_Rect map1_rect  = {.x=120 , .y=200 , .w=60 , .h=60 };

    SDL_Texture *map2_img = getImageTexture(sdlRenderer , "../start.bmp") ;
    SDL_Rect map2_rect  = {.x=200 , .y=200 , .w=60 , .h=60 };

    int cond = 1 ;
    while ( cond ) {
        SDL_RenderCopy(sdlRenderer, back_img, NULL, &back_rect);
        SDL_RenderCopy(sdlRenderer, map1_img, NULL, &map1_rect);
        SDL_RenderCopy(sdlRenderer, map2_img, NULL, &map2_rect);

        SDL_Event sdlEvent ;
        while (SDL_PollEvent(&sdlEvent))
        {
            Sint32 x = sdlEvent.motion.x ;
            Sint32 y = sdlEvent.motion.y ;
            switch (sdlEvent.type) {
                case SDL_MOUSEBUTTONDOWN:
                    if ( x>120 && x<180 && y>200 && y<260 )
                        LoadGame(sdlWindow , sdlRenderer , "../dat/tmp/map_data.dat" , 960 , 640 , 60 , 27 ) ;
//                        StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 27 , 18 , 18 ) ;
                    if ( x>200 && x<260 && y>200 && y<260 )
                        StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 30 , 18 , 18 ) ;
                    break;
                case SDL_QUIT:
                    return;
            }
        }

        SDL_RenderPresent(sdlRenderer) ;
    }
}