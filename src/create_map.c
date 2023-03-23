/**
 * @file create_map.c
 * @author Luca Pourceau et Melvin Maubert
 * @brief Programme qui génère la map et les tiles.
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "create_map.h"

/**
 * @brief Initialise la caméra en fonction du joueur.
 * 
 * @param camera 
 * @param window 
 */
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

int est_valide(int x) {
    return ( (x >= 0) && (x < MAP_SIZE)) ;
}

int getType(int t){
    return t / 10 ;
}

int isWater(int t) {
    if(t >= 0 && t < 10) return 1;
    return 0;
}

int isSand(int t) {
    if(t >= 10 && t < 20) return 1;
    return 0;
}

int isGrass(int t) {
    if(t >= 20 && t < 30) return 1;
    return 0;
}

int compterVoisinMASTER(int (*map)[MAP_SIZE], int x, int y){
    int i, j, k = 0, total = 0 ;
    int values[8] = {1,2,4,128,8,64,32,16} ;
    int val_tile = map[x][y] + 1;
    for(i = x-1; i< (x+1); i++) {
        for(j = (y-1) ; j < (y+1) ; j++) {
            if( (! est_valide(i)) ||  (! est_valide(j)) || (i==x && j==y) ) continue ;
            if (map[i][j] == val_tile) {
                total += values[k];
            }
            k++;
        }
    }
    return total ;
}

SDL_Rect choixTexture(int (*map)[MAP_SIZE], int x, int y) {
    SDL_Rect rect = {0, ( getType(map[x][y]) == SAND ? 0 : 64), 16, 16};
    int val_voisin = compterVoisinMASTER(map, x, y);
    switch(val_voisin) {
        case 0 : 
            rect.x = 96 + ( getType(map[x][y]) * 16 ) ;
            rect.y = ( map[x][y] % 10 ) * 16 ;
            break ;
        case 1 :
            rect.x = 0;
    }
}

/* Compte les voisins sur le coté dans la matrice */
int compterVoisinCote(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int cptr = 0 ;
    int type = getType(map[x][y])+1;
    int places[4] = {map[x+1][y], map[x-1][y], map[x][y+1], map[x][y-1]};

    for(int i = 0; i < 4; i++) {
        if(getType(places[i]) == type) cptr++;
    }

    return cptr;
}

/* Compter les voisins en diagonale*/
int compterVoisinDiag(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int cptr = 0; 
    int type = getType(map[x][y])+1;
    int places[4] = {map[x+1][y+1], map[x-1][y-1], map[x-1][y+1], map[x+1][y-1]};

    for(int i = 0; i < 4; i++) {
        if(getType(places[i]) == type) cptr++;
    }

    return cptr;
}

/**/
int checkPositionVoisinCote(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int type = getType(map[x][y])+1 ;
    if(getType(map[x+1][y]) == type) 
        return 0;
    if(getType(map[x][y+1]) == type) 
        return 16;
    if(getType(map[x-1][y]) == type) 
        return 32;
    if(getType(map[x][y-1]) == type) 
        return 48;
}

int checkPosition2VoisinOppose(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int type = getType(map[x][y])+1 ;
    if(getType(map[x][y+1]) == type && getType(map[x][y-1]) == type ) 
        return 0;
    if(getType(map[x+1][y]) == type && getType(map[x-1][y]) == type ) 
        return 16;
    return -1 ;
}

/* Cherche le seul voisin de coté non sable*/
int checkPositionVoisinCote3voisins(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int type = getType(map[x][y]) ;
    if(getType(map[x+1][y]) == type) 
        return 0;
    if(getType(map[x][y+1]) == type) 
        return 16;
    if(getType(map[x-1][y]) == type) 
        return 32;
    if(getType(map[x][y-1]) == type) 
        return 48;
}

int checkPositionVoisinAngle(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int type = getType(map[x][y])+1 ;
    if(getType(map[x+1][y]) == type && getType(map[x][y-1]) == type) 
        return 0;
    if(getType(map[x+1][y]) == type && getType(map[x][y+1]) == type) 
        return 16;
    if(getType(map[x-1][y]) == type && getType(map[x][y+1]) == type) 
        return 32;
    if(getType(map[x-1][y]) == type && getType(map[x][y-1]) == type) 
        return 48;
}

int checkPositionVoisinDiag(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int type = getType(map[x][y])+1 ;
    if(getType(map[x+1][y-1]) == type) 
        return 0;
    if(getType(map[x+1][y+1]) == type) 
        return 16;
    if(getType(map[x-1][y+1]) == type) 
        return 32;
    if(getType(map[x-1][y-1]) == type) 
        return 48;
}

int checkPosition2VoisinDiag(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int type = getType(map[x][y])+1 ;
    if(getType(map[x+1][y-1]) == type && getType(map[x+1][y+1]) == type) 
        return 0;
    if(getType(map[x+1][y+1]) == type && getType(map[x-1][y+1]) == type) 
        return 16;
    if(getType(map[x-1][y+1]) == type && getType(map[x-1][y-1]) == type) 
        return 32;
    if(getType(map[x-1][y-1]) == type && getType(map[x+1][y+1]) == type) 
        return 48;
}

int checkPosition2VoisinDiagOppose(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    int type = getType(map[x][y])+1 ;
    if(getType(map[x+1][y+1]) == type && getType(map[x-1][y-1]) == type) 
        return 32;
    if(getType(map[x+1][y-1]) == type && getType(map[x-1][y+1]) == type) 
        return 48;
    return -1 ;
}


/**
 * @brief Algo choix tile pour une case.
 * 
 * @param map 
 * @param x 
 * @param y 
 * @return coord_t 
 */
coord_t choixTile(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    coord_t coord;
    coord.y = (isWater(map[x][y]) ? 64 : 0);
    int nbVoisin = compterVoisinCote(map, x, y);
    int y_temp;
    if( nbVoisin == 1){ //Colonne 1
        coord.x = 0 ;
        coord.y += checkPositionVoisinCote(map, x, y) ;
        return coord ;
    }
    if(nbVoisin == 2) {
        y_temp = checkPosition2VoisinOppose(map, x, y);
        if(y_temp != -1) {
            coord.x = 64;
            coord.y += y_temp ;
            return coord ;
        }
        else {
            coord.x = 16;
            coord.y += checkPositionVoisinAngle(map, x, y) ;
            return coord ;
        }
    }
    if (nbVoisin == 3) {
        coord.x = 48 ;
        coord.y += checkPositionVoisinCote3voisins(map, x, y);
        return coord ;
    }
    if(nbVoisin == 0) {
        nbVoisin = compterVoisinDiag(map,x, y);
        if(nbVoisin == 0){
            if(isWater(map[x][y])) {
                coord.x = 96;
            }
            else if(isSand(map[x][y])) {
                coord.x = 112;
            }
            else { //isGrass
                coord.x = 128;
            }
            coord.y = map[x][y] % 10 * 16;
            return coord ;
        }
        if (nbVoisin == 1 ){
            coord.x = 32 ;
            coord.y += checkPositionVoisinDiag(map, x, y);
            return coord ;
        }
        if( nbVoisin == 2 ) {
            y_temp = checkPosition2VoisinDiagOppose(map, x, y);
            if(y_temp != -1 ){
                coord.x = 64 ;
                coord.y += y_temp ;
                return coord ;
            }
            else {
                coord.x = 80 ;
                coord.y += checkPosition2VoisinDiag(map, x, y) ;
                return coord ;
            }
        }
        
    }
    coord.x = 80;
    coord.y = 0;
    return coord ;
}

/**
 * @brief Fait le rendu de la map.
 * 
 * @param render 
 * @param map 
 * @param camera 
 */
void renderMap(SDL_Renderer** render, map_t* map, camera_t* camera) {

    SDL_Surface *tileset;
    SDL_Surface *sand;
    tileset = IMG_Load("resources/tileset_ground.png");
    SDL_Texture *grass_sand = SDL_CreateTextureFromSurface(*render, tileset);
    SDL_FreeSurface(tileset);

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
                
                SDL_RenderCopy(*render, tileset, &src, &dest);
            }
            else {
                SDL_RenderCopy(*render, tileset, &src, &dest);
            }
        }
    }
}

/**
 * @brief Create a map object.
 * 
 */
void create_map() {
    //Initialisation de la camera
    camera = malloc(sizeof(camera_t));
    initCamera(camera, window);

    //Création de la map
    map = malloc(sizeof(map_t)); 
    build_map(&map);
}

/**
 * @brief Affiche la map.
 * 
 */
void afficherMap(){
    renderMap(&renderer, map, camera);
}