#include "socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <pthread.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init.h"
#include "liste.h"
#include "client.h"

void SDL_ExitWithError(const char *message);

void *Send2Server();

Vecteur InitVecteur(int angle, int vitesse);

void dessinerJoueur(playersRect *p_datas ,int rotation);

void destroyAll(SDL_Window *window, SDL_Renderer *renderer);

void *rechargement();

void UpdateBullet(Bullet * b);

void UpdateBulletAll();

char* newLogName();

char* eventTime();

//méthode pour dessiner plus facilement les rectangles (objets)
void dessinerRect(SDL_Rect rectangle, SDL_Renderer *renderer);

void dessinerButton(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window);

void dessinerTank(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, int rotation);

void dessinerBalle(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, Bullet *b, int rotation, int vitesse);

void initBullet(Bullet * b, int x, int y, int rotation);

void buttonHoverPlay(SDL_Window *window, SDL_Texture *texture_play_hover, SDL_Renderer *renderer, SDL_Rect play_button_rect);

void buttonHoverHost(SDL_Window *window, SDL_Texture *texture_host_hover, SDL_Renderer *renderer, SDL_Rect host_button_rect);

void SDL_ExitWithError(const char *message);

//gcc src/main.c -o bin/prog src/liste.o -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread