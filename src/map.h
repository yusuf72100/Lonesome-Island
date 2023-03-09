/**
 * @file map.h
 * @author Luca Pourceau et Melvin Maubert
 * @brief Fichier header à map.c
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defs.h"

#define MAP_SIZE 100
#define SMOOTH_MAP 4

// Sur une base 10
#define GRASS_RATIO 7
#define SAND_RATIO 8

// Sur une base 100
#define TREE_RATIO 10
#define ROCK_RATIO 3

typedef struct map_s{
    int ground[MAP_SIZE][MAP_SIZE];
    int utils[MAP_SIZE][MAP_SIZE];
}map_t;

enum ground{ WATER, SAND, GRASS};

enum utils{ TREE=3, ROCK};

void print_map(int map[MAP_SIZE][MAP_SIZE]);

void build_map(map_t** map);

#endif