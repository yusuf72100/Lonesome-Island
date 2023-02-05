/**
 * @file defs.h
 * @author Yusuf Ulas
 * @brief Fichier contenant toutes les définitions de structures et de types
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */
#ifndef DEFS_H
#define DEFS_H

#include <SDL.h>

/**
 * @brief Sauvegarde les informations de chaque joueurs relatives uniquement au jeu en lui-même (points de vie, coordonnées, animation...).
 * 
 */
typedef struct
{
    SDL_Rect playerRect;
    int animation_state;
    int connected;

}player;

/**
 * @brief Item avec ses stats.
 * 
 */
typedef struct 
{
    int stack;
    int eatable;
    int isTool;
    int isWeapon;
    int damage;
    SDL_Texture *texture;
}item;

/**
 * @brief Stockera les informations de chaque case de l'inventaire du joueur.
 * 
 */
typedef struct
{
    item Item;
    int number;
}case_inventory;

extern item bois;

extern SDL_bool tabEvent[20];

extern case_inventory mat_inventory[3][10];

#endif