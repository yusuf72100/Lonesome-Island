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

void defineItem()
{
    axe = malloc(sizeof(item_t));
    apple = malloc(sizeof(item_t));
    axe->itemType = WEAPON;
    strcpy(axe->name, "Hache");
    axe->texture = NULL;
    axe->surface = NULL;
    axe->rectangle.h = 80;
    axe->rectangle.w = 80;
    axe->Item = &(weapon_t){ .damage = 5 };

    apple->itemType = FOOD;
    strcpy(apple->name, "Pomme");
    apple->texture = NULL;
    apple->surface = NULL;
    apple->rectangle.h = 80;
    apple->rectangle.w = 80;
    apple->Item = &(food_t){ .feed = 15, .thirst = 5 };
}

T_MENU menu = MAIN;

SDL_bool tabEvent[20] = {SDL_FALSE};

case_inventory mat_inventory[3][10];

void inventoryInit()
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mat_inventory[i][j].Item = apple;
            mat_inventory[i][j].number = 0;
        }
    }
    mat_inventory[0][0].Item = apple;
    mat_inventory[0][0].number = 1;
}

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