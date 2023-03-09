//C includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Personnal includes
#include <map.h>

//SDL includes
#include <SDL.h>
#include <SDL_image.h>

typedef struct coord_s{
    int x;
    int y;
}coord_t;

typedef struct player_s{
    coord_t position;
}player_t;

typedef struct camera_s{
    int wRender;
    int hRender;
    int tileSizeOnRender;
    coord_t startPosition;
}camera_t;

void initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    SDL_Init(SDL_INIT_EVERYTHING);
    *window = SDL_CreateWindow("Lonesome Island", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1, 1, SDL_WINDOW_MAXIMIZED);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
}

void initPlayer(player_t* player) {
    player->position.x = 50;
    player->position.y = 50;
}

void initCamera(camera_t* camera, SDL_Window* window) {
    camera->startPosition.x = 0;
    camera->startPosition.y = 0;
    camera->wRender = 90;

    //Récupération de la taille de la fenètre pour adapter la vue sur le jeu
    int w, h, tileSize;
    SDL_GetWindowSize(window, &w, &h);
    tileSize = w/camera->wRender;

    camera->tileSizeOnRender = tileSize;
    camera->hRender = h/tileSize;
}

/* Compte les voisins sur le coté dans la matrice */
int compterVoisinCote(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int cpt=0 ;
    int valCase = map[x][y]+1;
    if(map[x+1][y] == valCase) 
        cpt++ ;
    if(map[x-1][y] == valCase) 
        cpt++ ;
    if(map[x][y+1] == valCase) 
        cpt++ ;
    if(map[x][y-1] == valCase) 
        cpt++ ;
    return cpt ;
}

/* Compter les voisins en diagonale*/
int compterVoisinDiag(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int cpt=0; 
    int valCase = map[x][y]+1;
    if(map[x+1][y+1] == valCase) 
        cpt++ ;
    if(map[x-1][y-1] == valCase) 
        cpt++ ;
    if(map[x-1][y+1] == valCase) 
        cpt++ ;
    if(map[x+1][y-1] == valCase) 
        cpt++ ;
    return cpt ;
}

/**/
int checkPositionVoisinCote(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y]+1 ;
    if(map[x+1][y] == valCase) 
        return 0;
    if(map[x][y+1] == valCase) 
        return 16;
    if(map[x-1][y] == valCase) 
        return 32;
    if(map[x][y-1] == valCase) 
        return 48;
}

/* Cherche le seul voisin de coté non sable*/
int checkPositionVoisinCote3voisins(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y] ;
    if(map[x+1][y] == valCase) 
        return 0;
    if(map[x][y+1] == valCase) 
        return 16;
    if(map[x-1][y] == valCase) 
        return 32;
    if(map[x][y-1] == valCase) 
        return 48;
}

int checkPositionVoisinAngle(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y]+1 ;
    if(map[x+1][y] == valCase && map[x][y-1] == valCase) 
        return 0;
    if(map[x+1][y] == valCase && map[x][y+1] == valCase) 
        return 16;
    if(map[x-1][y] == valCase && map[x][y+1] == valCase) 
        return 32;
    if(map[x-1][y] == valCase && map[x][y-1] == valCase) 
        return 48;
}

int checkPositionVoisinDiag(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y]+1 ;
    if( (map[x+1][y-1] == valCase) ) 
        return 0;
    if( (map[x+1][y+1] == valCase) ) 
        return 16;
    if( (map[x-1][y+1] == valCase) ) 
        return 32;
    if( (map[x-1][y-1] == valCase) ) 
        return 48;
}

/* Algo choix tile pour une case */
coord_t choixTile(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    coord_t coord;
    int nbVoisin = compterVoisinCote(map, x, y);
    if( nbVoisin == 1){ //Colonne 1
        coord.x = 0 ;
        coord.y = checkPositionVoisinCote(map, x, y) ;
        return coord ;
    }
    if(nbVoisin == 2) {
        coord.x = 16;
        coord.y = checkPositionVoisinAngle(map, x, y) ;
        return coord ;
    }
    if (nbVoisin == 3) {
        coord.x = 48 ;
        coord.y = checkPositionVoisinCote3voisins(map, x, y);
        return coord ;
    }
    if(nbVoisin == 0) {
        nbVoisin = compterVoisinDiag(map,x, y);
        if(nbVoisin == 0){
            coord.x = 80 ;
            coord.y = (map[x][y] == GRASS ? 16 : 32) ;
            return coord ;
        }
        if (nbVoisin == 1 ){
            coord.x = 32 ;
            coord.y = checkPositionVoisinDiag(map, x, y);
            return coord ;
        }
        
    }
    coord.x = 80;
    coord.y = 0;
    return coord ;
}

//Affiche le jeu selon la camera donnée
void renderMap(SDL_Renderer** render, map_t* map, camera_t* camera) {

    SDL_Surface *grass;
    SDL_Surface *sand;
    grass = IMG_Load("resources/grass_sand_test.png");
    sand = IMG_Load("resources/sand_water_test2.png");
    SDL_Texture *grass_sand = SDL_CreateTextureFromSurface(*render, grass);
    SDL_Texture *sand_water = SDL_CreateTextureFromSurface(*render, sand);
    SDL_FreeSurface(grass);
    SDL_FreeSurface(sand);

    SDL_Rect src = {0, 0, 16, 16};
    SDL_Rect dest = {0, 0, camera->tileSizeOnRender, camera->tileSizeOnRender};

    coord_t temp;

    for (int i = 0; i <= camera->wRender; i++) {
        for (int j = 0; j <= camera->hRender; j++) {
            temp = choixTile(map->ground,i,j);
            src.x = temp.x;
            src.y = temp.y;

            dest.x = i  * camera->tileSizeOnRender;
            dest.y = j * camera->tileSizeOnRender;

            if(map->ground[i][j] == WATER) {
                SDL_RenderCopy(*render, sand_water, &src, &dest);
            }
            else {
                SDL_RenderCopy(*render, grass_sand, &src, &dest);
            }
        }
    }
}

int main(int argc, char** argv) {

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    //Initialisation de la SDL par une fonction ()
    initSDL(&window, &renderer);

    //Initialisation du joueur
    player_t* player = malloc(sizeof(player_t));
    initPlayer(player);

    //Initialisation de la camera
    camera_t* camera = malloc(sizeof(camera_t));
    initCamera(camera, window);

    //Création de la map
    map_t* map = malloc(sizeof(map_t)); 
    build_map(&map);

    //Boucle de jeu ~infini
    int game = 1;
    while(game) {
        SDL_Delay(50);
        while(SDL_PollEvent(&event)) {

            if(event.type == SDL_MOUSEMOTION) continue;

            switch(event.type) {
                case SDL_QUIT :
                    game = 0;
                    break;
            }
        }
        renderMap(&renderer, map, camera);
        SDL_RenderPresent(renderer);
    }
    
    //Suppression des pointeurs et des éléments de SDL
    free(player);
    free(camera);
    free(map);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/*
int main(int argc, char **argv) {
    
    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Event event;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) printf("error");

    window = SDL_CreateWindow("Survival Island", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1350, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(window == NULL || render == NULL) printf("error");
    SDL_SetWindowMinimumSize(window, 500, 500);

    player_t* player = createPlayer("IzeLeam", 16, 18, NORTH);
    setPlayerFacing(render, player, NORTH);

    position_t *camera = init_camera(player->pos);
    
    map_t* map = build_map();

    renderMapOnPlayer(&render, map->ground, window, player, camera);
    SDL_RenderPresent(render);

    int quit = 0;
    while(!quit){

        SDL_Delay(50);
        while(SDL_PollEvent(&event)) {

            if(event.type == SDL_MOUSEMOTION) continue;

            switch(event.type) {
                case SDL_QUIT :
                    quit = 1;
                    break;
                case SDL_WINDOWEVENT_RESIZED : 
                    renderMapOnPlayer(&render, map->ground, window, player);
                    break;
                case SDL_KEYDOWN :
                    int facing = 0;
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_W :
                            facing = NORTH;
                            break;
                        case SDL_SCANCODE_A :
                            facing = WEST;
                            break;
                        case SDL_SCANCODE_S :
                            facing = SOUTH;
                            break;
                        case SDL_SCANCODE_D :
                            facing = EAST;
                            break;
                    }
                   
                    if(player->facing != facing) {
                        setPlayerFacing(render, player, facing);
                        renderMapOnPlayer(&render, map->ground, window, player);
                    }
                    else if(movePlayer(player, map->ground, facing)) {
                        renderMapOnPlayer(&render, map->ground, window, player);
                    }
       
                    break;
            }
            SDL_RenderPresent(render);
        }
    }


    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
*/