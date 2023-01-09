#ifndef INIT_H
#define INIT_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "liste.h"

/* CE FICHIER CONTIENDRA TOUTES LES DECLARATIONS DE TOUTES LES VARAIBLES DU PROGRAMME */

void initialiser_surfaces();
void init_assets();
void SDL_ExitWithError(const char *message);
void destroyAll(SDL_Window *window, SDL_Renderer *renderer);

#endif