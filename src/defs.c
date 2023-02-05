/**
 * @file defs.h
 * @author Yusuf Ulas
 * @brief Fichier contenant toutes les définitions de structures et de types
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "defs.h"

item bois = {
    .damage = 0, 
    .eatable = 0,
    .isTool = 0,
    .isWeapon = 0,
    .stack = 64,
    .texture = NULL
};

SDL_bool tabEvent[20] = {SDL_FALSE};

case_inventory mat_inventory[3][10];