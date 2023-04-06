/**
 * @file main.h
 * @author Yusuf Ulas
 * @brief Fichier contenant toutes les variables d'affichage
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "liste.h"
#include "client.h"
#include "sound.h"
#include "settings.h"
#include "defs.h"
#include <windows.h>

static Liste *l;

static short loading;
static short debug = FALSE; 

static int tick=0;
static int tabTick[20] = {0};
static int rotation = 0;

static char *s;

static Vecteur vecteur;
static Bullet *bullet = NULL;

static SDL_bool program_launched = SDL_TRUE;
static SDL_Event event; 

static Uint32 animationDelay;

static pthread_t reloading;
static pthread_t server;
static pthread_t sendtoserver;
static pthread_t receivefromserver;
static pthread_t animations_thread;

static void checkEvents();
static void doEvents();
void *Send2Server();
void delay_settings_button_right();
void delay_settings_button_left();
void delay_running_down();
void delay_running_up();
void delay_running_right();
void delay_running_left();
void delay_breath();
static void *rechargement();
void *synchDatas(player_t * Joueurs, int taille);
char* newLogName();
char* eventTime();

#endif