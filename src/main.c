/**
 * @file main.c
 * @author Yusuf Ulas
 * @brief Fichier principal du programme qui s'occupera de la partie jeu
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "menus.h"
#include "main.h"

/**
 * @brief Tente de placer un item dans l'inventaire et renvoi 0 si l'action a réussit.
 * @param item
 * @param number
 * @return
 */
int putInInventory(item_t item, int number)
{
    int i, j;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 10; j++)
        {
            if(mat_inventory[i][j].number == 0)
            {
                mat_inventory[i][j].number = number;
                *mat_inventory[i][j].Item = item;
                return FALSE;
            }
            else if(mat_inventory[i][j].Item->itemType == item.itemType && mat_inventory[i][j].number < 64)
            {
                if(mat_inventory[i][j].number + number <= 64)
                {
                    mat_inventory[i][j].number += number;
                    return FALSE;
                }
                else{
                    int newNumber = number - (64 - mat_inventory[i][j].number);
                    mat_inventory[i][j].number = 64;
                    return (putInInventory(item, newNumber));
                }
            }
        }
    }
    return TRUE;
}

/**
 * @brief Récupère les données depuis le socket client.
 * 
 * @param Joueurs 
 * @param taille 
 */
void *synchDatas(player_t * Joueurs, int taille)
{
    size = taille;
    joueurs = Joueurs;
}

/**
 * @brief Envoi les données du joueur actuel au serveur.
 * 
 * @return void* 
 */
void *Send2Server()
{
    sendDatas(joueur);
}

/**
 * @brief Renvoi vrai si le keycode passé en paramètre correspond au keycode d'une touche alpha (une lettre).
 * @param code
 * @return int
 */
static int isKeyLetter(SDL_Keycode code)
{
    return (strlen(SDL_GetKeyName(code)) == 1 && isalpha(SDL_GetKeyName(code)[0]));
}

/**
 * @brief Renvoi vrai si le keycode passé en paramètre correspond au keycode d'une touche digit (un chiffre).
 * @param code
 * @return int
 */
static int isKeyNumber(SDL_Keycode code)
{
    return (strlen(SDL_GetKeyName(code)) == 1 && SDL_GetKeyName(code)[0] >= '0' && SDL_GetKeyName(code)[0] <= '9');
}

/**
 * @brief Renvoi vrai si la touche pressée est une majuscule.
 * @param code
 * @return int
 */
static int isKeyMaj(SDL_Keycode code)
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Vérifie si la touche Caps Lock est enfoncée
    if (SDL_GetModState() & KMOD_CAPS)
        return 1;

    // Vérifie si les touches Shift gauche ou droit sont enfoncées
    if (SDL_GetModState() & KMOD_SHIFT)
        return 1;

    // Vérifie si la touche est une lettre majuscule
    if (code >= SDLK_a && code <= SDLK_z)
        return (SDL_GetModState() & KMOD_SHIFT);

    return 0;
}

/**
 * @brief Met un cooldown pour un rechargement. (s'execute uniquement dans un thread)
 * 
 * @return void* 
 */
static void *rechargement()
{
    Sleep(5000);
    loading = 0;
}

/**
 * @brief Initialise un fichier logs.
 * 
 * @return char* 
 */
char* newLogName()
{
    struct tm * timeinfo;
    time_t now;

    time (&now);
    timeinfo = localtime ( &now );
    char *logName = malloc(sizeof(char)*100+1);

    //strcpy("logs/", logName);
    strftime(logName,30,"%b.%d.%Y.%H.%M.%S",timeinfo);
    strcat(logName, ".txt");
    logName = realloc(logName, sizeof(char) * strlen(logName)+1);

    return logName;
}

/**
 * @brief Récupère l'heure et la date actuelle.
 * 
 * @return char* 
 */
char* eventTime()
{
    char *eventTime = malloc(sizeof(char)*100); 
    struct tm * timeinfo;
    time_t now;

    time (&now);
    timeinfo = localtime ( &now );

    strftime(eventTime,30,"%H.%M.%S",timeinfo);
    eventTime = realloc(eventTime, sizeof(char) * strlen(eventTime)+1);

    return eventTime;
}

/**
 * @brief Change les valeurs du tableau d'évènements en fonction de nos instructions sur le clavier et la souris.
 * 
 */
static void checkEvents()
{
    switch(event.type)
    {
        //clavier
        case SDL_KEYDOWN:
            //si on attend que l'utilisateur écrive quelque-chose au clavier
            if(KEYBOARD_WAITING != NONE)
            {
                //si il s'agit d'une configuration de touches de jeu
                if(menu == SETTINGS_MAIN_KEYBIND_MENU || menu == SETTINGS_INGAME_MENU)
                {
                    saveKey(event.key.keysym.sym);
                    KEYBOARD_WAITING = NONE;
                }
                //si il s'agit de la configuration du pseudo du joueur
                else if(menu == GET_PLAYERNAME_MENU){
                    //on gère les touches d'effacement, d'insertion...
                    switch (event.key.keysym.sym) {
                        case SDLK_RETURN:
                            //touche ENTRER
                            if(strcmp(joueur.playername, "") || strcmp(joueur.playername, "\0"))
                            {
                                KEYBOARD_WAITING = NONE;
                                saveFile();
                                changeMenu(MAIN_MENU);
                            }
                            break;
                        case SDLK_BACKSPACE:
                            removeCharToPlayerName();
                            break;
                        default:
                            if ((strlen)(joueur.playername) != 16) {
                                if (isKeyLetter(event.key.keysym.sym)) {
                                    if (isKeyMaj(event.key.keysym.sym)) {
                                        // Si la touche est une lettre majuscule
                                        addCharToPlayerName((char)toupper(SDL_GetKeyName(event.key.keysym.sym)[0]));
                                    } else {
                                        // Sinon, c'est une lettre minuscule
                                        addCharToPlayerName((char)event.key.keysym.sym);
                                    }
                                } else if (isKeyNumber(event.key.keysym.sym)) {
                                    // Si la touche est un chiffre
                                    addCharToPlayerName((char)event.key.keysym.sym);
                                }
                            }
                            break;
                    }
                }
            }

            else{
                if (event.key.keysym.sym == globalKeyTab[0]) tabEvent[0] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[1]) tabEvent[1] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[2]) tabEvent[2] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[3]) tabEvent[3] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[4]) tabEvent[4] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[5]) tabEvent[5] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[6]) tabEvent[6] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[10]) tabEvent[10] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[11]) tabEvent[11] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[12]) tabEvent[12] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[13]) tabEvent[13] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[14]) tabEvent[14] = SDL_TRUE;

                if (event.key.keysym.sym == globalKeyTab[15]) tabEvent[15] = SDL_TRUE;
            }
        break;

        case SDL_KEYUP:
            //si il s'agit d'une configuration de touche...
            if(KEYBOARD_WAITING != NONE)
            {

            }
            else{
                joueur.isRunning = 0;
                
                if (event.key.keysym.sym == globalKeyTab[0]) tabEvent[0] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[1]) tabEvent[1] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[2]) tabEvent[2] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[3]) tabEvent[3] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[4]) tabEvent[4] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[5]) tabEvent[5] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[6]) tabEvent[6] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[10]) tabEvent[10] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[11]) tabEvent[11] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[12]) tabEvent[12] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[13]) tabEvent[13] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[14]) tabEvent[14] = SDL_FALSE;

                if (event.key.keysym.sym == globalKeyTab[15]) tabEvent[15] = SDL_FALSE;
            }
        break;

        //souris
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == globalKeyTab[7]) tabEvent[7] = SDL_TRUE;

            if (event.button.button == globalKeyTab[8]) tabEvent[8] = SDL_TRUE;

            if (event.button.button == globalKeyTab[9]) tabEvent[9] = SDL_TRUE;
        break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == globalKeyTab[7]) tabEvent[7] = SDL_FALSE;

            if (event.button.button == globalKeyTab[8]) tabEvent[8] = SDL_FALSE;

            if (event.button.button == globalKeyTab[9]) tabEvent[9] = SDL_FALSE;
        break;

        case SDL_QUIT:
            program_launched = SDL_FALSE;
            break;

    }
}

/**
 * @brief Délai d'animation de respiration 
 * 
 */
void delay_breath()
{
    int milli_seconds = 1000;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && !tabEvent[0] && !tabEvent[1] && !tabEvent[2] && !tabEvent[3] && !tabEvent[6] && !tabEvent[7]);
}

/**
 * @brief Délai de l'animation "courrir à gauche"
 * 
 */
void delay_running_left()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[1]);
}

/**
 * @brief Délai de l'animation "courrir à droite"
 * 
 */
void delay_running_right()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[3]);
}

/**
 * @brief Délai de l'animation "courrir en haut"
 * 
 */
void delay_running_up()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[0]);
}

/**
 * @brief Délai de l'animation "courrir en bas"
 * 
 */
void delay_running_down()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[2]);
}

/**
 * @brief Délai de l'animation du bouton de paramètres quand on a pas le curseur dessus 
 * 
 */
void delay_settings_button_left()
{
    int milli_seconds = 50;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && !hover_settingsbutton);
}

/**
 * @brief Délai de l'animation du bouton de paramètres quand on a le curseur dessus 
 * 
 */
void delay_settings_button_right()
{
    int milli_seconds = 50;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

/**
 * @brief Execute les events en fonction du tableau d'évènements
 * 
 */
static void doEvents()
{
    if(!joueur.stuck)
    {
        if(tabEvent[0])
        {
            //touche Z
            if (menu == INGAME_MENU)
            {
                if (debug) printf("Touche avancer pressee | %s\n", eventTime());
                direction = NORTH;
                movePlayer( map, &joueur, NORTH);
                joueur.isRunning = 1;


                if(tabEvent[1] == SDL_FALSE && tabEvent[3] == SDL_FALSE)
                {
                   /* if(pthread_kill(animations_thread, 0) != 0)
                        pthread_create(&animations_thread, NULL, running_up_animation,(void *)&joueur);  */
                    pthread_create(&sendtoserver,NULL,Send2Server,NULL);
                }
            }
        }

        if(tabEvent[1])
        {
            //touche Q
            if(menu == INGAME_MENU)
            {
                if (debug) printf("Touche gauche pressee | %s\n", eventTime());
                direction = WEST;
                movePlayer( map, &joueur, WEST);
                joueur.isRunning = 1;

                /*if(pthread_kill(animations_thread, 0) != 0)
                    pthread_create(&animations_thread, NULL, running_left_animation,(void *)&joueur);  */
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);
            }
        }

        if(tabEvent[2])
        {
            //touche S
            if(menu == INGAME_MENU)
            {
                if (debug) printf("Touche descendre pressee | %s\n", eventTime());
                direction = SOUTH;
                movePlayer( map, &joueur, SOUTH);
                joueur.isRunning = 1;

                if(tabEvent[1] == SDL_FALSE && tabEvent[3] == SDL_FALSE)
                {
                    /*if(pthread_kill(animations_thread, 0) != 0)
                        pthread_create(&animations_thread, NULL, running_down_animation,(void *)&joueur);  */
                    pthread_create(&sendtoserver,NULL,Send2Server,NULL);
                }
            }
        }

        if(tabEvent[3])
        {
            //touche D
            if(menu == INGAME_MENU)
            {
                if (debug) printf("Touche droite pressee | %s\n", eventTime());
                direction = EAST;
                movePlayer( map, &joueur, EAST);
                joueur.isRunning = 1;

               /* if(pthread_kill(animations_thread, 0) != 0)
                    pthread_create(&animations_thread, NULL, running_right_animation,(void *)&joueur);  */
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);
            }
        }

        if(tabEvent[4])
        {
            //touche LEFT
            if(menu == INGAME_MENU)
            {
                //if (debug) printf("Touche SDLK_LEFT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation - 1;
            }
        }

        if(tabEvent[5])
        {
            //touche RIGHT
            if(menu == INGAME_MENU)
            {
                //if (debug) printf("Touche SDLK_RIGHT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation + 1;
            }
        }
        if(tabEvent[6])
        {
            //touche ESPACE
            if(menu == INGAME_MENU)
            {
                if (!loading)
                { 
                    if (debug) printf("SHOT FIRE!!!\n"); 
                    loading = 1;
                    pthread_create(&reloading,NULL,rechargement,NULL);
                }
                else{
                    if (debug) printf("Erreur: reloading!!!\n");
                }
            }
        }
    }
    //mouse events
    if(tabEvent[7])
    {
        // --------------------------------------------- click LEFT DOWN -----------------------------------------------------------------
        
        //play button
        if(onButton(PLAY_BUTTON_HOVER) && menu == MAIN_MENU)
        {
            SOLO = TRUE;
            //create_map();
            build_map(&map);
            initCamera(camera, window, &joueur);
            updateGroundTexture(&renderer, &currentGround, window, tileset, camera, map);
            updateUtilsTexture(&renderer, &currentUtils, window, tileset, camera, map);
            init_boop(&tabEvent[7]);
            if (debug) printf("Host button clicked\n");
            pthread_create(&server,NULL,startServer,NULL);              //on héberge le serveur 
            startConnection();                                          //on créer un client qui se connecte au serveur
            pthread_create(&sendtoserver,NULL,Send2Server,NULL);
            pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
            changeMenu(INGAME_MENU);
            switchButtonState_hover(PLAY_BUTTON_HOVER);
        }

        //Connect button
        if(onButton(CONNECT_BUTTON_HOVER) && menu == MAIN_MENU)
        {
            init_boop(&tabEvent[7]);
            if (debug) printf("Connect button clicked\n");                                         
            if(startConnection() == 0)                                     //on créer un client qui se connecte au serveur 
            {
                //receiveMap(map);
                CONNECTED = TRUE;
                build_map(&map);
                Sleep(1000);
                pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                changeMenu(INGAME_MENU);
            }
            else{
                changeMenu(ERR_MENU);
                connectedError = TRUE;
            }
            switchButtonState_hover(CONNECT_BUTTON_HOVER);
        }

        //host button
        if(onButton(HOST_BUTTON_HOVER) && menu == MAIN_MENU)
        {
            init_boop(&tabEvent[7]);
            if (debug) printf("Host button clicked\n");
            pthread_create(&server,NULL,startServer,NULL);
            HOST = TRUE;
            //create_map();
            startConnection();                                                                          //on créer un client qui se connecte au serveur
            pthread_create(&sendtoserver,NULL,Send2Server,NULL);                    //on envoi les données du client une première fois
            pthread_create(&receivefromserver,NULL,receiveFromServer,NULL);         //on établie la communication au serveur
            changeMenu(INGAME_MENU);
            switchButtonState_hover(HOST_BUTTON_HOVER);
        }
        
        //settings button
        if(onButton(SETTINGS_BUTTON_HOVER) && menu == MAIN_MENU)
        {
            init_boop(&tabEvent[7]);
            switchButtonState_hover(SETTINGS_BUTTON_HOVER);
            changeMenu(SETTINGS_MAIN_MENU);
        }

        //settings keybinds button (main to settings_keybind)
        if(onButton(SETTINGS_KEYBINDS_HOVER) && (menu == SETTINGS_MAIN_MENU || menu == SETTINGS_INGAME_MENU))
        {
            if(getButtonState_clicked(SETTINGS_KEYBINDS_CLICKED) == FALSE && KEYBOARD_WAITING == NONE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    changeButtonState_clicked(SETTINGS_KEYBINDS_CLICKED, TRUE);
                    changeMenu(SETTINGS_MAIN_KEYBIND_MENU);
                    tabTick[7] = SDL_GetTicks();
                }
            }
            else{
                changeButtonState_clicked(SETTINGS_KEYBINDS_CLICKED, FALSE);
            }
        }

        //settings keybinds button (settings_keybind to main)
        if(onButton(SETTINGS_KEYBINDS_HOVER) && menu == SETTINGS_MAIN_KEYBIND_MENU)
        {
            if(getButtonState_clicked(SETTINGS_KEYBINDS_CLICKED) == FALSE && KEYBOARD_WAITING == NONE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    changeButtonState_clicked(SETTINGS_KEYBINDS_CLICKED, TRUE);
                    tabTick[7] = SDL_GetTicks();

                    if(SOLO == TRUE || HOST == TRUE)
                    {
                        changeMenu(SETTINGS_INGAME_MENU);
                    }
                    else{
                        changeMenu(SETTINGS_MAIN_MENU);
                    }
                }
            }
            else{
                changeButtonState_clicked(SETTINGS_KEYBINDS_CLICKED, FALSE);
            }
        }

        // ---------------------------------------------------------KEYBINDS-------------------------------------------------------------

        //UP
        if(onButton(SETTINGS_KEYBIND_UP_HOVER) && (menu == SETTINGS_MAIN_KEYBIND_MENU || menu == SETTINGS_INGAME_MENU))
        {
            if(getButtonState_clicked(SETTINGS_KEYBIND_UP_CLICKED) == FALSE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    changeButtonState_clicked(SETTINGS_KEYBIND_UP_CLICKED, TRUE);
                    tabTick[7] = SDL_GetTicks();
                    KEYBOARD_WAITING = SETTINGS_KEYBIND_UP_CLICKED;
                    changeButtonState_hover(SETTINGS_KEYBIND_UP_CLICKED, FALSE);
                }
            }
            else{
                changeButtonState_clicked(SETTINGS_KEYBIND_UP_CLICKED, FALSE);
            }
        }

        //DOWN
        if(onButton(SETTINGS_KEYBIND_DOWN_HOVER) && (menu == SETTINGS_MAIN_KEYBIND_MENU || menu == SETTINGS_INGAME_MENU))
        {
            if(getButtonState_clicked(SETTINGS_KEYBIND_DOWN_CLICKED) == FALSE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    changeButtonState_clicked(SETTINGS_KEYBIND_DOWN_CLICKED, TRUE);
                    tabTick[7] = SDL_GetTicks();
                    KEYBOARD_WAITING = SETTINGS_KEYBIND_DOWN_CLICKED;
                    changeButtonState_hover(SETTINGS_KEYBIND_DOWN_CLICKED, FALSE);
                }
            }
            else{
                changeButtonState_clicked(SETTINGS_KEYBIND_DOWN_CLICKED, FALSE);
            }
        }

        //LEFT
        if(onButton(SETTINGS_KEYBIND_LEFT_HOVER) && (menu == SETTINGS_MAIN_KEYBIND_MENU || menu == SETTINGS_INGAME_MENU))
        {
            if(getButtonState_clicked(SETTINGS_KEYBIND_LEFT_CLICKED) == FALSE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    changeButtonState_clicked(SETTINGS_KEYBIND_LEFT_CLICKED, TRUE);
                    tabTick[7] = SDL_GetTicks();
                    KEYBOARD_WAITING = SETTINGS_KEYBIND_LEFT_CLICKED;
                    changeButtonState_hover(SETTINGS_KEYBIND_LEFT_CLICKED, FALSE);
                }
            }
            else{
                changeButtonState_clicked(SETTINGS_KEYBIND_LEFT_CLICKED, FALSE);
            }
        }

        //RIGHT
        if(onButton(SETTINGS_KEYBIND_RIGHT_HOVER) && (menu == SETTINGS_MAIN_KEYBIND_MENU || menu == SETTINGS_INGAME_MENU))
        {
            if(getButtonState_clicked(SETTINGS_KEYBIND_RIGHT_CLICKED) == FALSE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    changeButtonState_clicked(SETTINGS_KEYBIND_RIGHT_CLICKED, TRUE);
                    tabTick[7] = SDL_GetTicks();
                    KEYBOARD_WAITING = SETTINGS_KEYBIND_RIGHT_CLICKED;
                    changeButtonState_hover(SETTINGS_KEYBIND_RIGHT_CLICKED, FALSE);
                }
            }
            else{
                changeButtonState_clicked(SETTINGS_KEYBIND_RIGHT_CLICKED, FALSE);
            }
        }

        //inventory item click
        if(getButtonState_clicked(INVENTORY_BUTTON_CLICKED) == FALSE && menu == INVENTORY_MENU)
        {
            changeButtonState_clicked(INVENTORY_BUTTON_CLICKED, TRUE);
            tabTick[7] = SDL_GetTicks();
            clickItem();
            init_inventory_click();
        }
    }
    if(!tabEvent[7])
    {
        if(menu == INVENTORY_MENU)
        {
            changeButtonState_clicked(INVENTORY_BUTTON_CLICKED, FALSE);
        }
        else if (menu == SETTINGS_MAIN_MENU){
            changeButtonState_clicked(SETTINGS_BUTTON_CLICKED, FALSE);
        }
    }
    if(tabEvent[8])
    {
        //click RIGHT
    }
    if(tabEvent[9])
    {
        //click MIDDLE
    }
    if(!tabEvent[0] && !tabEvent[1] && !tabEvent[2] && !tabEvent[3] && !tabEvent[6] && !tabEvent[7])
    {
        if((menu == INGAME_MENU || menu == INVENTORY_MENU))
        {
            if(pthread_kill(animations_thread, 0) != 0)
                pthread_create(&animations_thread, NULL, breathAnimation,(void *)&joueur);  
        }
    }

    if(tabEvent[10])
    {
        //touche ESCAPE
        if(menu == ERR_MENU)
        {
            if((SDL_GetTicks() - tabTick[10]) >= 200)
            {
                changeMenu(MAIN_MENU);
                tabTick[10] = SDL_GetTicks();
            }
        }
        else if(menu == INGAME_MENU){
            if((SDL_GetTicks() - tabTick[10]) >= 200)
            {
                changeMenu(SETTINGS_INGAME_MENU);
                tabTick[10] = SDL_GetTicks();
            }
        }
        else if(menu == SETTINGS_MAIN_MENU)
        {
            if(SOLO == TRUE || HOST == TRUE)
            {
                changeMenu(INGAME_MENU);
            }
            else{
                changeMenu(MAIN_MENU);
            }
            changeButtonState_hover(SETTINGS_KEYBINDS_HOVER, FALSE);
            tabTick[10] = SDL_GetTicks();
        }
        else if(menu == SETTINGS_INGAME_MENU){
            if((SDL_GetTicks() - tabTick[10]) >= 200)
            {
                changeMenu(INGAME_MENU);
                changeButtonState_hover(SETTINGS_KEYBINDS_HOVER, FALSE);
                tabTick[10] = SDL_GetTicks();
            }
        }
    }

    if(tabEvent[11] && tabEvent[12])
    {
        toggleFullscreen();
    }

    if(tabEvent[13])
    {
        //touche TAB
        if(menu == INGAME_MENU)
        {
            if((SDL_GetTicks() - tabTick[13]) >= 200)
            {
                changeMenu(INVENTORY_MENU);
                tabTick[13] = SDL_GetTicks();
            }
        }
        else if(menu == INVENTORY_MENU){
            if((SDL_GetTicks() - tabTick[13]) >= 200)
            {
                changeMenu(INGAME_MENU);
                tabTick[13] = SDL_GetTicks();
            }
        }
    }

    if(tabEvent[15])
    {
        //touche E
        if(menu == INGAME_MENU)
        {
            if((SDL_GetTicks() - tabTick[13]) >= 200)
            {
                int places[4] = {map->utils[joueur.mapPosition.x][joueur.mapPosition.y+1], map->utils[joueur.mapPosition.x+1][joueur.mapPosition.y], map->utils[joueur.mapPosition.x][joueur.mapPosition.y-1], map->utils[joueur.mapPosition.x-1][joueur.mapPosition.y]};
                if(getType(places[joueur.facing]) == TREE) {
                    putInInventory(*bois, 1);
                }
                tabTick[13] = SDL_GetTicks();
            }
        }
    }

    //erreur menu
    if(menu == ERR_MENU)
    {
        if(connectedError == TRUE)
        {
            displayError("Error: Server looking offline :/");
        }
    }

    if ((menu == INGAME_MENU || menu == INVENTORY_MENU || menu == SETTINGS_INGAME_MENU || menu == SETTINGS_MAIN_KEYBIND_MENU))
    {
        if(connectedError == FALSE)
        {
            if(SOLO == TRUE || HOST == TRUE || CONNECTED == TRUE)
            {
                renderMap(&renderer, map_texture);
                drawPlayers(joueurs, size);
            }
        }
        else{
            changeMenu(ERR_MENU);
        }
    }
    if(checkPlayerOut(camera, &joueur, direction)) {
        moveCamera(camera, direction);
        updateGroundTexture(&renderer, &currentGround, window, tileset, camera, map);
        updateUtilsTexture(&renderer, &currentUtils, window, tileset, camera, map);
    }

    //animation main menu background
    if(menu == MAIN_MENU && ((SDL_GetTicks() - tabTick[14]) > 50 || tabTick[14] == 0)) {
        tabTick[14] = SDL_GetTicks();
        moveCamera(camera, EAST);
        if(camera->startPosition.x + camera->wRender + 1 > MAP_SIZE) {
            camera->startPosition.x = 0;
            camera->startPosition.y = rand() % (MAP_SIZE - camera->hRender - 1);
        }
        updateGroundTexture(&renderer, &currentGround, window, tileset, camera, map);
        updateUtilsTexture(&renderer, &currentUtils, window, tileset, camera, map);
    }
}

/**
 * @brief Initialise toutes les variables relatives aux menus.
 * 
 */
static void init_vars()
{
    loading = 0;
    l = creerListe();
    //freopen(newLogName(), "a+", stdout);
    rotation = 0;
    memset(tabEvent, 0, 20*sizeof(SDL_bool));
    init_menus_vars();

    build_map(&map);
    initPlayer(renderer, &joueur, map);
    initCamera(camera, window, &joueur);

    //Initialisation des tileset
    tmp = IMG_Load("resources/tileset_ground.png");
    tileset = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

    //Camera de jeu
    updateGroundTexture(&renderer, &currentGround, window, tileset, camera, map);
    updateUtilsTexture(&renderer, &currentUtils, window, tileset, camera, map);
}

/**
 * @brief Programme principal 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1],"-d"))
    {
        debug = 1;
    }

    init_vars();
    initAudio();
    creation_canaux_musique();
    loadSettings();
    int timer = 0;

    animationDelay = SDL_GetTicks();

    while(program_launched)
    {
        tick = SDL_GetTicks();

        update_screen();

        while(SDL_PollEvent(&event))
        {
            checkEvents();
        }
        doEvents();
        drawMenu();

        //on gère les fps
        if((timer = (1000 / 75)-(SDL_GetTicks() - tick)) > 0)
            SDL_Delay(timer);
        else
            SDL_Delay(17);
    }

    stopServer();
    stopConnection();
    TTF_CloseFont(police);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();
    destroyAll(window, renderer);

    return EXIT_SUCCESS;
}