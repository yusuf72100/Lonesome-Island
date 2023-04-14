
#include "player.h"
#include "defs.h"
#include <time.h>

/**
 * @brief Initialise les variables du joueur.
 * @param renderer
 * @param player
 */
void initPlayer(SDL_Renderer* renderer, player_t* player, map_t* map) {

    srand(time(NULL));

    //Position sur la map
    coord_t co = {50, 50};
    while(isWater(map->ground[co.x][co.y]) || map->utils[co.x][co.y] != -1) {
        co.x = rand() % MAP_SIZE;
        co.y = rand() % MAP_SIZE;
    }
    printf("at %d %d : %d\n", co.x, co.y, map->ground[co.x][co.y]);

    player->mapPosition.x = co.x;
    player->mapPosition.y = co.y;

    //Decalage en pixel
    player->tilePosition.x = 0;
    player->tilePosition.y = 0;

    //Direction du joueur
    player->facing = SOUTH;
    player->isRunning = 0;
    player->isRunning = 0;

    //Animation du joueur
    player->animation_state = 0;
    player->animationDelay = 250;

    //Tileset du joueur
    SDL_Surface* tmp = IMG_Load("resources/player_new.png");
    player->tileset = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
}

/**
 * @brief Change l'animation du joueur.
 * @param player
 */
void nextAnimationState(player_t* player) {
    int max;
    max = player->isAttack ? 3 : MAX_ANIMATION;
    player->animation_state = (player->animation_state == max - 1 ? 0 : player->animation_state + 1);
}

/**
 * @brief Renvoi vrai si uen collision est détectée.
 * @param map
 * @param x
 * @param y
 * @return
 */
int isColisionOnTile(map_t* map, coord_t mapPos, coord_t tilePos) {
    if(mapPos.x < 0 || mapPos.x > MAP_SIZE || mapPos.y < 0 || mapPos.y > MAP_SIZE) return 1;
    if(isWater(map->ground[mapPos.x][mapPos.y]) || map->utils[mapPos.x][mapPos.y] != -1) return 1;
    return 0;
}

/**
 * @brief Déplace le joueur.
 * @param map
 * @param player
 * @param direction
 */
void movePlayer(map_t* map, player_t* player, int direction) {
    if(direction != player->facing) {
        player->facing = direction;
        return;
    }
    coord_t mapPos = player->mapPosition;
    coord_t tilePos = player->tilePosition;
    switch(direction) {
        case NORTH :
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
    if(isColisionOnTile(map, mapPos, tilePos)) return;
    player->mapPosition.x = mapPos.x;
    player->mapPosition.y = mapPos.y;
    player->tilePosition.x = tilePos.x;
    player->tilePosition.y = tilePos.y;
}