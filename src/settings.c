/**
 * @file settings.c
 * @author Yusuf Ulas   
 * @brief Programme qui s'occupera de la sauvegarde des paramètres de l'utilisateur.
 * @version 0.1
 * @date 2023-02-28
 * @copyright Copyright (c) 2023
 */

#include "settings.h"

/**
 * @brief Créer le fichier de config et charge les paramètres par défaut.
 * 
 */
static void createConfigs()
{
    FILE *config = fopen("settings.config", "w");

    globalKeyTab[0] = SDLK_z;
    globalKeyTab[1] = SDLK_q;
    globalKeyTab[2] = SDLK_s;
    globalKeyTab[3] = SDLK_d;
    globalKeyTab[4] = SDLK_LEFT;
    globalKeyTab[5] = SDLK_RIGHT;
    globalKeyTab[6] = SDLK_SPACE;
    globalKeyTab[7] = SDL_BUTTON_LEFT;
    globalKeyTab[8] = SDLK_LALT;
    globalKeyTab[9] = SDL_BUTTON_MIDDLE;
    globalKeyTab[10] = SDLK_ESCAPE;
    globalKeyTab[11] = SDLK_LALT;
    globalKeyTab[12] = SDLK_RETURN;
    globalKeyTab[13] = SDLK_TAB;

    fprintf(config, "[ KEYBINDS ]:\n");

    for(int i = 0; i < 14; i++)
    {
        fprintf(config, "%d : %d\n", i, SDL_GetScancodeFromKey(globalKeyTab[i]));
    }
    fprintf(config, "[ \\KEYBINDS ]\n");

    fclose(config);
}

/**
 * @brief Fonction qui charge les keybinds dans le tableau global.
 * 
 */
static void loadKeybinds()
{
    globalKeyTab[0] = localKeyTab[0];
    globalKeyTab[1] = localKeyTab[1];
    globalKeyTab[2] = localKeyTab[2];
    globalKeyTab[3] = localKeyTab[3];
    globalKeyTab[4] = SDLK_LEFT;
    globalKeyTab[5] = SDLK_RIGHT;
    globalKeyTab[6] = SDLK_SPACE;
    globalKeyTab[7] = SDL_BUTTON_LEFT;
    globalKeyTab[8] = SDLK_LALT;
    globalKeyTab[9] = SDL_BUTTON_MIDDLE;
    globalKeyTab[10] = SDLK_ESCAPE;
    globalKeyTab[11] = SDLK_LALT;
    globalKeyTab[12] = SDLK_RETURN;
    globalKeyTab[13] = localKeyTab[13];
}

/**
 * @brief Convertis un entier en sdl keycode et le met dans le tableau des keycode local.
 * 
 * @param key 
 * @param scancode 
 */
static void toKeyCode(int key, int scancode)
{
    localKeyTab[key] = SDL_GetKeyFromScancode(scancode);
}

/**
 * @brief Fonction qui s'occupe de charger les paramètres du joueur depuis un fichier.
 * 
 */
extern void loadSettings()
{
    FILE *config = fopen("settings.config", "r");
    char setting[100];
    char trash[100];
    int key;
    int value;

    if(config != NULL)
    {
        while(!feof(config))
        {
            if(fscanf(config, "[ %s ]:\n", setting))
            {
                if(strcmp(setting, "KEYBINDS") == 0)
                {
                    while(!fscanf(config, "[ \\%s ]\n", setting))
                    {
                        fscanf(config, "%d : %d", &key, &value);
                        toKeyCode(key, value);
                        key++;
                    }
                }
                loadKeybinds();
            }
            else{
                fscanf(config, "%s", trash);
            }
        }
    }
    else{
        printf("Creation of config file\n");
        createConfigs();
        loadSettings();
    }

    fclose(config);
}