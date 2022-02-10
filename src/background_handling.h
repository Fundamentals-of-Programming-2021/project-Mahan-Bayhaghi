#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//
// Created by Mahan on 1/30/2022.
//

#ifndef MAIN_C_BACKGROUND_HANDLING_H
#define MAIN_C_BACKGROUND_HANDLING_H
#endif //MAIN_C_BACKGROUND_HANDLING_H

#include <stdio.h>
#include <time.h>
#include <math.h>

typedef struct OneScore {
    int user_id ;
    int games_played ;
    int total_score ;
} OneScore ;

typedef struct OneUser {
    int user_id ;
    char* user_name ;
} OneUser ;

typedef struct OneStanding {
    char* name ;
    char* type ;
    int games_played ;
    int total_score ;
} OneStanding ;

void test_func() ;

// a function to get texture from image
SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) ;

// a function to Initialize graphics of potions
void InitializePotionGraphics ( SDL_Renderer* sdlRenderer , SDL_Texture** POTION_GRAPHIC) ;

// a function to Draw game background
void DrawBackground ( SDL_Renderer * sdlRenderer , SDL_Texture* Background_image , int HEIGHT , int WIDTH) ;

// a simple intro screen
void IntroScreen ( SDL_Renderer* sdlRenderer , int WIDTH , int HEIGHT , SDL_Texture* intro_back) ;


// a function to show start menu
int StartMenu ( SDL_Window *sdlWindow , SDL_Renderer* sdlRenderer , int user_id ) ;

// page you see when starting a new game
void NewGameSelection ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int user_id ) ;

// login to game
int Login ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer ) ;


// a function to get username
void GetName ( SDL_Window*  sdlWindow , SDL_Renderer* sdlRenderer , int id_to_check ) ;


// a function to update leaderboardd
void UpdateLeaderboard ( int* GLOBAL_POINTS_ARRAY , int NUM_PLAYERS , int user_id ) ;


// a function to show leaderboard
void ShowLeaderboard ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer ) ;

