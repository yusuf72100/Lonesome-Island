//C includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Personnal includes
#include "defs.h"
#include "player.h"
#include "map.h"
#include "camera.h"

//SDL includes
#include <SDL.h>
#include <SDL_image.h>

void initCamera(camera_t* camera, SDL_Window* window, player_t* player) {

    camera->offsetStartPosition.x = 0;
    camera->offsetStartPosition.y = 0;
    camera->wRender = RENDER;

    //Récupération de la taille de la fenètre pour adapter la vue sur le jeu
    int w, h, tileSize;
    SDL_GetWindowSize(window, &w, &h);
    camera->wWindow = w;
    camera->hWindow = h;

    tileSize = w/camera->wRender;
    camera->tileSizeOnRender = tileSize;

    camera->hRender = h/tileSize;

    camera->startPosition.x = player->mapPosition.x - (camera->wRender/2);
    camera->startPosition.y = player->mapPosition.y - (camera->hRender/2);
}

int checkPlayerOut(camera_t* camera, player_t* player, int direction) {
    //NORTH
    if(direction == NORTH && player->mapPosition.y + (float) (player->tilePosition.y)/MOVES_ON_TILE < camera->startPosition.y + (float) (camera->offsetStartPosition.x)/MOVES_ON_TILE + (float) (camera->hRender)/5) return 1;
    //EAST
    if(direction == EAST && player->mapPosition.x + (float) (player->tilePosition.x)/MOVES_ON_TILE + 1 > camera->startPosition.x + (float) (camera->offsetStartPosition.x)/MOVES_ON_TILE + camera->wRender - (float) (camera->wRender)/10) return 1;
    //SOUTH
    if(direction == SOUTH && player->mapPosition.y + (float) (player->tilePosition.y)/MOVES_ON_TILE + 1 > camera->startPosition.y + (float) (camera->offsetStartPosition.x)/MOVES_ON_TILE + camera->hRender - (float) (camera->hRender)/5) return 1;
    //WEST
    if(direction == WEST && player->mapPosition.x + (float) (player->tilePosition.x)/MOVES_ON_TILE < camera->startPosition.x + (float) (camera->offsetStartPosition.x)/MOVES_ON_TILE + (float) (camera->wRender)/10) return 1;
    return 0;
}

void moveCamera(camera_t* camera, int direction) {

    switch(direction) {
        case NORTH :
            if(camera->startPosition.y == 0) return;
            if(camera->offsetStartPosition.y == 0) {
                camera->offsetStartPosition.y = MOVES_ON_TILE - 1;
                camera->startPosition.y--;
            }
            else {
                camera->offsetStartPosition.y--;
            }
            break;
        case EAST :
            if(camera->startPosition.x + camera->wRender > MAP_SIZE-1) return;
            if(camera->offsetStartPosition.x == MOVES_ON_TILE - 1) {
                camera->offsetStartPosition.x = 0;
                camera->startPosition.x++;
            }
            else {
                camera->offsetStartPosition.x++;
            }
            break;
        case SOUTH :
            if(camera->startPosition.y + camera->hRender > MAP_SIZE-1) return;
            if(camera->offsetStartPosition.y == MOVES_ON_TILE - 1) {
                camera->offsetStartPosition.y = 0;
                camera->startPosition.y++;
            }
            else {
                camera->offsetStartPosition.y++;
            }
            break;
        case WEST :
            if(camera->startPosition.x == 0) return;
            if(camera->offsetStartPosition.x == 0) {
                camera->offsetStartPosition.x = MOVES_ON_TILE - 1;
                camera->startPosition.x--;
            }
            else {
                camera->offsetStartPosition.x--;
            }
            break;
    }
}
