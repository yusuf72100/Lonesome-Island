/**
 * @file settings.h
 * @author Yusuf Ulas
 * @brief Fichier content les définitions pour le fichier settings.c
 * @version 0.1
 * @date 2023-02-28
 * @copyright Copyright (c) 2023
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <windows.h>
#include <SDL.h>  
#include "defs.h"

static SDL_KeyCode localKeyTab[14];

extern void loadSettings();
extern void saveKey(SDL_KeyCode keycode);

#endif