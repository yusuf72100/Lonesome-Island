//C includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Personnal includes
#include "defs.h"
#include "player.h"
#include "map.h"
#include "camera.h"

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



void setTileSource(SDL_Rect* src, map_t* map, int x, int y) {
    // src->x = map->map_coord[x][y].x ;
    // src->y = map->map_coord[x][y].y ;
    coord_t result = choixTile(map->ground, map->id_tiles, x, y);
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

            src.x = map->coord.x[currentTile.x][currentTile.y];
            src.y = map->coord.y[currentTile.x][currentTile.y];

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

    SDL_Rect src = {player->animation_state*PLAYER_W_RESOLUTION, player->facing*PLAYER_H_RESOLUTION, PLAYER_W_RESOLUTION, PLAYER_H_RESOLUTION};
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