#ifndef RENDER_h_
#define RENDER_h_

#include <SDL.h>
#include "map.h"
#include "camera.h"
#include "player.h"

void updateGroundTexture(SDL_Renderer** renderer, SDL_Texture** target, SDL_Window* window, SDL_Texture* tileset, camera_t* camera, map_t* map);

void renderMap(SDL_Renderer** render, SDL_Texture* big_texture);

void updateUtilsTexture(SDL_Renderer** renderer, SDL_Texture** target, SDL_Window* window, SDL_Texture* tileset, camera_t* camera, map_t* map);

void renderUtils(SDL_Renderer** renderer, SDL_Texture* big_texture);

void renderPlayer(SDL_Renderer** renderer, camera_t* camera, player_t* player);

#endif