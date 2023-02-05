/**
 * @file defs.h
 * @author Yusuf Ulas
 * @brief Fichier contenant toutes les définitions de structures et de types
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "defs.h"

item axe = {
    .itemType = WEAPON,
    .name = "Hache",
    .texture = NULL,
    .Item = &(weapon){ .damage = 5 }
};

// printf("Le champ damage de l'objet weapon est %d\n", ((weapon*)axe.Item)->damage);

T_MENU menu = MAIN;

SDL_bool tabEvent[20] = {SDL_FALSE};

case_inventory mat_inventory[3][10];

/**
 * @brief Nous renvoi le menu dans lequel le client se trouve.
 * @return char*
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