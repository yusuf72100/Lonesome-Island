#include <stdio.h>
#include <stdlib.h>

#include <defs.h>
#include <player.h>
#include <map.h>
#include <SDL.h>
#include <SDL_image.h>

void initPlayer(SDL_Renderer* renderer, player_t* player) {

    //Position sur la map
    player->mapPosition.x = 50;
    player->mapPosition.y = 50;

    //Decalage en pixel
    player->tilePosition.x = 0;
    player->tilePosition.y = 0;

    //Direction du joueur
    player->facing = SOUTH;

    //Animation du joueur
    player->animationState = 0;

    //Tileset du joueur
    SDL_Surface* tmp = IMG_Load("resources/player_animations.png");
    player->tileset = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
}

void nextAnimationState(player_t* player) {
    player->animationState = (player->animationState == MAX_ANIMATION - 1 ? 0 : player->animationState + 1);
}

int isColisionOnTile(map_t* map, int x, int y) {
    if(x < 0 || x > MAP_SIZE || y < 0 || y > MAP_SIZE) return 1;
    if(isWater(map->ground[x][y]) || map->utils[x][y] != -1) return 1;
    return 0;
}

void movePlayer(map_t* map, player_t* player, int direction) {
    if(direction != player->facing) {
        player->facing = direction;
        return;
    }
    coord_t mapPos = player->mapPosition;
    coord_t tilePos = player->tilePosition;
    switch(direction) {
        case NORTH :
            if(isColisionOnTile(map, mapPos.x, mapPos.y)) break;
            if(tilePos.y == 0) {
                mapPos.y--;
                tilePos.y = MOVES_ON_TILE - 1;
            }
            else {
                tilePos.y--;
            }
            break;
        case EAST :
            if(tilePos.x == MOVES_ON_TILE - 1) {
                mapPos.x++;
                tilePos.x = 0;
            }
            else {
                tilePos.x++;
            }
            break;
        case SOUTH :
            if(tilePos.y == MOVES_ON_TILE - 1) {
                mapPos.y++;
                tilePos.y = 0;
            }
            else {
                tilePos.y++;
            }
            break;
        case WEST :
            if(tilePos.x == 0) {
                mapPos.x--;
                tilePos.x = MOVES_ON_TILE - 1;
            }
            else {
                tilePos.x--;
            }
            break;
    }
    if(isColisionOnTile(map, mapPos.x, mapPos.y)) return;
    player->mapPosition.x = mapPos.x;
    player->mapPosition.y = mapPos.y;
    player->tilePosition.x = tilePos.x;
    player->tilePosition.y = tilePos.y;
}