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

item_t axe = {
    .itemType = WEAPON,
    .name = "Hache",
    .texture = NULL,
    .Item = &(weapon_t){ .damage = 5 }
};

item_t apple = {
    .itemType = FOOD,
    .name = "Pomme",
    .texture = NULL,
    .Item = &(food_t){ .feed = 15, .thirst = 5 }
};

// printf("Le champ damage de l'objet weapon est %d\n", ((weapon*)axe.Item)->damage);

T_MENU menu = MAIN;

SDL_bool tabEvent[20] = {SDL_FALSE};

case_inventory mat_inventory[3][10];

/**
 * @brief Nous renvoi le menu dans lequel le client se trouve.
 * @return T_MENU
 */
T_MENU getMenu()
{
    return (menu);
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