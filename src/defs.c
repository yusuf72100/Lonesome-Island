/**
 * @file defs.c
 * @author Yusuf Ulas
 * @brief Fichier qui initialisera les variables globales.
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "defs.h"

item_t *axe;
item_t *apple;

int SOLO = FALSE;

T_MENU menu = MAIN_MENU;

SDL_bool tabEvent[20] = {SDL_FALSE};

case_inventory mat_inventory[3][10];

case_inventory *wearedItem;

/**
 * @brief Initialise les items globaux.
 * 
 */
void defineItem()
{
    wearedItem = malloc(sizeof(case_inventory));
    axe = malloc(sizeof(item_t));
    apple = malloc(sizeof(item_t));

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
}

/**
 * @brief Change de menu.
 * 
 * @param menuTarget 
 */
void changeMenu(T_MENU menuTarget)
{
    menu = menuTarget;
}