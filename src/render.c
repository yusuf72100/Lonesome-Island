/**
 * @file map.c
 * @author Luca Pourceau Elvin Maubert
 * @brief Fichier permettant de calculer le rendu de jeu
 */



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
    src->x = map->coord[x][y].x ;
    src->y = map->coord[x][y].y ;
}

void setUtilSource(SDL_Rect* src, int data) {
    if(getType(data) == TREE) {
        src->y = 192;
        switch (getVariant(data)) {
            case 0 :
                src->x = 0;
                return;
            case 1 :
                src->x = 16;
                return;
            case 2 :
                src->x = 48;
                return;
        }
    }
}

void updateGroundTexture(SDL_Renderer** renderer, SDL_Texture** target, SDL_Window* window, SDL_Texture* tileset, camera_t* camera, map_t* map) {

    if(*target == NULL) {
        int w, h;

        SDL_GetWindowSize(window, &w, &h);

        *target = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
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

void updateUtilsTexture(SDL_Renderer** renderer, SDL_Texture** target, SDL_Window* window, SDL_Texture* tileset, camera_t* camera, map_t* map) {

    if(*target == NULL) {

        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        *target = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetTextureBlendMode(*target, SDL_BLENDMODE_BLEND);
    }
    
    SDL_SetRenderTarget(*renderer, *target);

    //Fond transparent de la texture
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
    SDL_RenderClear(*renderer);

    //Remise pour imprimer les textures
    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    
    SDL_Rect src, dest;

    coord_t currentTile;
    int currentData;

    for (int j = 0; j < camera->hRender + 5; j++) {
        for (int i = 0; i <= camera->wRender + 4; i++) {

            currentTile.x = i + camera->startPosition.x;
            currentTile.y = j + camera->startPosition.y;
            currentData = map->utils[currentTile.x][currentTile.y];

            if(currentData == -1) continue;

            if(getType(currentData) == TREE && getVariant(currentData) != 9) {

                getTreeRenderDimensions(getVariant(currentData), &src.w, &src.h);
                setUtilSource(&src, currentData);

                dest.x = i * camera->tileSizeOnRender - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.x) - (src.w - 1) * camera->tileSizeOnRender;
                dest.y = j * camera->tileSizeOnRender - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.y) - (src.h - 1) * camera->tileSizeOnRender;
                
                dest.w = src.w * camera->tileSizeOnRender;
                dest.h = src.h * camera->tileSizeOnRender;

                src.w *= TILE_RESOLUTION;
                src.h *= TILE_RESOLUTION;

                SDL_RenderCopy(*renderer, tileset, &src, &dest);
                continue;
            }

            if(getType(currentData) == RAFT) {

                dest.x = i * camera->tileSizeOnRender - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.x);
                dest.y = j * camera->tileSizeOnRender - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.y);
                
                dest.w = camera->tileSizeOnRender;
                dest.h = camera->tileSizeOnRender;

                SDL_RenderFillRect(*renderer, &dest);
                continue;
            }
        }
    }

    SDL_SetRenderTarget(*renderer, NULL);
}

void renderUtils(SDL_Renderer** renderer, SDL_Texture* big_texture) {

    SDL_RenderCopy(*renderer, big_texture, NULL, NULL);

}

void renderPlayer(SDL_Renderer** renderer, camera_t* camera, player_t** players, int nb_joueurs) {

    player_t* player = malloc(sizeof(player_t));
    for(int i = 0; i < nb_joueurs; i++) {
        player = players[i];

        int xStart, yStart;
        xStart = camera->tileSizeOnRender * (player->mapPosition.x - camera->startPosition.x) - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.x) + getPixelTilePos(camera->tileSizeOnRender, player->tilePosition.x);
        yStart = camera->tileSizeOnRender * (player->mapPosition.y - camera->startPosition.y) - getPixelTilePos(camera->tileSizeOnRender, camera->offsetStartPosition.y) + getPixelTilePos(camera->tileSizeOnRender, player->tilePosition.y);

        SDL_Rect src = {player->animation_state*PLAYER_W_RESOLUTION, player->facing*PLAYER_H_RESOLUTION, PLAYER_W_RESOLUTION, PLAYER_H_RESOLUTION};
        src.y += player->isRunning ? 96 : 0;
        SDL_Rect dest = {xStart, yStart - (float) (PLAYER_H_RESOLUTION - PLAYER_W_RESOLUTION)/TILE_RESOLUTION * camera->tileSizeOnRender, (float) PLAYER_W_RESOLUTION/TILE_RESOLUTION * camera->tileSizeOnRender, (float) PLAYER_H_RESOLUTION/TILE_RESOLUTION * camera->tileSizeOnRender};

        SDL_RenderCopy(*renderer, player->tileset, &src, &dest);
    }
    free(player);
}
