#ifndef MAP_h_
#define MAP_h_

typedef struct map_s map_t;

#include "player.h"

#define MAP_SIZE 100
#define SMOOTH_MAP 4

// Sur une base 10
#define GRASS_RATIO 7
#define SAND_RATIO 8

// Sur une base 100
#define TREE_RATIO 0
#define ROCK_RATIO 3

typedef struct map_s{
    int ground[MAP_SIZE][MAP_SIZE];
    coord_t coord[MAP_SIZE][MAP_SIZE];
    int utils[MAP_SIZE][MAP_SIZE];
}map_t;

enum ground{ WATER, SAND, GRASS};

enum utils{ TREE = 1, ROCK};

int isWater(int);

int isSand(int);

int isGrass(int);

int getType(int);

int getVarient(int); 

void getTreeDimensions(int, int*, int*);

void print_map(int map[MAP_SIZE][MAP_SIZE]);

void build_map(map_t**);

#endif