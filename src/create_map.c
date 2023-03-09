/**
 * @file create_map.c
 * @author Yusuf Ulas
 * @brief Programme qui génère la map et les tiles.
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "create_map.h"

void initCamera(camera_t* camera, SDL_Window* window) {
    camera->startPosition.x = 0;
    camera->startPosition.y = 0;
    camera->wRender = 90;

    //Récupération de la taille de la fenètre pour adapter la vue sur le jeu
    int w, h, tileSize;
    SDL_GetWindowSize(window, &w, &h);
    tileSize = w/camera->wRender;

    camera->tileSizeOnRender = tileSize;
    camera->hRender = h/tileSize;
}

/* Compte les voisins sur le coté dans la matrice */
int compterVoisinCote(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int cpt=0 ;
    int valCase = map[x][y]+1;
    if(map[x+1][y] == valCase) 
        cpt++ ;
    if(map[x-1][y] == valCase) 
        cpt++ ;
    if(map[x][y+1] == valCase) 
        cpt++ ;
    if(map[x][y-1] == valCase) 
        cpt++ ;
    return cpt ;
}

/* Compter les voisins en diagonale*/
int compterVoisinDiag(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int cpt=0; 
    int valCase = map[x][y]+1;
    if(map[x+1][y+1] == valCase) 
        cpt++ ;
    if(map[x-1][y-1] == valCase) 
        cpt++ ;
    if(map[x-1][y+1] == valCase) 
        cpt++ ;
    if(map[x+1][y-1] == valCase) 
        cpt++ ;
    return cpt ;
}

/**/
int checkPositionVoisinCote(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y]+1 ;
    if(map[x+1][y] == valCase) 
        return 0;
    if(map[x][y+1] == valCase) 
        return 16;
    if(map[x-1][y] == valCase) 
        return 32;
    if(map[x][y-1] == valCase) 
        return 48;
}

/* Cherche le seul voisin de coté non sable*/
int checkPositionVoisinCote3voisins(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y] ;
    if(map[x+1][y] == valCase) 
        return 0;
    if(map[x][y+1] == valCase) 
        return 16;
    if(map[x-1][y] == valCase) 
        return 32;
    if(map[x][y-1] == valCase) 
        return 48;
}

int checkPositionVoisinAngle(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y]+1 ;
    if(map[x+1][y] == valCase && map[x][y-1] == valCase) 
        return 0;
    if(map[x+1][y] == valCase && map[x][y+1] == valCase) 
        return 16;
    if(map[x-1][y] == valCase && map[x][y+1] == valCase) 
        return 32;
    if(map[x-1][y] == valCase && map[x][y-1] == valCase) 
        return 48;
}

int checkPositionVoisinDiag(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int valCase = map[x][y]+1 ;
    if( (map[x+1][y-1] == valCase) ) 
        return 0;
    if( (map[x+1][y+1] == valCase) ) 
        return 16;
    if( (map[x-1][y+1] == valCase) ) 
        return 32;
    if( (map[x-1][y-1] == valCase) ) 
        return 48;
}

/* Algo choix tile pour une case */
coord_t choixTile(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    coord_t coord;
    int nbVoisin = compterVoisinCote(map, x, y);
    if( nbVoisin == 1){ //Colonne 1
        coord.x = 0 ;
        coord.y = checkPositionVoisinCote(map, x, y) ;
        return coord ;
    }
    if(nbVoisin == 2) {
        coord.x = 16;
        coord.y = checkPositionVoisinAngle(map, x, y) ;
        return coord ;
    }
    if (nbVoisin == 3) {
        coord.x = 48 ;
        coord.y = checkPositionVoisinCote3voisins(map, x, y);
        return coord ;
    }
    if(nbVoisin == 0) {
        nbVoisin = compterVoisinDiag(map,x, y);
        if(nbVoisin == 0){
            coord.x = 80 ;
            coord.y = (map[x][y] == GRASS ? 16 : 32) ;
            return coord ;
        }
        if (nbVoisin == 1 ){
            coord.x = 32 ;
            coord.y = checkPositionVoisinDiag(map, x, y);
            return coord ;
        }
        
    }
    coord.x = 80;
    coord.y = 0;
    return coord ;
}

//Affiche le jeu selon la camera donnée
void renderMap(SDL_Renderer** render, map_t* map, camera_t* camera) {

    SDL_Surface *grass;
    SDL_Surface *sand;
    grass = IMG_Load("resources/grass_sand_test.png");
    sand = IMG_Load("resources/sand_water_test2.png");
    SDL_Texture *grass_sand = SDL_CreateTextureFromSurface(*render, grass);
    SDL_Texture *sand_water = SDL_CreateTextureFromSurface(*render, sand);
    SDL_FreeSurface(grass);
    SDL_FreeSurface(sand);

    SDL_Rect src = {0, 0, 16, 16};
    SDL_Rect dest = {0, 0, camera->tileSizeOnRender, camera->tileSizeOnRender};

    coord_t temp;

    for (int i = 0; i <= camera->wRender; i++) {
        for (int j = 0; j <= camera->hRender; j++) {
            temp = choixTile(map->ground,i,j);
            src.x = temp.x;
            src.y = temp.y;

            dest.x = i  * camera->tileSizeOnRender;
            dest.y = j * camera->tileSizeOnRender;

            if(map->ground[i][j] == WATER) {
                SDL_RenderCopy(*render, sand_water, &src, &dest);
            }
            else {
                SDL_RenderCopy(*render, grass_sand, &src, &dest);
            }
        }
    }
}

void create_map() {
    //Initialisation de la camera
    camera = malloc(sizeof(camera_t));
    initCamera(camera, window);

    //Création de la map
    map = malloc(sizeof(map_t)); 
    build_map(&map);
}

void afficherMap(){
    renderMap(&renderer, map, camera);
}