////
//// Created by Mahan on 2/1/2022.
////
//
//
//const int WIDTH = 800 ;
//const int HEIGHT = 600 ;
//const int FPS = 60 ;
//const int PRO_RATE = 1 ;
//const int INIT_SOLDIER = 25 ;
//const int NUMBER_OF_PLAYERS = 3 ;
//const int LAND_WIDTH = 30 ;
//const int LAND_HEIGHT = 40 ;
//
//Uint32 COLORS[4] = { 0xffa39d8c , 0xff3434eb , 0xff6ebe34 , 0xffb00500 };
//// grey          // blue          // green        // red
//
//typedef  struct properties {
//    int state_condition  ;          // -1 --> null // 0 --> neutral // 1 --> in_use //
//    int owner ;                     // a number between 0 to (num_of_players) // 0 for neutral // other nums for players //
//    int soldiers ;                  // always between 0 and 120 //
//    int soldier_production_rate ;   // 0 for non-military and PRO_RATE for others //
//} properties ;
//
//void create_map_array ( const char* map_name , properties arr[10][20])
//{
//    FILE *map_txt = fopen("../logs/template1.txt" , "r") ;
//    char c = ' ' ;
//    for ( int i=0 ; c != EOF ; i++)
//    {
//        for ( int j=0 ; j<20 ; j++)
//        {
//            c = fgetc(map_txt) ;
//            if ( c == '0' )   // BLACK IN MAP
//            {
//                arr[i][j].state_condition = -1 ;
//                arr[i][j].owner = -1 ;
//                arr[i][j].soldiers = 0 ;
//                arr[i][j].soldier_production_rate = 0 ;
//            }
//            else if ( c=='L' )  // FREE LAND WITH MILITARY BASE
//            {
//                arr[i][j].state_condition = 0 ;
//                arr[i][j].owner = 0 ;
//                arr[i][j].soldiers = INIT_SOLDIER ;
//                arr[i][j].soldier_production_rate = PRO_RATE ;
//            }
//            else if ( c =='M' ) // FREE LAND WITH NO MILITARY BASE
//            {
//                arr[i][j].state_condition = 0 ;
//                arr[i][j].owner = 0 ;
//                arr[i][j].soldiers = INIT_SOLDIER ;
//                arr[i][j].soldier_production_rate = 0 ;
//            }
//            c = fgetc(map_txt) ;
//        }
//    }
//}
//
//void RANDOMIZE_MAP ( properties arr[10][20] , int players_num )
//{
//    // 0 --> NEUTRAL LAND WITH NO MILITARY BASE
//    // 1 TO players_num --> PLAYERS' LAND WITH MILITARY BASE AT THE BEGINNING OF THE GAME
//    // ALL PLAYERS WILL HAVE AT LEAST 1 AND AT MAX 3 LANDS AT THE BEGINNING OF THE GAME
//    srand(time(0)) ;
//    int land_num ;
//    int rand_x ;
//    int rand_y ;
//    for ( int i=1 ; i<=players_num ; i++ )
//    {
//        land_num = rand() % 2 + 1 ;
//        for ( land_num ; land_num>0 ; land_num-- )
//        {
//            rand_x = rand() % 20 ;
//            rand_y = rand() % 10 ;
//            while ( arr[rand_y][rand_x].owner!=0 || arr[rand_y][rand_x].soldier_production_rate==0 )
//            {
//                rand_x = rand() % 20 ;
//                rand_y = rand() % 10 ;
//            }
//            arr[rand_y][rand_x].owner = i ;
//            arr[rand_y][rand_x].state_condition = 0 ;
//        }
//    }
//}
//
//void DRAW_LAND ( SDL_Renderer *renderer , properties land , int length , int height , Sint16 x , Sint16 y )
//{
//    if ( land.state_condition != -1 )
//    {
//        SDL_Rect border = { .x=x-length/2 , .y=y-height/2 , .w = length , .h=height } ;
//        boxColor(renderer, x-length/2, y-height/2, x+length/2, y+height/2, COLORS[land.owner]);
//        if ( land.soldier_production_rate != 0 ) {
//            filledCircleColor(renderer, (x), (y), 2, 0xff000000);
//            char* buffer = malloc(sizeof (char) * 20) ;
//            sprintf(buffer , "%d" , land.soldiers) ;
//            stringRGBA(renderer , x-5 , y+5 , buffer , 0 , 0 , 0 , 255) ;
//        }
//        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
//        SDL_RenderDrawRect(renderer, &border);
//    }
//}
//
//void GAME_BACKGROUND (SDL_Renderer *renderer , properties arr[10][20] , int length , int height )
//{
//    int x = length/2 ;
//    int y = height/2 ;
//    for ( int i=0 ; i<10 ; i++)
//    {
//        x = length/2 ;
//        for ( int j=0 ; j<20 ; j++)
//        {
//            DRAW_LAND(renderer , arr[i][j] , length , height , x , y ) ;
//            x += length ;
//        }
//        y += height ;
//    }
//}
//
//void ADD_SOLDIERS ( properties map_arr[10][20] )
//{
//    for ( int i=0 ; i<10 ; i++)
//    {
//        for ( int j=0 ; j<20 ; j++)
//        {
//            properties* land = &map_arr[i][j] ;
//            if ( land->soldiers < 120 && land->soldier_production_rate!=0) {
//                land->soldiers += land->soldier_production_rate;
//                if (land->soldiers > 120)
//                    land->soldiers = 120;
//            }
//        }
//    }
//}
//
//int main()
//{
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
//        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//        return 0;
//    }
//
//    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
//        const char* driver_name = SDL_GetAudioDriver(i);
//        printf("audio driver name : %s\n" , driver_name) ;
//        if (SDL_AudioInit(driver_name)) {
//            printf("Audio driver failed to initialize: %s\n", driver_name);
//            continue;
//        }
//    }
//
//    test_func() ;
//    map_handling_test_func() ;
//
//    SDL_Window *sdlWindow = SDL_CreateWindow("State.io" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;
//    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;
//
//    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff ) ;
//    SDL_RenderClear(sdlRenderer) ;
//
//
//    land* map_arr = malloc(sizeof(land) * 7*12 ) ;
//    map_arr = GENERATE_HEXAGON_RANDOM_MAP(sdlWindow , sdlRenderer , 3 , WIDTH , HEIGHT , map_arr , 35 ) ;
//    int NUM_OF_CELLS = ShowHexagonBackground(sdlWindow , sdlRenderer , map_arr , 84 , 35) ;
//    printf("num of cells is : %d\n" , NUM_OF_CELLS) ;
//
//    SDL_RenderPresent(sdlRenderer) ;
//    SDL_Delay(15000) ;
//
//    properties map_array[10][20] ;
//    create_map_array("hello" , map_array) ;
//    RANDOMIZE_MAP(map_array , NUMBER_OF_PLAYERS) ;
//
//    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
//    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};
//    if ( ShowMenu(sdlWindow , sdlRenderer , WIDTH , HEIGHT , img) == 0 )
//        return 0 ;
//
//    int i=0 ;
//
//    SDL_bool shallExit = SDL_FALSE ;
//    int little_circle = 0 ;
//    while ( !shallExit )
//    {
//        i %= 200000 ;
//
//        SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff ,0xff) ;
//        SDL_RenderClear(sdlRenderer) ;
//        SDL_RenderCopy(sdlRenderer, img , NULL, &texture_rect);
//        GAME_BACKGROUND(sdlRenderer , map_array , LAND_WIDTH , LAND_HEIGHT) ;
//
//
//        if ( i%80 == 0 )
//            ADD_SOLDIERS(map_array) ;
//
//        SDL_Event sdlEvent;
//        while (SDL_PollEvent(&sdlEvent)) {
//            int x = sdlEvent.motion.x ;
//            int y = sdlEvent.motion.y ;
//            switch (sdlEvent.type)
//            {
//                case SDL_QUIT:
//                    printf("program Exited because user wanted !\n") ;
//                    shallExit = SDL_TRUE;
//                    break;
//                case SDL_MOUSEMOTION:
//                    if ( little_circle == 1)
//                        filledCircleColor(sdlRenderer , x , y , 5 , 0xff4f5500) ;
//                    break;
//                case SDL_MOUSEBUTTONDOWN:
//                    little_circle = 1 ;
//                    printf("mouse is being pushed at (%d , %d)\n" , x , y) ;
//                    printf("land (%d , %d)\n " , x/LAND_WIDTH , y/LAND_HEIGHT );
//                    printf("owner : %d\n" , map_array[y/LAND_HEIGHT][x/LAND_WIDTH].owner) ;
//                    if (  map_array[y/LAND_HEIGHT][x/LAND_WIDTH].owner == 1 )
//                        printf("it is your land \n") ;
//                    else
//                        printf("it is not your land\n") ;
//                    break;
//                case SDL_MOUSEBUTTONUP:
//                    printf("destination is (%d,%d}\n" , x/LAND_WIDTH , y/LAND_HEIGHT) ;
//                    little_circle = 0 ;
//                    break;
//            }
//        }
//        SDL_RenderPresent(sdlRenderer) ;
//        SDL_Delay(1000/FPS) ;
//        i++;
//    }
//
//    // Quitting window and program //
//    SDL_DestroyWindow(sdlWindow) ;
//    SDL_Quit() ;
//    printf("all done") ;
//    return  0 ;
//}


//if ( clicked_cell_info.owner_id == 1 )
//{
//HomelandClicked += 1 ;
//}
//
//if ( HomelandClicked%2 == 1 && clicked_cell_info.owner_id == 1 )
//{
//Origin_x = x ;
//Origin_y = y ;
//}
//
//else if ( HomelandClicked%2 == 0 && clicked_cell_info.owner_id == 1 && Origin_x != clicked_cell_info.x && Origin_y != clicked_cell_info.y)
//{
//Destination_x = x ;
//Destination_y = y ;
//}
//
//else if ( HomelandClicked%2 == 0 && clicked_cell_info.owner_id == 1 )
//{
//Destination_x = 0 ;
//Destination_y = 0 ;
//}
//
//else if ( HomelandClicked%2 == 0 && clicked_cell_info.owner_id != -1 )
//{
//Destination_x = x ;
//Destination_y = y ;
//}
//
//if ( HomelandClicked%2 == 0 && Destination_x!=0 && Destination_y!=0 )
//printf("attack ! \n") ;





//                            AllSoldiersArray[0] = malloc(sizeof(OneSoldier) *
//                                    map_arr[Origin_counter].soldiers_number) ;
//
//                            AllSoldiersArray[0][0].destination_x = clicked_cell_info.x ;
//                            AllSoldiersArray[0][0].destination_y = clicked_cell_info.y ;
//                            AllSoldiersArray[0][0].destination_counter = clicked_cell_info.counter ;
//                            AllSoldiersArray[0][0].owner_id = map_arr[Origin_counter].owner_id ;
//                            AllSoldiersArray[0][0].power = 1 ;
//
//                            // delta management //
//                            float delta_y = clicked_cell_info.y - map_arr[Origin_counter].y ;
//                            float delta_x = clicked_cell_info.x - map_arr[Origin_counter].x ;
//                            double theta = delta_y / delta_x ;
//                            AllSoldiersArray[0][0].verticalSpeed = 8.0 * sin(atan(theta) ) ;
//                            if ( delta_y > 0 )
//                                AllSoldiersArray[0][0].verticalSpeed = fabs(AllSoldiersArray[0][0].verticalSpeed) ;
//                            else if ( delta_y <0 )
//                                AllSoldiersArray[0][0].verticalSpeed = -1 * fabs(AllSoldiersArray[0][0].verticalSpeed) ;
//                            AllSoldiersArray[0][0].horizontalSpeed = 8.0 * cos(atan(theta) ) ;
//                            if ( delta_x > 0 )
//                                AllSoldiersArray[0][0].horizontalSpeed = fabs(AllSoldiersArray[0][0].horizontalSpeed) ;
//                            else if ( delta_x <0 )
//                                AllSoldiersArray[0][0].horizontalSpeed = -1 * fabs(AllSoldiersArray[0][0].horizontalSpeed) ;
//
//
//                            AllSoldiersArray[0][0].x = map_arr[Origin_counter].x ;
//                            AllSoldiersArray[0][0].y = map_arr[Origin_counter].y ;
//
//                            int c = map_arr[Origin_counter].soldiers_number ;
//                            printf("map_arr[Origin_counter].soldiers_number : %d\n" , map_arr[Origin_counter].soldiers_number) ;
//                            for ( int temp_c = 1 ; temp_c<25 ; temp_c++ )
//                            {
//                                printf("vx : %f | vy : %f\n" , AllSoldiersArray[0][0].horizontalSpeed
//                                        , AllSoldiersArray[0][0].verticalSpeed) ;
//                                AllSoldiersArray[0][temp_c].x = AllSoldiersArray[0][temp_c-1].x - 1.5*AllSoldiersArray[0][0].horizontalSpeed ;
//                                AllSoldiersArray[0][temp_c].y = AllSoldiersArray[0][temp_c-1].y - 1.5*AllSoldiersArray[0][0].verticalSpeed ;
//                                AllSoldiersArray[0][temp_c].destination_x = AllSoldiersArray[0][0].destination_x ;
//                                AllSoldiersArray[0][temp_c].destination_y = AllSoldiersArray[0][0].destination_y ;
//                                AllSoldiersArray[0][temp_c].destination_counter = AllSoldiersArray[0][0].destination_counter ;
//                                AllSoldiersArray[0][temp_c].owner_id = AllSoldiersArray[0][0].owner_id ;
//                                AllSoldiersArray[0][temp_c].power = 1 ;
//                            }



//        for ( int temp_c=0 ; temp_c<25 ; temp_c++ ) {
//            if (AllSoldiersArray[0] != NULL && AllSoldiersArray[0][temp_c].x >= -999) {
//                AllSoldiersArray[0][temp_c].x += AllSoldiersArray[0][0].horizontalSpeed;
//                AllSoldiersArray[0][temp_c].y += AllSoldiersArray[0][0].verticalSpeed;
//                int r = AllSoldiersArray[0][temp_c].power * 2 ;
//
//                filledCircleColor(sdlRenderer, AllSoldiersArray[0][temp_c].x, AllSoldiersArray[0][temp_c].y, 2 * r, 0xffff00ff);
//
//                printf("[%d] --> x : %lf | des_x : %lf\n", temp_c ,
//                       AllSoldiersArray[0][temp_c].x, AllSoldiersArray[0][temp_c].destination_x);
//
//                if (fabs(AllSoldiersArray[0][temp_c].x-AllSoldiersArray[0][temp_c].destination_x)/HEXAGON_A < 0.5 &&
//                    fabs(AllSoldiersArray[0][temp_c].y-AllSoldiersArray[0][temp_c].destination_y)/HEXAGON_A < 0.5)
//                {
//                    AllSoldiersArray[0][temp_c].x = -1001;
//                    if (map_arr[AllSoldiersArray[0][temp_c].destination_counter].owner_id != AllSoldiersArray[0][temp_c].owner_id)
//                        map_arr[AllSoldiersArray[0][temp_c].destination_counter].soldiers_number -= AllSoldiersArray[0][temp_c].power;
//                    else
//                        map_arr[AllSoldiersArray[0][temp_c].destination_counter].soldiers_number += AllSoldiersArray[0][temp_c].power;
//                }
//            }
//        }




//        if ( i%200 == 33 && Potion_condition==0 ) {
//            live_time_potion = CreatePotion(WIDTH, HEIGHT);
//            printf("potion_x : %d | potion_y : %d\npotion_id : %d\n----------------\n", live_time_potion.x,
//                   live_time_potion.y, live_time_potion.potion_id);
//            if ( live_time_potion.x != -1 ) {
//                Potion_condition = 1 ;  // Potion is created but not used yet
//                Potion_time_to_exist = 0 ;
//            }
//        }

//        if ( Potion_condition==1 )
//        {
//            Potion_time_to_exist += 1 ;
//            if ( Potion_time_to_exist % 300 == 0 )
//                Potion_condition = 0 ;
//            filledCircleColor(sdlRenderer , live_time_potion.x , live_time_potion.y , 10 , 0xff000000) ;
//            if ( CheckForSoldierPotionConflict(AllSoldiersArray , live_time_potion , SPEED_ARRAY ) != -1 )
//            {
//                Potion_condition = 2 ;
//                Potion_time_to_exist = 0 ;
//            }
//        }
//
//        if ( Potion_condition==2 )
//        {
//            Potion_time_to_exist += 1 ;
//            if ( Potion_time_to_exist%400 == 0 ) {
//                SPEED_ARRAY[1] = 10 ;
//                Potion_condition = 0;
//            }
//        }


//////////////////////////////////////////////////////////


//int StartNewGame ( const int NUMBER_OF_PLAYERS , const int WIDTH , const int HEIGHT , const int FPS ,
//                   const int HEXAGON_A , const int NUM_OF_ROWS , const int NUM_OF_COLS ) {
//
////    const int WIDTH = 800 ;
////    const int HEIGHT = 600 ;
////    const int FPS = 60 ;
////    const int HEXAGON_A = 26 ;
////    const int NUM_OF_ROWS = 20 ;
////    const int NUM_OF_COLS = 18 ;
////
////    const int NUMBER_OF_PLAYERS = 6 ;
//
//    float SPEED_ARRAY [6] =             { 1 , 1 , 1 , 1 , 1 , 1};
//    int PRODUCTION_RATE_ARRAY [6] =     { 0 , 2 , 2 , 2 , 2 , 2};
//    float SOLDIERS_POWER_ARRAY [6] =    { 1 , 1 , 1 , 1 , 1 , 1};
//    int IMMUNE_LANDS_ARRAY [6] =        { 0 , 0 , 0 , 0 , 0 , 0};
//    int GLOBAL_POINTS_ARRAY[6] =        { 0 , 0 , 0 , 0 , 0 , 0 };
//    SDL_Texture* POTION_GRAPHIC[8] ;
//
//    // game main sdlWindow
//    SDL_Window *sdlWindow = SDL_CreateWindow("State.io" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;
//
//    // game main sdlRenderer
//    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;
//
//    SDL_Texture *castle_texture = getImageTexture(sdlRenderer , "../img/castle.bmp") ;
//
//    // creating white screen for entrance
//    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff ) ;
//    SDL_RenderClear(sdlRenderer) ;
//
//    // Generating map_arr
//    land* map_arr = malloc(sizeof(land) * (NUM_OF_ROWS * NUM_OF_COLS) ) ;
//    if ( map_arr == NULL ) {
//        fprintf(stderr, "map allocation error");
//        return 0;
//    }
//
////    int NUM_OF_CELLS ;
////    ImportMap(map_arr , "../dat/map/map_sample.dat" , &NUM_OF_CELLS) ;
//    map_arr = GENERATE_HEXAGON_RANDOM_MAP(NUMBER_OF_PLAYERS , NUM_OF_COLS , NUM_OF_ROWS , map_arr , HEXAGON_A ) ;
//    int NUM_OF_CELLS = ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_ROWS*NUM_OF_COLS , HEXAGON_A , castle_texture) ;
//
////    char* name = "../dat/map/map_sample.dat" ;
////    ExportMap(map_arr , name , NUM_OF_CELLS) ;
//
//
//    printf("num of cells is : %d\n" , NUM_OF_CELLS) ;
//    // an array to save number of cells owned by each owner_id
//    int* CELLS_OWNED = calloc( NUMBER_OF_PLAYERS , sizeof(int)) ;
//
//    // an array to save counter of each owner_id lands
//    int** LANDS_OWNED_COUNTERS = malloc(  NUMBER_OF_PLAYERS * sizeof(int*) ) ;
//    for ( int i=0 ; i<NUMBER_OF_PLAYERS ; i++)
//        LANDS_OWNED_COUNTERS[i] = calloc( NUM_OF_CELLS , sizeof(int)) ;
//
//    // updating map info
//    UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS ) ;
//
//    // getting background picture as texture
//    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
//    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};
//
//    // getting go_back_to_menu picture as texture
//    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
//    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};
//
//    // Initializing potion textures
//    InitializePotionGraphics(sdlRenderer , POTION_GRAPHIC) ;
//    SDL_Rect potion_rect = {.x=0, .y=0, .w=30 , .h=30};
//
//    /////////////////variable definitions/////////////////
//
//    // game primary counter
//    int i=1 ;
//    SDL_bool shallExit = SDL_FALSE ;
//    SDL_bool shallShowMenu = SDL_TRUE ;
//    // variable to check mouse status
//    int click_status = 0 ;
//    // bunch of needed variables for user interaction
//    Sint16 Origin_x = 0 ;
//    Sint16 Origin_y = 0 ;
//    int Origin_counter = 0 ;
//    Sint16 Destination_x = 0 ;
//    Sint16 Destination_y = 0 ;
//    // variable containing data of potion that is on screen
//    Potion live_time_potion = {.potion_id=-1} ;
//
//    //////////////////////////////////////////////////////
//    // a very important array to save moving soldiers data
//    OneSoldier** AllSoldiersArray = calloc(2*NUM_OF_CELLS , sizeof(OneSoldier*) ) ;
//
//    // a very important array to save activated Potions data
//    OnePotionEffect* AllPotionsArray = calloc(NUMBER_OF_PLAYERS , sizeof(OnePotionEffect)) ;
//    for ( int temp=0 ; temp<NUMBER_OF_PLAYERS ; temp++)
//        AllPotionsArray[temp].potion_id = -1 ;
//
//    //////////////// game /////////////////
//    while ( !shallExit )
//    {
//        UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS) ;
//        i %= 200000 ;
//        while ( shallShowMenu )
//        {
//            int condition = ShowMenu(sdlWindow , sdlRenderer , WIDTH , HEIGHT , img) ;
//            if ( condition ==1  )
//                shallShowMenu = SDL_FALSE ;
//            else if ( condition == 0 )
//            {
//                printf("User quited the game successfully \n") ;
//                shallExit = SDL_TRUE ;
//                break;
//            }
//        }
//
//        DrawBackground(sdlRenderer , img , HEIGHT , WIDTH) ;
//        ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_CELLS , HEXAGON_A , castle_texture) ;
//
//        if ( i%50 == 20 ) {
//            for ( int j=2 ; j<NUMBER_OF_PLAYERS ; j++)
//                SystemMakeMovement(j, AllSoldiersArray, map_arr, CELLS_OWNED
//                        , LANDS_OWNED_COUNTERS, 5
//                        , SOLDIERS_POWER_ARRAY , IMMUNE_LANDS_ARRAY );
//        }
//
//        if ( i%80 == 0 )
//            AddSoldiers(map_arr , NUM_OF_CELLS , PRODUCTION_RATE_ARRAY ) ;
//
//        if ( i%550 == 50 )
//            live_time_potion = CreatePotion(WIDTH , HEIGHT) ;
//
//
//        CheckForSoldierPotionConflict(AllSoldiersArray, &live_time_potion, AllPotionsArray);
//        UpdatePotionEffectArray(AllPotionsArray , NUMBER_OF_PLAYERS) ;
//        ApplyPotionEffect(AllPotionsArray , SPEED_ARRAY , SOLDIERS_POWER_ARRAY , PRODUCTION_RATE_ARRAY , IMMUNE_LANDS_ARRAY , NUMBER_OF_PLAYERS) ;
//        RenderPotion(sdlRenderer , live_time_potion , POTION_GRAPHIC ) ;
//
//        SDL_Event sdlEvent;
//        land clicked_cell_info ;
//
//        if ( click_status == 1 )
//            AimAssist(sdlRenderer , sdlEvent , map_arr , NUM_OF_CELLS , HEXAGON_A , Origin_x , Origin_y ) ;
//
//
//        while (SDL_PollEvent(&sdlEvent)) {
//            Sint32 x = sdlEvent.motion.x ;
//            Sint32 y = sdlEvent.motion.y ;
//            switch (sdlEvent.type)
//            {
//                case SDL_QUIT:
//                    printf("User quited the game successfully !\n") ;
//                    shallExit = SDL_TRUE;
//                    break;
//                case SDL_MOUSEMOTION:
//                    break;
//                case SDL_MOUSEBUTTONDOWN:
//                    if ( x<40 && y<40 )
//                        shallShowMenu = SDL_TRUE ;
//
//                    clicked_cell_info = GiveClickedCellInfo(x , y , map_arr , NUM_OF_CELLS , HEXAGON_A) ;
//                    if ( clicked_cell_info.owner_id == 1 && click_status == 0 )
//                    {
//                        click_status += 1 ;
//                        click_status %= 2 ;
//                        Origin_x = clicked_cell_info.x ;
//                        Origin_y = clicked_cell_info.y ;
//                        Origin_counter = clicked_cell_info.counter ;
//                    }
//                    else if ( clicked_cell_info.owner_id != -1 && click_status == 1 )
//                    {
//                        click_status += 1 ;
//                        click_status %= 2 ;
//                        Destination_x = clicked_cell_info.x ;
//                        Destination_y = clicked_cell_info.y ;
//                        // should send soldiers from origin to destination //
//                        if ( Origin_counter != clicked_cell_info.counter && map_arr[Origin_counter].soldiers_number != 0
//                             && IMMUNE_LANDS_ARRAY[clicked_cell_info.owner_id] != 1 && map_arr[Origin_counter].owner_id == 1 )
//                            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , clicked_cell_info , SOLDIERS_POWER_ARRAY) ;
//                    }
//                    SDL_Delay(150) ;
//                    break;
//                case SDL_MOUSEBUTTONUP:
//                    break;
//            }
//        }
//
//
//        SoldierConflictSolver(AllSoldiersArray ) ;
//        ShowLinesOfSoldiers(sdlRenderer , AllSoldiersArray , HEXAGON_A , map_arr , SPEED_ARRAY) ;
//
//        UpdateScore(map_arr , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY) ;
//        if ( CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS) != -1 )
//        {
//            printf("player %d has won \n" , CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)) ;
//            GLOBAL_POINTS_ARRAY[CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)] += 400 ;
//            for ( int i=1 ; i<NUMBER_OF_PLAYERS ; i++)
//                printf("user %d with score of %d\n" , i , GLOBAL_POINTS_ARRAY[i]) ;
//            return 0 ;
//        }
//        DisplayScores(sdlRenderer , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY ) ;
//        DisplayPotionsEffect(sdlRenderer , AllPotionsArray , POTION_GRAPHIC , NUMBER_OF_PLAYERS) ;
//        SDL_RenderPresent(sdlRenderer) ;
//        SDL_Delay(1000/FPS) ;
//        i++;
//    }
//
//
//
//    // Freeing some memory for god's sake
//    SDL_free(img) ;
//    SDL_free(back_to_menu) ;
//    free(AllSoldiersArray) ;
//    free(map_arr) ;
//
//    // Destroying window and program //
//    SDL_DestroyTexture(img) ;
//    SDL_DestroyRenderer(sdlRenderer) ;
//    SDL_DestroyWindow(sdlWindow) ;
//
//}


/////////////////////////////////////////////////////////


//void ExportMap ( land* map_arr , char* Map_name , int NUM_CELLS )
//{
//
//    FILE * map_txt = fopen(Map_name , "w+") ;
//    fprintf(map_txt , "%d/%d\n" , 4 , NUM_CELLS ) ;
//
//    for ( int i=0 ; i<NUM_CELLS ; i++ )
//        fprintf(map_txt , "%d~%d~%d~%d~%d~%d~%f\n" ,map_arr[i].counter ,map_arr[i].owner_id
//                ,map_arr[i].x ,map_arr[i].y
//                ,map_arr[i].IS_MILITARY ,map_arr[i].RELATED_MILITARY_COUNTER
//                ,map_arr[i].soldiers_number ) ;
//
//    fclose(map_txt) ;
//}
//
//void ImportMap ( land* map_arr , char* Map_name , int* NUM_CELLS )
//{
//    FILE *map_txt = fopen(Map_name , "r") ;
//    fseek(map_txt , 0 , SEEK_SET ) ;
//    int temp ;
//    fscanf(map_txt , "%d/%d\n" , &temp , NUM_CELLS ) ;
//    printf("num of cells is file is : %d\n" , *NUM_CELLS ) ;
//    for ( int i=0 ; i< (*NUM_CELLS) ; i++) {
//        int a1 , a2 , a3 , a4 , a5 , a6 ;
//        float a7 ;
//        fscanf(map_txt, "%d~%d~%d~%d~%d~%d~%f\n", &a1, &a2, &a3, &a4, &a5, &a6, &a7);
//        printf("%d~%d~%d~%d~%d~%d~%f\n", a1, a2, a3, a4, a5, a6, a7) ;
//        map_arr[i].counter = a1 ;
//        map_arr[i].owner_id = a2 ;
//        map_arr[i].x = a3 ;
//        map_arr[i].y = a4 ;
//        map_arr[i].IS_MILITARY = a5 ;
//        map_arr[i].RELATED_MILITARY_COUNTER = a6 ;
//        map_arr[i].soldiers_number = a7 ;
//    }
//    fclose(map_txt) ;
//}


////////////////////////////// main game ////////////////////////////////
SDL_DestroyRenderer(sdlRenderer) ;
SDL_DestroyWindow(sdlWindow) ;

//    StartNewGame( 3 , WIDTH , HEIGHT , 80 , 28 , 18 , 16 ) ;

//
//    // game main sdlWindow
//    SDL_Window *sdlWindow = SDL_CreateWindow("State.io" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , WIDTH , HEIGHT , SDL_WINDOW_OPENGL) ;
//
//    // game main sdlRenderer
//    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED) ;
//
//    SDL_Texture *castle_texture = getImageTexture(sdlRenderer , "../img/castle.bmp") ;
//
//    // creating white screen for entrance
//    SDL_SetRenderDrawColor(sdlRenderer , 0xff , 0xff , 0xff , 0xff ) ;
//    SDL_RenderClear(sdlRenderer) ;
//
//    // Generating map_arr
//    land* map_arr = malloc(sizeof(land) * (NUM_OF_ROWS * NUM_OF_COLS) ) ;
//    if ( map_arr == NULL ) {
//        fprintf(stderr, "map allocation error");
//        return 0;
//    }
//
////    int NUM_OF_CELLS ;
////    ImportMap(map_arr , "../dat/map/map_sample.dat" , &NUM_OF_CELLS) ;
//    map_arr = GENERATE_HEXAGON_RANDOM_MAP(NUMBER_OF_PLAYERS , NUM_OF_COLS , NUM_OF_ROWS , map_arr , HEXAGON_A ) ;
//    int NUM_OF_CELLS = ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_ROWS*NUM_OF_COLS , HEXAGON_A , castle_texture) ;
//
////    char* name = "../dat/map/map_sample.dat" ;
////    ExportMap(map_arr , name , NUM_OF_CELLS) ;
//
//
//    printf("num of cells is : %d\n" , NUM_OF_CELLS) ;
//    // an array to save number of cells owned by each owner_id
//    int* CELLS_OWNED = calloc( NUMBER_OF_PLAYERS , sizeof(int)) ;
//
//    // an array to save counter of each owner_id lands
//    int** LANDS_OWNED_COUNTERS = malloc(  NUMBER_OF_PLAYERS * sizeof(int*) ) ;
//    for ( int i=0 ; i<NUMBER_OF_PLAYERS ; i++)
//        LANDS_OWNED_COUNTERS[i] = calloc( NUM_OF_CELLS , sizeof(int)) ;
//
//    // updating map info
//    UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS ) ;
//
//    // getting background picture as texture
//    SDL_Texture *img = getImageTexture(sdlRenderer , "../back.bmp") ;
//    SDL_Rect texture_rect = {.x=0, .y=0, .w=WIDTH, .h=HEIGHT};
//
//    // getting go_back_to_menu picture as texture
//    SDL_Texture *back_to_menu = getImageTexture(sdlRenderer , "../go_back_to_menu.bmp") ;
//    SDL_Rect back_to_menu_texture_rect = {.x=0, .y=0, .w=40, .h=40};
//
//    // Initializing potion textures
//    InitializePotionGraphics(sdlRenderer , POTION_GRAPHIC) ;
//    SDL_Rect potion_rect = {.x=0, .y=0, .w=30 , .h=30};
//
//    /////////////////variable definitions/////////////////
//
//    // game primary counter
//    int i=1 ;
//    SDL_bool shallExit = SDL_FALSE ;
//    SDL_bool shallShowMenu = SDL_TRUE ;
//    // variable to check mouse status
//    int click_status = 0 ;
//    // bunch of needed variables for user interaction
//    Sint16 Origin_x = 0 ;
//    Sint16 Origin_y = 0 ;
//    int Origin_counter = 0 ;
//    Sint16 Destination_x = 0 ;
//    Sint16 Destination_y = 0 ;
//    // variable containing data of potion that is on screen
//    Potion live_time_potion = {.potion_id=-1} ;
//
//    //////////////////////////////////////////////////////
//    // a very important array to save moving soldiers data
//    OneSoldier** AllSoldiersArray = calloc(2*NUM_OF_CELLS , sizeof(OneSoldier*) ) ;
//
//    // a very important array to save activated Potions data
//    OnePotionEffect* AllPotionsArray = calloc(NUMBER_OF_PLAYERS , sizeof(OnePotionEffect)) ;
//    for ( int temp=0 ; temp<NUMBER_OF_PLAYERS ; temp++)
//        AllPotionsArray[temp].potion_id = -1 ;
//
//    //////////////// game /////////////////
//    while ( !shallExit )
//    {
//        UpdateMapInfo(map_arr , NUM_OF_CELLS , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS) ;
//        i %= 200000 ;
//        while ( shallShowMenu )
//        {
//            int condition = ShowMenu(sdlWindow , sdlRenderer , WIDTH , HEIGHT , img) ;
//            if ( condition ==1  )
//                shallShowMenu = SDL_FALSE ;
//            else if ( condition == 0 )
//            {
//                printf("User quited the game successfully \n") ;
//                shallExit = SDL_TRUE ;
//                break;
//            }
//        }
//
//        DrawBackground(sdlRenderer , img , HEIGHT , WIDTH) ;
//        ShowHexagonBackground( sdlRenderer , map_arr , NUM_OF_CELLS , HEXAGON_A , castle_texture) ;
//
//        if ( i%50 == 20 ) {
//            for ( int j=2 ; j<NUMBER_OF_PLAYERS ; j++)
//                SystemMakeMovement(j, AllSoldiersArray, map_arr, CELLS_OWNED
//                                   , LANDS_OWNED_COUNTERS, 5
//                                   , SOLDIERS_POWER_ARRAY , IMMUNE_LANDS_ARRAY );
//        }
//
//        if ( i%80 == 0 )
//            AddSoldiers(map_arr , NUM_OF_CELLS , PRODUCTION_RATE_ARRAY ) ;
//
//        if ( i%550 == 50 )
//            live_time_potion = CreatePotion(WIDTH , HEIGHT) ;
//
//
//        CheckForSoldierPotionConflict(AllSoldiersArray, &live_time_potion, AllPotionsArray);
//        UpdatePotionEffectArray(AllPotionsArray , NUMBER_OF_PLAYERS) ;
//        ApplyPotionEffect(AllPotionsArray , SPEED_ARRAY , SOLDIERS_POWER_ARRAY , PRODUCTION_RATE_ARRAY , IMMUNE_LANDS_ARRAY , NUMBER_OF_PLAYERS) ;
//        RenderPotion(sdlRenderer , live_time_potion , POTION_GRAPHIC ) ;
//
//        SDL_Event sdlEvent;
//        land clicked_cell_info ;
//
//        if ( click_status == 1 )
//            AimAssist(sdlRenderer , sdlEvent , map_arr , NUM_OF_CELLS , HEXAGON_A , Origin_x , Origin_y ) ;
//
//
//        while (SDL_PollEvent(&sdlEvent)) {
//            Sint32 x = sdlEvent.motion.x ;
//            Sint32 y = sdlEvent.motion.y ;
//            switch (sdlEvent.type)
//            {
//                case SDL_QUIT:
//                    printf("User quited the game successfully !\n") ;
//                    shallExit = SDL_TRUE;
//                    break;
//                case SDL_MOUSEMOTION:
//                    break;
//                case SDL_MOUSEBUTTONDOWN:
//                    if ( x<40 && y<40 )
//                        shallShowMenu = SDL_TRUE ;
//
//                    clicked_cell_info = GiveClickedCellInfo(x , y , map_arr , NUM_OF_CELLS , HEXAGON_A) ;
//                    if ( clicked_cell_info.owner_id == 1 && click_status == 0 )
//                    {
//                        click_status += 1 ;
//                        click_status %= 2 ;
//                        Origin_x = clicked_cell_info.x ;
//                        Origin_y = clicked_cell_info.y ;
//                        Origin_counter = clicked_cell_info.counter ;
//                    }
//                    else if ( clicked_cell_info.owner_id != -1 && click_status == 1 )
//                    {
//                        click_status += 1 ;
//                        click_status %= 2 ;
//                        Destination_x = clicked_cell_info.x ;
//                        Destination_y = clicked_cell_info.y ;
//                        // should send soldiers from origin to destination //
//                        if ( Origin_counter != clicked_cell_info.counter && map_arr[Origin_counter].soldiers_number != 0
//                            && IMMUNE_LANDS_ARRAY[clicked_cell_info.owner_id] != 1 && map_arr[Origin_counter].owner_id == 1 )
//                            CreateLineOfSoldiers(AllSoldiersArray , map_arr , Origin_counter , clicked_cell_info , SOLDIERS_POWER_ARRAY) ;
//                    }
//                    SDL_Delay(150) ;
//                    break;
//                case SDL_MOUSEBUTTONUP:
//                    break;
//            }
//        }
//
//
//        SoldierConflictSolver(AllSoldiersArray ) ;
//        ShowLinesOfSoldiers(sdlRenderer , AllSoldiersArray , HEXAGON_A , map_arr , SPEED_ARRAY) ;
//
//        UpdateScore(map_arr , CELLS_OWNED , LANDS_OWNED_COUNTERS , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY) ;
//        if ( CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS) != -1 )
//        {
//            printf("player %d has won \n" , CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)) ;
//            GLOBAL_POINTS_ARRAY[CheckWinState(CELLS_OWNED , NUMBER_OF_PLAYERS)] += 400 ;
//            for ( int i=1 ; i<NUMBER_OF_PLAYERS ; i++)
//                printf("user %d with score of %d\n" , i , GLOBAL_POINTS_ARRAY[i]) ;
//            return 0 ;
//        }
//        DisplayScores(sdlRenderer , NUMBER_OF_PLAYERS , GLOBAL_POINTS_ARRAY ) ;
//        DisplayPotionsEffect(sdlRenderer , AllPotionsArray , POTION_GRAPHIC , NUMBER_OF_PLAYERS) ;
//        SDL_RenderPresent(sdlRenderer) ;
//        SDL_Delay(1000/FPS) ;
//        i++;
//    }
//
//
//
//
//    // Freeing some memory for god's sake
//    SDL_free(img) ;
//    SDL_free(back_to_menu) ;
//    free(AllSoldiersArray) ;
//    free(map_arr) ;
//
//
//    // Destroying window and program //
//    SDL_DestroyTexture(img) ;

//    SDL_DestroyRenderer(sdlRenderer) ;
//    SDL_DestroyWindow(sdlWindow) ;

// Quitting SDL and subsystems
//SDL_Quit() ;
//printf("all done") ;
//return  0 ;