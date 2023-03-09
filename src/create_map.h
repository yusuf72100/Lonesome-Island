/**
 * @file create_map.h
 * @author Luca Pourceau et Melvin Maubert
 * @brief Fichier header create_map.c
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#ifndef CREATE_MAP_H
#define CREATE_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <windows.h>
#include "map.h"
#include "defs.h"

typedef struct coord_s{
    int x;
    int y;
}coord_t;

typedef struct camera_s{
    int wRender;
    int hRender;
    int tileSizeOnRender;
    coord_t startPosition;
}camera_t;

static camera_t* camera;
static map_t* map;

void afficherMap();

void create_map();

#endif