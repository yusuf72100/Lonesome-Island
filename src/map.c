/**
 * @file map.c
 * @author Luca Pourceau et Melvin Maubert
 * @brief Fichier comportant les méthodes pour générer la map.
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "map.h"

/**
 * @brief Affiche la matrice de la map.
 * 
 * @param map 
 */
void print_map(int map[MAP_SIZE][MAP_SIZE])
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            printf("%2d", map[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Fait une copie de la map.
 * 
 * @param src 
 * @param dest 
 */
void mapCopy(int src[MAP_SIZE][MAP_SIZE], int dest[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

/**
 * @brief Vérifie si 2 tiles ont le même voisin.
 * 
 * @param ground 
 * @param x 
 * @param y 
 * @return int 
 */
int checkSameNeighboor(int ground[MAP_SIZE][MAP_SIZE], int x, int y)
{

    int cptr = 0;
    int tile = ground[x][y];
    int places[4] = {ground[x + 1][y], ground[x][y + 1], ground[x - 1][y], ground[x][y - 1]};

    for (int i = 0; i < 4; i++)
    {
        if (places[i] == tile)
            cptr++;
    }

    return cptr;
}

/**
 * @brief Cherche la position du tile de terre le plus proche.
 * 
 * @param ground 
 * @param x 
 * @param y 
 * @param range 
 * @return int 
 */
int isAnyGrassNear(int ground[MAP_SIZE][MAP_SIZE], int x, int y, int range) {
    for(int i = 0; i < range*2+1; i++) {
        for(int j = 0; j < range*2+1; j++) {
            if(ground[x+i-range][y+j-range] == GRASS) return 1;
        }
    }
    return 0;
}

/**
 * @brief Cherche la position du tile de sable le plus proche.
 * 
 * @param ground 
 * @param x 
 * @param y 
 * @param range 
 * @return int 
 */
int isAnySandNear(int ground[MAP_SIZE][MAP_SIZE], int x, int y, int range) {
    for(int i = 0; i < range*2+1; i++) {
        for(int j = 0; j < range*2+1; j++) {
            if(ground[x+i-range][y+j-range] == SAND) return 1;
        }
    }
    return 0;
}

void firstRandomPreset(int ground[MAP_SIZE][MAP_SIZE])
{
    int perc;
    int center = MAP_SIZE / 2;

    srand(time(NULL));

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            perc = rand() % MAP_SIZE / 10 * GRASS_RATIO;

            if (perc > abs(center - i) && perc > abs(center - j))
            {
                ground[i][j] = GRASS;
            }
        }
    }
}

/**
 * @brief Ajoute une transition de plage.
 * 
 * @param ground 
 */
void addBeachTransition(int ground[MAP_SIZE][MAP_SIZE]) {

    //Premier ajout de sable
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(ground[i][j] == WATER && isAnyGrassNear(ground, i, j, 1)) {
                ground[i][j] = SAND;
            }
        }
    }

    int groundCopy[MAP_SIZE][MAP_SIZE];
    mapCopy(ground, groundCopy);

    //Rendre les plages plus naturelles
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(ground[i][j] == WATER && isAnySandNear(ground, i, j, 1) && rand()%10+1 < SAND_RATIO) {
                groundCopy[i][j] = SAND;
            }
        }
    }
    //Rendre les plages plus fluides
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(groundCopy[i][j] == WATER && checkSameNeighboor(groundCopy, i, j) <= 1) {
                groundCopy[i][j] = SAND;
            }
        }
    }

    mapCopy(groundCopy, ground);
}

/**
 * @brief Fait un lissage de la matrice.
 * 
 * @param ground 
 */
void smoothGroundShape(int ground[MAP_SIZE][MAP_SIZE])
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (checkSameNeighboor(ground, i, j) <= 2)
                ground[i][j] = (ground[i][j] == WATER ? GRASS : WATER);
        }
    }
}

/**
 * @brief Initialise le sol.
 * 
 * @param ground 
 */
void init_ground(int ground[MAP_SIZE][MAP_SIZE])
{
    // Set ground randomly from map center
    firstRandomPreset(ground);
    // Smooth ground shape
    for (int i = 0; i < SMOOTH_MAP; i++)
    {
        smoothGroundShape(ground);
    }
    // Add sand beach transition
    addBeachTransition(ground);
}

void init_utils(map_t* map)
{
    int perc;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (map->ground[i][j] == GRASS)
            {
                perc = rand() % 101;
                if (perc < TREE_RATIO)
                    map->utils[i][j] = TREE;
            }
        }
    }
}

/**
 * @brief Initialise la map.
 * 
 * @param map 
 */
void init_map(map_t *map)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            map->ground[i][j] = WATER;
            map->utils[i][j] = -1;
        }
    }
}

/**
 * @brief Génère une map.
 * 
 * @param map 
 */
void build_map(map_t** map)
{
    // Initialisation du random
    srand(time(NULL));

    // Initialisation de la map
    init_map(*map);

    // Création du sol et des ressources
    init_ground((*map)->ground);
    //init_utils(*map);
}
