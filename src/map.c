#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "defs.h"
#include <SDL_image.h>

int getType(int);

void print_map(int map[MAP_SIZE][MAP_SIZE])
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            printf("%3d", map[i][j]);
        }
        printf("\n");
    }
}

void mapCopy(int src[MAP_SIZE][MAP_SIZE], int dest[MAP_SIZE][MAP_SIZE])
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

int checkSameNeighboor(int ground[MAP_SIZE][MAP_SIZE], int x, int y)
{

    int cptr = 0;
    int type = ground[x][y] / 10;
    int places[4] = {ground[x + 1][y], ground[x][y + 1], ground[x - 1][y], ground[x][y - 1]};

    for (int i = 0; i < 4; i++)
    {
        if (places[i] / 10 == type)
            cptr++;
    }

    return cptr;
}

int isWater(int t)
{
    if (t >= 0 && t < 10)
        return 1;
    return 0;
}

int isSand(int t)
{
    if (t >= 10 && t < 20)
        return 1;
    return 0;
}

int isGrass(int t)
{
    if (t >= 20 && t < 30)
        return 1;
    return 0;
}

int getType(int t)
{
    return t / 10;
}

int getVariant(int v)
{
    return v % 10;
}

void getTreeDimensions(int t, int *w, int *h)
{
    switch (t)
    {
    case 0:
        *w = 1;
        *h = 1;
        break;
    case 1:
        *w = 2;
        *h = 2;
        break;
    case 2:
        *w = 3;
        *h = 2;
        break;
    case 3:
        *w = 2;
        *h = 1;
        break;
    }
}

int isAnyGrassNear(int ground[MAP_SIZE][MAP_SIZE], int x, int y, int range)
{
    int currentTile;
    for (int i = 0; i < range * 2 + 1; i++)
    {
        for (int j = 0; j < range * 2 + 1; j++)
        {
            currentTile = ground[x + i - range][y + j - range];
            if (isGrass(currentTile))
                return 1;
        }
    }
    return 0;
}

int isAnySandNear(int ground[MAP_SIZE][MAP_SIZE], int x, int y, int range)
{
    int currentTile;
    for (int i = 0; i < range * 2 + 1; i++)
    {
        for (int j = 0; j < range * 2 + 1; j++)
        {
            currentTile = ground[x + i - range][y + j - range];
            if (isSand(currentTile))
                return 1;
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
                ground[i][j] = rand() % 7 + 20;
            }
        }
    }
}

void addBeachTransition(int ground[MAP_SIZE][MAP_SIZE])
{

    // Premier ajout de sable
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (isWater(ground[i][j]) && isAnyGrassNear(ground, i, j, 1))
            {
                ground[i][j] = rand() % 7 + 10;
            }
        }
    }

    int groundCopy[MAP_SIZE][MAP_SIZE];
    mapCopy(ground, groundCopy);

    // Rendre les plages plus naturelles
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (isWater(ground[i][j]) && isAnySandNear(ground, i, j, 1) && rand() % 10 + 1 < SAND_RATIO)
            {
                groundCopy[i][j] = rand() % 7 + 10;
            }
        }
    }
    // Rendre les plages plus fluides
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (isWater(ground[i][j]) && checkSameNeighboor(groundCopy, i, j) <= 1)
            {
                groundCopy[i][j] = rand() % 7 + 10;
            }
        }
    }

    mapCopy(groundCopy, ground);
}

void smoothGroundShape(int ground[MAP_SIZE][MAP_SIZE])
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (checkSameNeighboor(ground, i, j) <= 2)
                ground[i][j] = rand() % 7 + (isWater(ground[i][j]) ? 20 : 0);
        }
    }
}

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

int canBePlaced(map_t *map, int x, int y, int w, int h)
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (isWater(map->ground[x + i][y + j]) || map->utils[x + i][y + j] != -1)
            {
                return 0;
            }
        }
    }
    return 1;
}

void place_tree(int util[MAP_SIZE][MAP_SIZE], int x, int y, int w, int h, int tree)
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            util[x + i][y + j] = 19;
        }
    }
    util[x][y] = tree + 10;
}

void plant_trees(map_t *map) {
    int perc;
    int tree, w, h;

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (!isGrass(map->ground[i][j])) continue;
            perc = rand() % 101;
            if (perc < TREE_RATIO) {
                tree = rand() % 4;
                getTreeDimensions(getVariant(tree), &w, &h);
                if (canBePlaced(map, i, j, w, h))
                    place_tree(map->utils, i, j, w, h, tree);
            }
        }
    }
}

void add_rocks(map_t *map)
{
    int perc;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (isGrass(map->ground[i][j]) && map->utils[i][j] == -1)
            {
                perc = rand() % 101;
                if (perc < TREE_RATIO)
                    map->utils[i][j] = rand() % 1 + 10;
            }
        }
    }
}

/**
 * @brief Initialise les resources de la map.
 * @param map
 */
void init_utils(map_t *map)
{
    // Add plantations
    plant_trees(map);

    // Add rocks
    // add_rocks(map);
}

void init_map(map_t *map)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            map->ground[i][j] = rand() % 7;
            map->utils[i][j] = -1;
        }
    }
}

int est_valide(int x, int y) {
    return ( ! ( (x<0 || x >= MAP_SIZE) || (y<0 || y >= MAP_SIZE) ) ) ;
}

/**
 * @brief Calcule un id en focntion des voisins d'une case, permet de choisir une texture
 * @param map
 * @param x
 * @param y 
 * @return un octet contenant l'id de la case courante
*/
unsigned char calculId(int (*map)[MAP_SIZE], int x, int y){
    int valTile = getType(map[x][y]) ;
    unsigned char total = 0x00;
    // On place la valeurs des 8 voisins dans un tableau s'il sont des coordonnées valides. 
    int places[8] = { 
            est_valide(x-1,y-1) ? map[x-1][y-1] : -1, 
            est_valide(x-1,y) ? map[x-1][y] : -1, 
            est_valide(x-1,y+1) ? map[x-1][y+1] : -1, 
            est_valide(x,y-1) ? map[x][y-1] : -1,
            est_valide(x,y+1) ? map[x][y+1] : -1,
            est_valide(x+1,y-1) ? map[x+1][y-1] : -1,
            est_valide(x+1,y) ? map[x+1][y] : -1, 
            est_valide(x+1,y+1) ? map[x+1][y+1] : -1
    };
    
    for (int i = 0; i < 8; i++) {
        if ( ( getType(places[i]) == (valTile+1) ) &&  places[i] != -1)
            total +=( 1 << i) ;
    }
    return total ;
}

/**
 * @brief Permet de sélectionner la bonne texture dans le tileset
 * @param map
 * @param id L'id est l'octet représentant les voisins d'une case 
 * @param x
 * @param y
 * @return Les coordonnées de la textures dans le tileset (en pixels) 
 */
coord_t choixTile(int (*map)[MAP_SIZE], unsigned char id, int x, int y) {
    coord_t coord ;
    coord.y = (isWater(map[x][y]) ? 96 : 0);
    //Le tableau contients les valeurs que l'on compare aux id
    // Ces valeurs ont été calculé à la main et sont rangé dans les même ordre que le tileset (on parcours le tileset colonnes par colonnes et non lignes par lignes)
    int tab_val[29] = { 74, 88, 82, 26, 
                        24, 66, 18, 10, 72, 80, 
                        2, 8, 64, 16, 
                        133, 164, 133, 37, 
                        129, 36, 33, 160, 132, 5, 
                        4, 1, 32, 128 };
    // Contient la taille de chauqe colonne du tileset, permet de faire varier la longueur de la boucle imbriquée
    int nb_tiles[6] = {4, 6, 4, 4, 6, 4} ;
    int i=0,j=0, k=0, x_res = 0;
    if (id == 0x00) {
        if(isWater(map[x][y])) 
            coord.x = 96;
        else if(isSand(map[x][y])) 
            coord.x = 112;
        else  //isGrass
            coord.x = 128;
        coord.y = map[x][y] % 10 * 16;
        return coord ;
    }
    while (i < 29 ) {
        for(j=0; j < nb_tiles[k]; j++){
            if ( ( id & tab_val[i] ) == tab_val[i]) {
                coord.x = x_res;
                coord.y += j*16 ;
                return coord;
            }
            i++ ;
        }
        k++;
        x_res+=16;
    }
}

/**
 * @brief Attribue un id à chaque cases et choisi puis stocke l'emplacement de la texture dans une matrice
 * @param map
 */
void creerMapTileset(map_t *map) {
    int i, j ;
    unsigned char id ;
    for(i = 0; i < MAP_SIZE ; i++) {
        for(j = 0 ; j < MAP_SIZE ; j++ ) {
            id = calculId(map->ground, i, j);
            map->coord[i][j] = choixTile(map->ground, id, i, j) ;
        }
    }
    
}


void build_map(map_t **map)
{
    // Initialisation du random
    srand(time(NULL));
    *map = malloc(sizeof(map_t));

    // Initialisation de la map
    init_map(*map);

    // Création du sol et des ressources
    init_ground((*map)->ground);
    creerMapTileset(*map);
    init_utils(*map);
}
