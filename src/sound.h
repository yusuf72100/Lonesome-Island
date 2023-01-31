/**
 * @file sound.h
 * @author Yusuf Ulas
 * @brief Fichier contenant les sons
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SDL.h>  
#include <SDL_mixer.h>
#include <windows.h>
#include <pthread.h>

static Mix_Chunk* boop = NULL;
static Mix_Chunk* hover = NULL;

static pthread_t hover_Thread;
static pthread_t boop_Thread;

void initAudio();

void creation_canaux_musique();

void *play_hover_audio(void *key);

void *play_boop(void *key);

void init_boop(SDL_bool *key);

void init_hover(SDL_bool *key);

void detruire_musique(Mix_Chunk* chunk);