
#include "player.h"
#include "defs.h"

/**
 * @brief Initialise les variables du joueur.
 * @param renderer
 * @param player
 */
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
    player->animation_state = 0;

    //Tileset du joueur
    SDL_Surface* tmp = IMG_Load("resources/player2.png");
    player->tileset = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
}

/**
 * @brief Change l'animation du joueur.
 * @param player
 */
void nextAnimationState(player_t* player) {
    player->animation_state = (player->animation_state == MAX_ANIMATION - 1 ? 0 : player->animation_state + 1);
}

/**
 * @brief Renvoi vrai si uen collision est détectée.
 * @param map
 * @param x
 * @param y
 * @return
 */
int isColisionOnTile(map_t* map, int x, int y) {
    if(x < 0 || x > MAP_SIZE || y < 0 || y > MAP_SIZE) return 1;
    if(isWater(map->ground[x][y]) || map->utils[x][y] != -1) return 1;
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