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

// a function to get texture from image
SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) ;

// a function to show menu
int ShowMenu (SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int WIDTH , int HEIGHT ,SDL_Texture *img ) ;

// a function to Initialize graphics of potions
void InitializePotionGraphics ( SDL_Renderer* sdlRenderer , SDL_Texture** POTION_GRAPHIC) ;

// a function to Draw game background
void DrawBackground ( SDL_Renderer * sdlRenderer , SDL_Texture* Background_image , int HEIGHT , int WIDTH) ;

// a simple intro screen
void IntroScreen ( SDL_Renderer* sdlRenderer , int WIDTH , int HEIGHT , SDL_Texture* intro_back) ;


// a function to show start menu
int StartMenu ( SDL_Window *sdlWindow , SDL_Renderer* sdlRenderer) ;




void NewGame ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer ) ;