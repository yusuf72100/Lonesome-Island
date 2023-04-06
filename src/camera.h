#ifndef CAMERA_h_
#define CAMERA_h_

#include <SDL.h>
#include "player.h"

#define RENDER 25;

typedef struct camera_s{
    int wWindow;
    int hWindow;
    int wRender;
    int hRender;
    int tileSizeOnRender;
    coord_t startPosition;
    coord_t offsetStartPosition;
}camera_t;

void initCamera(camera_t* camera, SDL_Window* window, player_t* player);

int checkPlayerOut(camera_t* camera, player_t* player, int direction);

void moveCamera(camera_t* camera, int direction);

#endif