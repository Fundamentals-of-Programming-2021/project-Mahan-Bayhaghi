#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>

//
// Created by Mahan on 1/30/2022.
//

#ifndef MAIN_C_BACKGROUND_HANDLING_H
#define MAIN_C_BACKGROUND_HANDLING_H
#endif //MAIN_C_BACKGROUND_HANDLING_H

#include <stdio.h>
#include <time.h>
#include <math.h>

void test_func() ;

SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) ;

int ShowMenu (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int WIDTH , int HEIGHT ,SDL_Texture *img ) ;