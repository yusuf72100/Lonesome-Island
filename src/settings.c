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
 * Met à jour le fichier de paramètres.
 */
void saveFile()
{
    FILE *config = fopen("settings.config", "w+");

    fprintf(config, "[ KEYBINDS ]:\n");
    for(int i = 0; i < 14; i++)
    {
        fprintf(config, "%d : %d\n", i, SDL_GetScancodeFromKey(globalKeyTab[i]));
    }
    fprintf(config, "[ \\KEYBINDS ]\n\n");
    fprintf(config, "[ PLAYERNAME ]:\n");
    fprintf(config, "%s\n", joueur.playername);
    fprintf(config, "[ \\PLAYERNAME ]\n");

    fclose(config);
    loadSettings();
}

/**
 * @brief Créer le fichier de config et charge les paramètres par défaut.
 *
 */
static void createConfigs()
{
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

    KEYBOARD_WAITING = GETPLAYERNAME_CLICKED;
    changeMenu(GET_PLAYERNAME_MENU);
}

/**
 * Enregistre une touche.
 * @param key
 * @param keycode
 */
static void saveKeybind(int key, SDL_KeyCode keycode)
{
    localKeyTab[key] = keycode;
}

/**
 * @brief Fonction qui charge les keybinds dans le tableau global.
 *
 */
static void loadKeybinds()
{
    globalKeyTab[0] = localKeyTab[0];       //touche aller en haut
    globalKeyTab[1] = localKeyTab[1];       //touche aller en bas
    globalKeyTab[2] = localKeyTab[2];       //touche aller à gauche
    globalKeyTab[3] = localKeyTab[3];       //touche aller à droite
    globalKeyTab[4] = SDLK_LEFT;
    globalKeyTab[5] = SDLK_RIGHT;
    globalKeyTab[6] = SDLK_SPACE;
    globalKeyTab[7] = SDL_BUTTON_LEFT;
    globalKeyTab[8] = SDLK_LALT;
    globalKeyTab[9] = SDL_BUTTON_MIDDLE;
    globalKeyTab[10] = SDLK_ESCAPE;
    globalKeyTab[11] = SDLK_LALT;
    globalKeyTab[12] = SDLK_RETURN;
    globalKeyTab[13] = localKeyTab[13];     //touche TAB
    globalKeyTab[14] = SDLK_DELETE;

    bindButtonText[0] = (char) globalKeyTab[0];
    bindButtonText[1] = (char) globalKeyTab[1];
    bindButtonText[2] = (char) globalKeyTab[2];
    bindButtonText[3] = (char) globalKeyTab[3];
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
                else if(strcmp(setting, "PLAYERNAME") == 0){
                    fscanf(config, "%s", &joueur.playername);
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
        saveFile();
    }

    fclose(config);
}

/**
 * @brief Gère l'enregistrement des touches..
 * @param keycode
 */
extern void saveKey(SDL_KeyCode keycode)
{
    switch (KEYBOARD_WAITING) {
        case SETTINGS_KEYBIND_UP_CLICKED:
            saveKeybind(0, keycode);
            break;
        case SETTINGS_KEYBIND_LEFT_CLICKED:
            saveKeybind(1, keycode);
            break;
        case SETTINGS_KEYBIND_DOWN_CLICKED:
            saveKeybind(2, keycode);
            break;
        case SETTINGS_KEYBIND_RIGHT_CLICKED:
            saveKeybind(3, keycode);
            break;
    }
    loadKeybinds();
    saveFile();
}

/**
 * @brief Ajoute un caractère au pseudo du joueur.
 * @param c
 */
extern void addCharToPlayerName(char c)
{
    if(strcmp(joueur.playername, "Username") == 0 || strcmp(joueur.playername, " ") == 0)
    {
        joueur.playername[0] = c;
        joueur.playername[1] = '\0';
    }
    else{
        int taille = strlen(joueur.playername);
        joueur.playername[taille] = c;
        joueur.playername[taille + 1] = '\0';
    }
}

/**
 * @brief Enlève un caractère du pseudo du joueur.
 */
extern void removeCharToPlayerName()
{
    int taille = strlen(joueur.playername);
    if(strcmp(joueur.playername, "Username") == 0 || taille == 1)
    {
        joueur.playername[0] = ' ';
        joueur.playername[1] = '\0';
    }
    else{
        joueur.playername[taille-1] = '\0';
    }
}