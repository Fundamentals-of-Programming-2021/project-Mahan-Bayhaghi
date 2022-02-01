//
// Created by Mahan on 1/30/2022.
//

#include "background_handling.h"


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
//    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
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

