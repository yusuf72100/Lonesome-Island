/**
 * @file defs.c
 * @author Yusuf Ulas
 * @brief Fichier qui initialisera les variables globales.
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "defs.h"

int CONNECTED = FALSE;
int SOLO = FALSE;
int HOST = FALSE;
int connectedError = FALSE;
int size = 0;
char IP_PUBLIC[] = "192.168.1.16";
char bindButtonText[4] = {""};
int direction = -1;

SDL_Surface* tmp = NULL;
SDL_Texture* tileset = NULL;
SDL_Texture* currentGround = NULL;
SDL_Texture* currentUtils = NULL;

camera_t *camera = NULL;
map_t *map = NULL;

player_t joueur;
player_t *joueurs;

T_BUTTONS_CLICKED KEYBOARD_WAITING = NONE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

item_t *axe;
item_t *apple;
item_t *chest_item;
block_t *chest_block;
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
    camera = malloc((sizeof(camera_t)));
    map = malloc(sizeof(map_t));
    joueurs = malloc(sizeof(player_t) * 10);
    /**joueurs[0}->health = 100;
    *joueurs->stuck = FALSE;*/
    joueur.health = 100;
    strcat(joueur.playername, "Username");

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