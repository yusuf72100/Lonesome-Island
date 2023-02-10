#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include "defs.h"
 
#define TILE_SIZE 16
#define MAP_SIZE 100
#define RENDER 10
#define TOTALRENDER 21

#define BIOME 0
#define UTILS 1

#define GRASS 0
#define WATER 2
#define CONSTRUCTOR 3

typedef enum {NORTH, EAST, SOUTH, WEST} facing_e;

int map[MAP_SIZE][MAP_SIZE];
int quit = 0;

typedef struct position_s{
    int x;
    int y;
}position_t;

typedef struct camera_s {
    position_t start;
    int XRender;
    int YRender;
}camera_t;

typedef struct player_s {
    char* name;
    position_t pos;
    facing_e facing;
    SDL_Texture* texture;
}player_t;

player_t* createPlayer(char* name, int x, int y, facing_e facing) {
    player_t* player;
    player->name = name;
    position_t pos;
    pos.x = x;
    pos.y = y;
    player->pos = pos;
    player->facing = NORTH;
    return player;
}

void setPlayerFacing(SDL_Renderer* render, player_t* player, facing_e face) {
    SDL_Surface* img = IMG_Load("resources/player_faces.png");
    SDL_Surface* final = SDL_CreateRGBSurface(0, 16, 16, 32, 0, 0, 0, 0);
    
    SDL_Rect src = {face*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
    SDL_Rect dest = {0, 0, TILE_SIZE, TILE_SIZE};

    SDL_BlitSurface(img, &src, final, &dest);
    player->texture = SDL_CreateTextureFromSurface(render, final);
    player->facing = face;

    SDL_FreeSurface(img);
    SDL_FreeSurface(final);
}

void init_border_map(int map[MAP_SIZE][MAP_SIZE]) {
    int coin = MAP_SIZE*0.1;
    for(int i = coin; i < MAP_SIZE-coin; i++) {
        for(int j = coin; j < MAP_SIZE-coin; j++){
            map[i][j] = GRASS;
        }
    }
}

void init_map(int map[MAP_SIZE][MAP_SIZE]) {
    int i, j, text;
    for(i = 0; i < MAP_SIZE; i++) {
        for(j = 0; j < MAP_SIZE; j++) {
            text = WATER;
            if(i == 0 || j == 0) text = CONSTRUCTOR;
            map[i][j] = text;
        }     
    }
    init_border_map(map);
}

int movePlayer(player_t *player, int map[MAP_SIZE][MAP_SIZE], int facing) {

    position_t transfer = {player->pos.x, player->pos.y};
    
    switch(facing) {
        case NORTH:
            transfer.y--;
            break;
        case EAST:
            transfer.x++;
            break;
        case SOUTH:
            transfer.y++;
            break;
        case WEST:
            transfer.x--;
            break;
    }

    if(transfer.x < 0 || transfer.x > MAP_SIZE-1 || transfer.y < 0 || transfer.y > MAP_SIZE) return 0;

    player->pos.x = transfer.x;
    player->pos.y = transfer.y;
    return 1;
}

void renderMapOnPlayer(SDL_Renderer **render, int map[MAP_SIZE][MAP_SIZE], SDL_Window* window, player_t* player) {
    
    SDL_Surface *tileset;
    tileset = IMG_Load("resources/tileset.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(*render, tileset);
    SDL_FreeSurface(tileset);
    
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    int tileSize = w/TOTALRENDER;
    int totalYRender = h/tileSize;
    int YRender = totalYRender%2 == 0 ? totalYRender/2-1 : totalYRender/2;

    position_t start = {player->pos.x - RENDER, player->pos.y - YRender};
    
    SDL_Rect src = {0, 0, TILE_SIZE, TILE_SIZE};
    SDL_Rect dest = {0, 0, tileSize, tileSize};

    for(int i = 0; i <= TOTALRENDER; i++) {
        for(int j = 0; j <= totalYRender; j++) {
            src.x = map[i+start.x][j+start.y]*TILE_SIZE;
            src.y = BIOME;

            dest.x = i*tileSize;
            dest.y = j*tileSize;

            SDL_RenderCopy(*render, texture, &src, &dest);
        }
    }

    src.x = 0;
    src.y = 0;

    dest.x = (RENDER)*tileSize;
    dest.y = (YRender)*tileSize;

    SDL_RenderCopy(*render, player->texture, &src, &dest);
}

void afficher_map(int map[MAP_SIZE][MAP_SIZE]) {
    int i, j;
    for(i = 0; i < MAP_SIZE; i++) {
        for(j = 0; j < MAP_SIZE; j++) {
            printf("%2d", map[i][j]);
        }  
    printf("\n");
    }
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    player_t* player = createPlayer("IzeLeam", 16, 18, NORTH);
    setPlayerFacing(render, player, NORTH);
    
    int map[MAP_SIZE][MAP_SIZE];
    init_map(map);

    renderMapOnPlayer(&render, map, window, player);
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
                    renderMapOnPlayer(&render, map, window, player);
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
                   
                    setPlayerFacing(render, player, facing);
                   
                    if(movePlayer(player, map, facing)) {
                        renderMapOnPlayer(&render, map, window, player);
                        printf("Player : x=%d | y=%d\n", player->pos.x, player->pos.y);
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