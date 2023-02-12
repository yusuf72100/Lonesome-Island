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

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#define TRUE 1
#define FALSE 0

typedef enum {PLAY_BUTTON, CONNECT_BUTTON, HOST_BUTTON, SETTINGS_BUTTON} T_BUTTONS;
typedef enum {MAIN_MENU, INGAME_MENU, INVENTORY_MENU, SETTINGS_MENU, ERR_MENU} T_MENU;
typedef enum {WEAPON, FOOD, TOOL} T_ITEM;

extern int SOLO;

/**
 * @brief Sauvegarde les informations de chaque joueurs relatives uniquement au jeu en lui-même (points de vie, coordonnées, animation...).
 * 
 */
typedef struct
{
    SDL_Rect playerRect;
    int animation_state;
    int connected;
}player_t;

/**
 * @brief Définition d'un weapon.
 * 
 */
typedef struct
{
    int damage; 
}weapon_t;

/**
 * @brief Définition d'un food.
 * 
 */
typedef struct 
{
    int feed;
    int thirst;
}food_t;

/**
 * @brief Item avec ses stats.
 * 
 */
typedef struct 
{
    void *Item;
    char name[20];
    SDL_Texture *texture;
    SDL_Surface *surface;
    SDL_Rect rectangle;
    T_ITEM itemType;
}item_t;

/**
 * @brief Stockera les informations de chaque case de l'inventaire du joueur.
 * 
 */
typedef struct
{
    item_t *Item;
    int number;
    SDL_Texture *text_texture;
    SDL_Surface *text_surface;
    SDL_Rect text_rectangle;
}case_inventory;

extern T_MENU menu;

extern item_t bois;

extern SDL_bool tabEvent[20];

extern case_inventory mat_inventory[3][10];

extern void changeMenu(T_MENU menuTarget);

extern T_MENU getMenu();

extern void defineItem();

extern item_t *axe;

extern item_t *apple;

#endif