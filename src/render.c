//C includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Personnal includes
#include <defs.h>
#include <player.h>
#include <map.h>
#include <camera.h>

//SDL includes
#include <SDL.h>
#include <SDL_image.h>

#define TILE_RESOLUTION 16

int isValid(int x, int y) {
    return(x > 0 && x < MAP_SIZE-1 && y > 0 && y < MAP_SIZE-1);
}

int getPixelTilePos(int tileSize, int tileState) {
    return(tileSize/MOVES_ON_TILE * tileState);
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
    if(getType(map[x-1][y-1]) == type && getType(map[x+1][y-1]) == type) 
        return 48;
}

/* Algo choix tile pour une case */
coord_t choixTile(int map[MAP_SIZE][MAP_SIZE], int x, int y) {
    coord_t coord;
    coord.y = (isWater(map[x][y]) ? 64 : 0);
    int nbVoisin = compterVoisinCote(map, x, y);
    if( nbVoisin == 1){ //Colonne 1
        coord.x = 0 ;
        coord.y += checkPositionVoisinCote(map, x, y) ;
        return coord ;
    }
    if(nbVoisin == 2) {
        coord.x = 16;
        coord.y += checkPositionVoisinAngle(map, x, y) ;
        return coord ;
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
        if (nbVoisin == 2) {
            coord.x = 80;
            coord.y += checkPosition2VoisinDiag(map, x, y);
            return coord;
        }
    }
    coord.x = 96;
    coord.y = 112;
    return coord ;
}

void setTileSource(SDL_Rect* src, map_t* map, int x, int y) {
    coord_t result = choixTile(map->ground, x, y);
    src->x = result.x;
    src->y = result.y;
}

void updateGroundTexture(SDL_Renderer** renderer, SDL_Texture** target, SDL_Window* window, SDL_Texture* tileset, camera_t* camera, map_t* map) {

    if(*target == NULL) {
        Uint32 format;
        int w, h;

        SDL_QueryTexture(tileset, &format, NULL, &w, &h);
        SDL_GetWindowSize(window, &w, &h);

        *target = SDL_CreateTexture(*renderer, format, SDL_TEXTUREACCESS_TARGET, w, h);
    }
    
    SDL_SetRenderTarget(*renderer, *target);
    
    SDL_Rect src, dest;

    src.w = TILE_RESOLUTION;
    src.h = TILE_RESOLUTION;

    dest.w = camera->tileSizeOnRender;
    dest.h = camera->tileSizeOnRender;

    coord_t currentTile;

    for (int i = 0; i < camera->wRender + 1; i++) {
        for (int j = 0; j <= camera->hRender + 1; j++) {
            currentTile.x = i + camera->startPosition.x;
            currentTile.y = j + camera->startPosition.y;

            setTileSource(&src, map, currentTile.x, currentTile.y);

            dest.x = i * camera->tileSizeOnRender - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.x);
            dest.y = j * camera->tileSizeOnRender - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.y);

            SDL_RenderCopy(*renderer, tileset, &src, &dest);
        }
    }

    SDL_SetRenderTarget(*renderer, NULL);
}

void renderMap(SDL_Renderer** renderer, SDL_Texture* big_texture) {

    SDL_RenderCopy(*renderer, big_texture, NULL, NULL);

}

void renderPlayer(SDL_Renderer** renderer, camera_t* camera, player_t* player) {
    int xStart, yStart;
    xStart = camera->tileSizeOnRender * (player->mapPosition.x - camera->startPosition.x) - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.x) + getPixelTilePos(camera->tileSizeOnRender, player->tilePosition.x);
    yStart = camera->tileSizeOnRender * (player->mapPosition.y - camera->startPosition.y) - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.y) + getPixelTilePos(camera->tileSizeOnRender, player->tilePosition.y);

    SDL_Rect src = {player->animationState*PLAYER_W_RESOLUTION, player->facing*PLAYER_H_RESOLUTION, PLAYER_W_RESOLUTION, PLAYER_W_RESOLUTION};
    SDL_Rect dest = {xStart, yStart, camera->tileSizeOnRender, camera->tileSizeOnRender};

    SDL_RenderCopy(*renderer, player->tileset, &src, &dest);
}

/*
void renderUtils(SDL_Renderer** renderer, camera_t* camera, map_t* map) {

    int w, h;

    coord_t currentTile;
    int tile;

    for (int i = 0; i < camera->wRender; i++) {
        for (int j = 0; j < camera->hRender; j++) {
            currentTile.x = i + camera->startPosition.x;
            currentTile.y = j + camera->startPosition.y;
            tile = map->utils[currentTile.x][currentTile.y];

            if (tile == -1) continue;
            if(getType(tile) == TREE) {
                getTreeDimensions(tile, &w, &h);

                
            }

            setTileSource(&src, map, currentTile.x, currentTile.y);

            dest.x = i * camera->tileSizeOnRender - camera->tileSizeOnRender/MOVES_ON_TILE * camera->offsetStartPosition.x ;
            dest.y = j * camera->tileSizeOnRender - camera->tileSizeOnRender/MOVES_ON_TILE * camera->offsetStartPosition.y;

            SDL_RenderCopy(*renderer, tileset, &src, &dest);
        }
    }
}
*/