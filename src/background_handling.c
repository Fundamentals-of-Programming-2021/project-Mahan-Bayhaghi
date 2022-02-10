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
int StartMenu ( SDL_Window *sdlWindow , SDL_Renderer* sdlRenderer , int user_id )
{
    SDL_Texture *back_img = getImageTexture(sdlRenderer , "../back.bmp" ) ;
    SDL_Rect back_rect = {.x=0 , .y=0 , .w=960 , .h=640};

    SDL_Texture *continue_img = getImageTexture(sdlRenderer , "../continue.bmp" ) ;
    SDL_Rect continue_rect = {.x = 400 , .y=300 , .w=160 , .h=40};

    SDL_Texture *start_img = getImageTexture(sdlRenderer , "../start.bmp" ) ;
    SDL_Rect start_rect = {.x=400 , .y=380 , .w=160 , .h=60 };

    SDL_Texture *leaderboard_img = getImageTexture(sdlRenderer , "../scores.bmp" ) ;
    SDL_Rect leaderboard_rect = {.x=400 , .y=460 , .w=160 , .h=40 };

    SDL_Texture *quit_img = getImageTexture(sdlRenderer , "../quit.bmp" ) ;
    SDL_Rect quit_rect = {.x=400 , .y=540 , .w=160 , .h=55 };

    FILE* log_info = fopen("../dat/usr/usernames.txt" , "r") ;
    char* logger_username = malloc(sizeof(char) * 16 ) ;
    for ( int i=0 ; i<6 ; i++)
    {
        int tmp ; char* tmpp = (char*) malloc(sizeof(char) * 16 );
        fscanf(log_info , "%d~%s\n" , &tmp , tmpp ) ;
        if ( tmp == user_id ) {
            logger_username = tmpp;
            break;
        }
    }
    fclose(log_info) ;

    int cond = 1 ;
    int return_code ;

    char* file_name = "../dat/tmp/cond.txt" ;
    FILE* should_con_file = fopen( file_name , "r") ;
    int to_continue = fgetc(should_con_file) ;
    fclose(should_con_file) ;
    char* login_info = malloc(sizeof(char) * 40 ) ;
    sprintf(login_info , "logged in as %s" , logger_username) ;
    char* brought_to_you = "version 1.2 | brought to you by Mahan Bayhaghi" ;

    while ( cond ) {
        SDL_RenderCopy(sdlRenderer, back_img, NULL, &back_rect);
        if ( to_continue == '1')
            SDL_RenderCopy(sdlRenderer, continue_img, NULL, &continue_rect);
        SDL_RenderCopy(sdlRenderer, start_img, NULL, &start_rect);
        SDL_RenderCopy(sdlRenderer, leaderboard_img, NULL, &leaderboard_rect);
        SDL_RenderCopy(sdlRenderer, quit_img, NULL, &quit_rect);
        stringColor(sdlRenderer , 10 , 10 , login_info , 0x66ffffff) ;
        stringColor(sdlRenderer , 10 , 625 , brought_to_you , 0x66000000) ;

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

    SDL_DestroyTexture(back_img) ;
    SDL_DestroyTexture(continue_img) ;
    SDL_DestroyTexture(start_img) ;
    SDL_DestroyTexture(leaderboard_img) ;
    SDL_DestroyTexture(quit_img) ;

    return return_code ;
}


// choosing map and starting new game
void NewGameSelection ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer , int user_id )
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
    SDL_Texture *small_4x_img = getImageTexture(sdlRenderer , "../img/map-selection/4x-small.bmp") ;
    SDL_Rect small_4x_rect  = {.x=240 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr4 = {.x = 240 , .y=y+40 , .w = 120 , .h=30 };


    SDL_Texture *normal_4x_img = getImageTexture(sdlRenderer , "../img/map-selection/4x-normal.bmp") ;
    SDL_Rect normal_4x_rect  = {.x=420 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr5 = {.x = 420 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *huge_4x_img = getImageTexture(sdlRenderer , "../img/map-selection/4x-huge.bmp") ;
    SDL_Rect huge_4x_rect = {.x=600 , .y=y , .w=120 , .h = 30 };
    SDL_Rect cr6 = {.x = 600 , .y=y+40 , .w = 120 , .h=30 };

    y+= 120 ;
    SDL_Texture *small_5x_img = getImageTexture(sdlRenderer , "../img/map-selection/5x-small.bmp") ;
    SDL_Rect small_5x_rect  = {.x=240 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr7 = {.x = 240 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *normal_5x_img = getImageTexture(sdlRenderer , "../img/map-selection/5x-normal.bmp") ;
    SDL_Rect normal_5x_rect  = {.x=420 , .y=y , .w=120 , .h=30 };
    SDL_Rect cr8 = {.x = 420 , .y=y+40 , .w = 120 , .h=30 };

    SDL_Texture *huge_5x_img = getImageTexture(sdlRenderer , "../img/map-selection/5x-huge.bmp") ;
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
                    if ( ycoord>y && ycoord<y+30 )          // ready 3x maps
                    {
                        if ( xcoord>240 && xcoord<360 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/3x-small-map.dat" , 960 , 640 , 60 , user_id) ;
                        else if ( xcoord>420 && xcoord<540 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/3x-normal-map.dat" , 960 , 640 , 60 , user_id) ;
                        else if ( xcoord>600 && xcoord<720 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/3x-huge-map.dat" , 960 , 640 , 60 , user_id ) ;
                    }

                    if ( ycoord>y+40 && ycoord<y+70 )       // create 3x map
                    {
                        if ( xcoord>240 && xcoord<360 )
                            StartNewGame(sdlWindow , sdlRenderer , 4 , 960 , 640 , 60 , 33 , 14 , 14 , user_id) ;

                        else if ( xcoord>420 && xcoord<540 )
                            StartNewGame(sdlWindow , sdlRenderer , 4 , 960 , 640 , 60 , 30 , 16 , 16 , user_id) ;

                        else if ( xcoord>600 && xcoord<720 )
                            StartNewGame(sdlWindow , sdlRenderer , 4 , 960 , 640 , 60 , 27 , 20 , 20 , user_id) ;
                    }

                    if ( ycoord>y+120 && ycoord<y+150 )          // ready 4x maps
                    {
                        if ( xcoord>240 && xcoord<360 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/4x-small-map.dat" , 960 , 640 , 60 , user_id ) ;
                        else if ( xcoord>420 && xcoord<540 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/4x-normal-map.dat" , 960 , 640 , 60 , user_id ) ;
                        else if ( xcoord>600 && xcoord<720 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/4x-huge-map.dat" , 960 , 640 , 60 , user_id ) ;
                    }

                    if ( ycoord>y+160 && ycoord<y+190 )       // create 4x map
                    {
                        if ( xcoord>240 && xcoord<360 )
                            StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 33 , 14 , 14 , user_id) ;

                        else if ( xcoord>420 && xcoord<540 )
                            StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 30 , 16 , 16 , user_id) ;

                        else if ( xcoord>600 && xcoord<720 )
                            StartNewGame(sdlWindow , sdlRenderer , 5 , 960 , 640 , 60 , 27 , 20 , 20 , user_id) ;
                    }

                    if ( ycoord>y+240 && ycoord<y+270 )          // ready 5x maps
                    {
                        if ( xcoord>240 && xcoord<360 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/5x-small-map.dat" , 960 , 640 , 80 , user_id) ;
                        else if ( xcoord>420 && xcoord<540 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/5x-normal-map.dat" , 960 , 640 , 80 , user_id) ;
                        else if ( xcoord>600 && xcoord<720 )
                            LoadGame(sdlWindow , sdlRenderer , "../dat/map/5x-huge-map.dat" , 960 , 640 , 80 , user_id) ;
                    }

                    if ( ycoord>y+280 && ycoord<y+410 )       // create 5x map
                    {
                        if ( xcoord>240 && xcoord<360 )
                            StartNewGame(sdlWindow , sdlRenderer , 6 , 960 , 640 , 80 , 33 , 14 , 14 , user_id) ;

                        else if ( xcoord>420 && xcoord<540 )
                            StartNewGame(sdlWindow , sdlRenderer , 6 , 960 , 640 , 80 , 30 , 16 , 16 , user_id) ;

                        else if ( xcoord>600 && xcoord<720 )
                            StartNewGame(sdlWindow , sdlRenderer , 6 , 960 , 640 , 80 , 27 , 20 , 20 , user_id) ;
                    }
                    cond = 0 ;
                    break;
                case SDL_QUIT:
                    cond = 0 ;
            }
        }
        SDL_RenderPresent(sdlRenderer) ;
    }

    SDL_free(&small_3x_rect) ;
    SDL_free(&normal_3x_rect) ;
    SDL_free(&huge_3x_rect) ;
    SDL_free(&small_4x_rect) ;
    SDL_free(&normal_4x_rect) ;
    SDL_free(&huge_4x_rect) ;
    SDL_free(&small_5x_rect) ;
    SDL_free(&normal_5x_rect) ;
    SDL_free(&huge_5x_rect) ;

    SDL_free(&cr1) ;
    SDL_free(&cr2) ;
    SDL_free(&cr3) ;
    SDL_free(&cr4) ;
    SDL_free(&cr5) ;
    SDL_free(&cr6) ;
    SDL_free(&cr7) ;
    SDL_free(&cr8) ;
    SDL_free(&cr9) ;

    SDL_free(&back_rect) ;

    SDL_DestroyTexture(small_3x_img) ;
    SDL_DestroyTexture(normal_3x_img) ;
    SDL_DestroyTexture(huge_3x_img) ;
    SDL_DestroyTexture(small_4x_img) ;
    SDL_DestroyTexture(normal_4x_img) ;
    SDL_DestroyTexture(huge_4x_img) ;
    SDL_DestroyTexture(small_5x_img) ;
    SDL_DestroyTexture(normal_5x_img) ;
    SDL_DestroyTexture(huge_5x_img)  ;
    SDL_DestroyTexture(generate)  ;
    SDL_DestroyTexture(back_img) ;
}


int Login ( SDL_Window *sdlWindow , SDL_Renderer *sdlRenderer )
{
    FILE* scores_txt = fopen("../dat/usr/scores.txt" , "r" ) ;
    int num_of_users ;
    OneScore *AllScoresArray = malloc(sizeof(OneScore) * 6) ;
    for ( int i=0 ; i<6 ; i++)
    {
        int us_id , gp , ts ;
        fscanf(scores_txt , "%d~%d~%d\n" , &us_id , &gp , &ts ) ;
        printf("%d~%d~%d\n" , us_id , gp , ts) ;
        AllScoresArray[i].user_id = us_id ;
        AllScoresArray[i].games_played = gp ;
        AllScoresArray[i].total_score = ts ;
    }
    fclose(scores_txt) ;

    int x = 300 ;
    int y = 400 ;

    SDL_Texture *back = getImageTexture(sdlRenderer , "../img/saves/hex-back.bmp") ;
    SDL_Rect back_rect = {.x=0 , .y=0 , .w=960 , .h=1200};


    SDL_Texture *save_1_red = getImageTexture(sdlRenderer , "../img/saves/save-1-red.bmp") ;
    SDL_Texture *save_1_green = getImageTexture(sdlRenderer , "../img/saves/save-1-green.bmp") ;
    SDL_Rect save_1 = {.x=x , .y=y , .h=40 , .w=160 };
    SDL_Texture *save_2_red = getImageTexture(sdlRenderer , "../img/saves/save-2-red.bmp") ;
    SDL_Texture *save_2_green = getImageTexture(sdlRenderer , "../img/saves/save-2-green.bmp") ;
    SDL_Rect save_2 = {.x=x+200 , .y=y , .h=40 , .w=160 };

    SDL_Texture *save_3_red = getImageTexture(sdlRenderer , "../img/saves/save-3-red.bmp") ;
    SDL_Texture *save_3_green = getImageTexture(sdlRenderer , "../img/saves/save-3-green.bmp") ;
    SDL_Rect save_3 = {.x=x , .y=y+60 , .h=40 , .w=160 };
    SDL_Texture *save_4_red = getImageTexture(sdlRenderer , "../img/saves/save-4-red.bmp") ;
    SDL_Texture *save_4_green = getImageTexture(sdlRenderer , "../img/saves/save-4-green.bmp") ;
    SDL_Rect save_4 = {.x=x+200 , .y=y+60 , .h=40 , .w=160 };

    SDL_Texture *save_5_red = getImageTexture(sdlRenderer , "../img/saves/save-5-red.bmp") ;
    SDL_Texture *save_5_green = getImageTexture(sdlRenderer , "../img/saves/save-5-green.bmp") ;
    SDL_Rect save_5 = {.x=x , .y=y+120 , .h=40 , .w=160 };
    SDL_Texture *save_6_red = getImageTexture(sdlRenderer , "../img/saves/save-6-red.bmp") ;
    SDL_Texture *save_6_green = getImageTexture(sdlRenderer , "../img/saves/save-6-green.bmp") ;
    SDL_Rect save_6 = {.x=x+200 , .y=y+120 , .h=40 , .w=160 };

    Sint16 vx[4] = {0 , 960 , 960 , 0 } ;
    Sint16 vy[4] = {0 , 0 , 640 , 640 } ;
    filledPolygonColor(sdlRenderer , vx , vy , 4 , 0x8800ff00) ;

    int cond = 1 ;
    int id_to_check = -1 ;
    while ( cond )
    {
        id_to_check = -1 ;

        SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff, 0xff ) ;
        SDL_RenderCopy(sdlRenderer , back , NULL , &back_rect) ;

        if ( AllScoresArray[0].games_played == -1 )
            SDL_RenderCopy(sdlRenderer , save_1_red , NULL , &save_1) ;
        else
            SDL_RenderCopy(sdlRenderer , save_1_green , NULL , &save_1) ;

        if ( AllScoresArray[1].games_played == -1 )
            SDL_RenderCopy(sdlRenderer , save_2_red , NULL , &save_2) ;
        else
            SDL_RenderCopy(sdlRenderer , save_2_green , NULL , &save_2) ;

        if ( AllScoresArray[2].games_played == -1 )
            SDL_RenderCopy(sdlRenderer , save_3_red , NULL , &save_3) ;
        else
            SDL_RenderCopy(sdlRenderer , save_3_green , NULL , &save_3) ;

        if ( AllScoresArray[3].games_played == -1 )
            SDL_RenderCopy(sdlRenderer , save_4_red , NULL , &save_4) ;
        else
            SDL_RenderCopy(sdlRenderer , save_4_green , NULL , &save_4) ;

        if ( AllScoresArray[4].games_played == -1 )
            SDL_RenderCopy(sdlRenderer , save_5_red , NULL , &save_5) ;
        else
            SDL_RenderCopy(sdlRenderer , save_5_green , NULL , &save_5) ;

        if ( AllScoresArray[5].games_played == -1 )
            SDL_RenderCopy(sdlRenderer , save_6_red , NULL , &save_6) ;
        else
            SDL_RenderCopy(sdlRenderer , save_6_green , NULL , &save_6) ;

        SDL_Event sdlEvent ;
        while (SDL_PollEvent(&sdlEvent))
        {
            Sint32 xcoord = sdlEvent.motion.x ;
            Sint32 ycoord = sdlEvent.motion.y ;
            switch ( sdlEvent.type )
            {
                case SDL_QUIT:
                    cond = 0 ;
//                    return -1 ;

                case SDL_MOUSEBUTTONDOWN:
                    if ( xcoord > x && xcoord < x+160 )
                    {
                        if ( ycoord>y && ycoord<y+40 )              // save 1
                            id_to_check = 1 ;
                        else if ( ycoord>y+60 && ycoord<y+100 )     // save 3
                            id_to_check = 3 ;
                        else if ( ycoord>y+120 && ycoord<y+160 )    // save 5
                            id_to_check = 5 ;
                    }
                    if ( xcoord > x+200 && xcoord < x+360 )
                    {
                        if ( ycoord>y && ycoord<y+40 )              // save 2
                            id_to_check = 2 ;
                        else if ( ycoord>y+60 && ycoord<y+100 )     // save 4
                            id_to_check = 4 ;
                        else if ( ycoord>y+120 && ycoord<y+160 )    // save 6
                            id_to_check = 6 ;
                    }

            }
        }

        if ( id_to_check != -1 )
        {
            if ( AllScoresArray[id_to_check-1].games_played != -1 )
                cond = 0 ;

            else
            {
                AllScoresArray[id_to_check-1].games_played = 0 ;
                FILE* scores = fopen("../dat/usr/scores.txt" , "w" ) ;
                for ( int i=0 ; i<6 ; i++)
                    fprintf(scores , "%d~%d~%d\n"
                            ,AllScoresArray[i].user_id , AllScoresArray[i].games_played , AllScoresArray[i].total_score ) ;
                fclose(scores) ;
                GetName(sdlWindow , sdlRenderer , id_to_check ) ;
                printf("successfully created account\n") ;
                cond = 0 ;
            }
        }

        SDL_RenderPresent(sdlRenderer) ;
    }

    SDL_DestroyTexture(save_1_green) ;
    SDL_DestroyTexture(save_1_red) ;
    SDL_DestroyTexture(save_2_green) ;
    SDL_DestroyTexture(save_2_red) ;
    SDL_DestroyTexture(save_3_green) ;
    SDL_DestroyTexture(save_3_red) ;
    SDL_DestroyTexture(save_4_green) ;
    SDL_DestroyTexture(save_4_red) ;
    SDL_DestroyTexture(save_5_green) ;
    SDL_DestroyTexture(save_5_red) ;
    SDL_DestroyTexture(save_6_green) ;
    SDL_DestroyTexture(save_6_red) ;

    SDL_DestroyTexture(back) ;

    SDL_free(&save_1) ;
    SDL_free(&save_2) ;
    SDL_free(&save_3) ;
    SDL_free(&save_4) ;
    SDL_free(&save_5) ;
    SDL_free(&save_6) ;

    SDL_free(&back_rect) ;

    return id_to_check ;
}


void GetName ( SDL_Window*  sdlWindow , SDL_Renderer* sdlRenderer , int id_to_check )
{

    FILE* usernames_txt = fopen("../dat/usr/usernames.txt" , "r") ;
    OneUser *AllUsersArray = malloc(sizeof(OneUser) * 6 ) ;
    for ( int i=0 ; i<6 ; i++)
    {
        int id ;
        char* name = (char*) malloc(sizeof(char) * 16) ;
        fscanf(usernames_txt , "%d~%s\n" , &id , name ) ;
        AllUsersArray[i].user_id = id ;
        AllUsersArray[i].user_name = name ;
    }
    fclose(usernames_txt) ;

    SDL_Texture *who = getImageTexture(sdlRenderer , "../img/saves/who-.bmp") ;
    SDL_Rect who_rect = {.x=220 , .y=140 , .w=520 , .h=120 };

    SDL_Rect shadow = {.x=0 , .y=0 , .w=960 , .h=640};
    SDL_Rect back_box = {.x=200 , .y=100 , .w=560 , .h=300};
    SDL_Rect box = {.x=400 , .y=300 , .w=160 , .h=60 };

    SDL_Texture *back = getImageTexture(sdlRenderer , "../img/saves/hex-back.bmp") ;
    SDL_Rect back_rect = {.x=0 , .y=0 , .w=960 , .h=1200};

    char* name = (char*) calloc(16 , sizeof(char)) ;
    int num_letters = 0 ;
    int cond = 1 ;
    while ( cond )
    {
        SDL_RenderCopy(sdlRenderer , back , NULL , &back_rect ) ;

        SDL_SetRenderDrawColor(sdlRenderer , 0x00 , 0x00 , 0x00 , 0x33 ) ;
        SDL_RenderFillRect(sdlRenderer , &shadow ) ;

        SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff) ;
        SDL_RenderFillRect(sdlRenderer , &back_box) ;

        SDL_RenderCopy(sdlRenderer , who , NULL , &who_rect) ;

        SDL_SetRenderDrawColor(sdlRenderer , 0x80 , 0x80 , 0x80 , 0xbb ) ;
        SDL_RenderFillRect(sdlRenderer , &box ) ;



        SDL_Event sdlEvent ;
        while (SDL_PollEvent(&sdlEvent))
        {
            switch ( sdlEvent.type ) {
                case SDL_KEYDOWN:
                    if ( sdlEvent.key.keysym.sym == SDLK_RETURN && num_letters != 0 )
                    {
                        name[num_letters] = '\0' ;
                        cond = 0 ;
                    }
                    else if ( sdlEvent.key.keysym.sym == SDLK_SPACE )
                    {
                        name[num_letters] = '-' ;
                        num_letters++ ;
                    }
                    else if ( (sdlEvent.key.keysym.sym == SDLK_BACKSPACE || sdlEvent.key.keysym.sym == SDLK_DELETE)
                                && num_letters > 0 )
                    {
                        num_letters-- ;
                        name[num_letters] = '\0' ;
                    }
                    else
                    {
                        name[num_letters] = toupper(sdlEvent.key.keysym.sym) ;
                        num_letters += 1 ;
                    }
                    SDL_Delay(100) ;
            }
        }
        stringColor(sdlRenderer , 480-4*num_letters , 330 , name , 0xff000000) ;
        SDL_RenderPresent(sdlRenderer) ;
    }

    AllUsersArray[id_to_check-1].user_name = name ;

    FILE* usernames = fopen("../dat/usr/usernames.txt" , "w") ;
    for ( int i=0 ; i<6 ; i++)
        fprintf(usernames , "%d~%s\n" , AllUsersArray[i].user_id , AllUsersArray[i].user_name ) ;
    fclose(usernames_txt) ;


    SDL_DestroyTexture(back) ;
    SDL_DestroyTexture(who) ;

    SDL_free(&shadow) ;
    SDL_free(&box) ;
    SDL_free(&back_rect) ;
    SDL_free(&who_rect) ;
}


void UpdateLeaderboard ( int* GLOBAL_POINTS_ARRAY , int NUM_PLAYERS , int user_id )
{
    FILE* scores = fopen("../dat/usr/scores.txt" , "r") ;
    OneScore *AllUsersScores = (OneScore*) malloc(sizeof(OneScore) * 6 ) ;
    for ( int i=0 ; i<6 ; i++)
    {
        int id , gp , ts ;
        fscanf(scores , "%d~%d~%d\n" , &id , &gp , &ts ) ;
        AllUsersScores[i].user_id = id ;
        AllUsersScores[i].games_played = gp ;
        AllUsersScores[i].total_score = ts ;
    }

    fclose(scores) ;

    AllUsersScores[user_id-1].games_played += 1 ;
    AllUsersScores[user_id-1].total_score += GLOBAL_POINTS_ARRAY[1] ;

    FILE* scores_txt = fopen("../dat/usr/scores.txt" , "w+") ;
    for ( int i=0 ; i<6 ; i++)
        fprintf(scores , "%d~%d~%d\n" , AllUsersScores[i].user_id , AllUsersScores[i].games_played , AllUsersScores[i].total_score ) ;
    fclose(scores) ;


    FILE* bots_scores_txt = fopen("../dat/usr/bots_scores.txt" , "r") ;
    OneScore * AllBotsArray = malloc(4 * sizeof(OneScore) ) ;
    for ( int i=0 ; i<4 ; i++)
    {
        int b_id , b_gp , b_ts ;
        fscanf(bots_scores_txt , "%d~%d~%d\n" , &b_id , &b_gp , &b_ts ) ;
        AllBotsArray[i].user_id = b_id ;
        AllBotsArray[i].games_played = b_gp ;
        AllBotsArray[i].total_score = b_ts ;
    }
    fclose(bots_scores_txt) ;

    for ( int i=2 ; i<NUM_PLAYERS ; i++ )
    {
        AllBotsArray[i-2].games_played += 1 ;
        AllBotsArray[i-2].total_score = GLOBAL_POINTS_ARRAY[i] ;
    }

    FILE* bots_scores = fopen("../dat/usr/bots_scores.txt" , "w") ;
    for ( int i=0 ; i<4 ; i++)
        fprintf(bots_scores , "%d~%d~%d\n" , AllBotsArray[i].user_id , AllBotsArray[i].games_played , AllBotsArray[i].total_score ) ;
    fclose(bots_scores) ;
}


void swap_cells ( OneStanding* first , OneStanding* second )
{
    char* name_ = first->name ;
    char* type_ = first->type ;
    int gp = first->games_played ;
    int ts  = first->total_score ;

    first->name = second->name ;
    first->type = second->type ;
    first->games_played = second->games_played ;
    first->total_score = second->total_score ;

    second->name = name_ ;
    second->type = type_ ;
    second->games_played = gp ;
    second->total_score = ts ;
}

void ShowLeaderboard ( SDL_Window* sdlWindow , SDL_Renderer* sdlRenderer )
{
    FILE* user_data = fopen("../dat/usr/scores.txt" , "r") ;
    int number_of_users = 0 ;
    for ( int i=0 ; i<6 ; i++ )
    {
        int id , gp , ts ;
        fscanf(user_data , "%d~%d~%d\n" , &id , &gp , &ts ) ;
        if ( gp != -1 )
            number_of_users += 1 ;
    }
    fclose(user_data) ;
    OneStanding *AllStanding = calloc( number_of_users+4 , sizeof(OneStanding)) ;

    FILE* users_scores = fopen("../dat/usr/scores.txt" , "r") ;
    FILE* usernames = fopen("../dat/usr/usernames.txt" , "r") ;

    int counter = 0 ;
    for ( int i=0 ; i<6 ; i++)
    {
        int id , gp , ts ;
        char* name = malloc(sizeof(char) * 16 ) ;
        fscanf(users_scores , "%d~%d~%d\n" , &id , &gp , &ts ) ;
        fscanf(usernames , "%d~%s\n" , &id , name ) ;
        if ( gp != -1 )
        {
            AllStanding[counter].name = name ;
            AllStanding[counter].games_played = gp ;
            AllStanding[counter].total_score = ts ;
            AllStanding[counter].type = "USER" ;
            counter ++ ;
        }
    }
    fclose(users_scores) ;
    fclose(usernames) ;


    FILE* bots_scores = fopen("../dat/usr/bots_scores.txt" , "r") ;
    FILE* bots_usernames = fopen("../dat/usr/bots_usernames.txt" , "r") ;
    for ( int i=0 ; i<4 ; i++)
    {
        int id , gp , ts ;
        char* name = malloc(sizeof(char) * 16 ) ;
        fscanf(bots_scores , "%d~%d~%d\n" , &id , &gp , &ts ) ;
        fscanf(bots_usernames , "%d~%s\n" , &id , name ) ;
        AllStanding[counter].name = name ;
        AllStanding[counter].games_played = gp ;
        AllStanding[counter].total_score = ts ;
        AllStanding[counter].type = "SYSTEM" ;
        counter ++ ;
    }
    fclose(bots_usernames) ;
    fclose(bots_scores) ;

    for (int i = 0; i < counter-1; i++)
    {
        // Last i elements are already in place
        for (int j = 0; j < counter-i-1; j++)
        {
            if (    AllStanding[j].games_played != 0 && AllStanding[j+1].games_played != 0 &&
                    AllStanding[j].total_score/AllStanding[j].games_played < AllStanding[j+1].total_score/AllStanding[j+1].games_played )
                swap_cells(&AllStanding[j] , &AllStanding[j+1]);
        }
    }

    SDL_Texture *curve_img = getImageTexture(sdlRenderer , "../img/curve.bmp") ;
    SDL_Rect curve_rect = {.x=60 , .y=-70 , .w=840 , .h=800 };

    int cond = 1 ;
    while ( cond )
    {
        int x = 200 ;
        int y = 150 ;
        SDL_Event sdlEvent ;
        while ( SDL_PollEvent(&sdlEvent) )
        {
            switch (sdlEvent.type){
                case SDL_QUIT:
                    cond = 0 ;
                    break;
                default:
                    break;
            }
        }

        SDL_RenderCopy(sdlRenderer , curve_img , NULL , &curve_rect) ;

        char* title = malloc(sizeof(char) * 120 ) ;
        sprintf(title , "%16s %16s  %12s  %12s" , "USERNAME" , "TYPE" , "GAMES PLAYED" , "TOTAL SCORE") ;
        stringColor(sdlRenderer , x , y-60 ,title , 0xff000000) ;

        aalineColor(sdlRenderer , x-20 , y-30 , x+570 , y-30 , 0xff000000) ;

        for ( int i=0 ; i<counter ; i++ ) {
            char* output = malloc(sizeof(char) * 120 ) ;
            sprintf(output , "%16s %16s  %12d  %12d", AllStanding[i].name, AllStanding[i].type,
                    AllStanding[i].games_played, AllStanding[i].total_score) ;
            stringColor(sdlRenderer, x, y, output , 0xff000000);
            y += 40;
        }

        SDL_RenderPresent(sdlRenderer) ;
        SDL_Delay(1000/60) ;
    }

    SDL_DestroyTexture(curve_img) ;
    SDL_free(&curve_rect) ;
}
