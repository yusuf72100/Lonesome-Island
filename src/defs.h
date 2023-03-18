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

typedef enum {PLAY_BUTTON_HOVER, CONNECT_BUTTON_HOVER, HOST_BUTTON_HOVER, SETTINGS_BUTTON_HOVER, SETTINGS_KEYBIND_HOVER, INVENTORY_BUTTON_HOVER} T_BUTTONS_HOVER;
typedef enum {PLAY_BUTTON_CLICKED, CONNECT_BUTTON_CLICKED, HOST_BUTTON_CLICKED, SETTINGS_BUTTON_CLICKED, SETTINGS_KEYBIND_CLICKED, INVENTORY_BUTTON_CLICKED} T_BUTTONS_CLICKED;
typedef enum {MAIN_MENU, INGAME_MENU, INVENTORY_MENU, SETTINGS_MAIN_MENU, SETTINGS_MAIN_KEYBIND_MENU, SETTINGS_INGAME_MENU, ERR_MENU} T_MENU;
typedef enum {WEAPON, FOOD, TOOL, BLOCK_ITEM} T_ITEM;
typedef enum {CHEST, RESOURCE, BLOCK} T_BLOCK;

extern int SOLO;
extern int HOST;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern char IP_PUBLIC[256];

/**
 * @brief Sauvegarde les informations de chaque joueurs relatives uniquement au jeu en lui-même (points de vie, coordonnées, animation...).
 * 
 */
typedef struct
{
    SDL_Rect playerRect;
    int animation_state;
    int connected;
    int health;
    int thirst;
    int stuck;
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
    T_ITEM itemType;
}item_t;

/**
 * @brief Stockera les informations de chaque case de l'inventaire du joueur.
 * 
 */
typedef struct
{
    int number;
    item_t *Item;
    SDL_Texture *text_texture;
    SDL_Surface *text_surface;
    SDL_Rect text_rectangle;
    SDL_Rect item_rectangle;
}case_inventory;

/**
 * @brief Définition d'un chest.
 * 
 */
typedef struct 
{
    case_inventory chest_inventory[3][10];
}chest_t;

/**
 * @brief Définition d'un block.
 * 
 */
typedef struct 
{
    void *Block;
    char name[20];
    SDL_Texture *texture;
    SDL_Surface *surface;   
    SDL_Rect rect;
    T_BLOCK blockType;
}block_t;

extern T_MENU menu;
extern item_t bois;
extern SDL_bool tabEvent[20];
extern SDL_KeyCode globalKeyTab[20];
extern case_inventory mat_inventory[3][10];
extern case_inventory *wearedItem;
extern void defineItem();
extern item_t *axe;
extern item_t *apple;
extern item_t *chest_item;
extern block_t *chest_block;
extern player_t joueur;
extern player_t * joueurs;

#endif