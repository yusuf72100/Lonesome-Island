/**
 * @file defs.c
 * @author Yusuf Ulas
 * @brief Fichier qui initialisera les variables globales.
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "defs.h"

int SOLO = FALSE;

item_t *axe;
item_t *apple;
item_t *chest_item;
block_t *chest_block;
player_t joueur;
player_t * joueurs;
T_MENU menu = MAIN_MENU;
SDL_bool tabEvent[20] = {SDL_FALSE};
SDL_KeyCode globalKeyTab[20];
case_inventory mat_inventory[3][10];
case_inventory *wearedItem;

/**
 * @brief Initialise les items globaux.
 * 
 */
void defineItem()
{
    joueurs = malloc(sizeof(player_t) * 10);
    joueurs->health = 100;
    joueurs->stuck = FALSE;
    joueur.health = 100;
    wearedItem = malloc(sizeof(case_inventory));
    axe = malloc(sizeof(item_t));
    apple = malloc(sizeof(item_t));
    chest_item = malloc(sizeof(item_t));
    chest_block = malloc(sizeof(block_t));

    chest_block->blockType = CHEST;
    strcpy(chest_block->name, "Coffre");
    chest_block->texture = NULL;
    chest_block->surface = NULL;
    chest_block->Block = &(chest_t){};

    axe->itemType = WEAPON;
    strcpy(axe->name, "Hache");
    axe->texture = NULL;
    axe->surface = NULL;
    axe->Item = &(weapon_t){ .damage = 5 };

    apple->itemType = FOOD;
    strcpy(apple->name, "Pomme");
    apple->texture = NULL;
    apple->surface = NULL;
    apple->Item = &(food_t){ .feed = 15, .thirst = 5 };

    chest_item->itemType = BLOCK_ITEM;
    strcpy(chest_item->name, "Coffre");
    chest_item->texture = NULL;
    chest_item->surface = NULL;
    chest_item->Item = &(block_t){ .Block = chest_block };
}

/**
 * @brief Change de menu.
 * 
 * @param menuTarget 
 */
void changeMenu(T_MENU menuTarget)
{
    if(menuTarget == MAIN_MENU) SOLO = FALSE;
    menu = menuTarget;
}