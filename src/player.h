#ifndef PLAYER_h_
#define PLAYER_h_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define MAX_ANIMATION 4
#define MOVES_ON_TILE 4
#define PLAYER_W_RESOLUTION 16
#define PLAYER_H_RESOLUTION 16



/**
 * @brief Structure du joueur.
 */
typedef struct player_s{
    coord_t mapPosition;
    coord_t tilePosition;
    SDL_Texture* tileset;
    int facing;
    char playername[16];
    int animation_state;
    int connected;
    int health;
    int thirst;
    int stuck;
}player_t;

void initPlayer(SDL_Renderer* renderer, player_t* player);

void nextAnimationState(player_t* player);

void movePlayer(map_t* map, player_t* player, int direction);

#endif