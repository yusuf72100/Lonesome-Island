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
 * @brief Calcul un vecteur selon un angle de rotation et une vitesse.
 * 
 * @param angle 
 * @param vitesse 
 * @return Vecteur 
 */
Vecteur InitVecteur(int angle, int vitesse)
{
    Vecteur Vecteur;
    double bufferX;
    double bufferY;

    bufferX = cos(angle) * vitesse;
    bufferY = sin(angle) * vitesse; 

    Vecteur.x = round(bufferX);
    Vecteur.y = round(bufferY);

    if(debug) printf("Angle : %d\n", angle);
    if(debug) printf("Vecteur.x : %lf pour un cos de %lf\n", Vecteur.x, cos(angle));
    if(debug) printf("Vecteur.y : %lf pour un sin de %lf\n", Vecteur.y, sin(angle));

    return Vecteur;
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

static void UpdateBullet(Bullet * b)
{
  b->rectangle.x += b->Vitesse;
  b->rectangle.y += b->Vitesse;
}

static void UpdateBulletAll()
{
    Liste *courant = l->suivant;
    while(hasNext(courant))
    {
        if (debug) printf("Une bullet trouvé!\n");
        UpdateBullet(courant->b);
        courant = courant->suivant;
    }
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
            case SDL_KEYDOWN:

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
            break;

            case SDL_KEYUP:

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
            break;

            //mouse buttons
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
                if (debug) printf("Touche SDLK_z pressee | %s\n", eventTime());
                joueur.playerRect.y = joueur.playerRect.y - 3;

                if(tabEvent[1] == SDL_FALSE && tabEvent[3] == SDL_FALSE)
                {
                    if(pthread_kill(animations_thread, 0) != 0)
                        pthread_create(&animations_thread, NULL, running_up_animation,(void *)&joueur);  
                    pthread_create(&sendtoserver,NULL,Send2Server,NULL);
                }
            }
        }

        if(tabEvent[1])
        {
            //touche Q
            if(menu == INGAME_MENU)
            {
                joueur.playerRect.x = joueur.playerRect.x - 3;
                if (debug) printf("Touche SDLK_q pressee | %s\n", eventTime());
                
                if(pthread_kill(animations_thread, 0) != 0)
                    pthread_create(&animations_thread, NULL, running_left_animation,(void *)&joueur);  
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);
            }
        }

        if(tabEvent[2])
        {
            //touche S
            if(menu == INGAME_MENU)
            {
                if (debug) printf("Touche SDLK_s pressee | %s\n", eventTime());
                joueur.playerRect.y = joueur.playerRect.y + 3;

                if(tabEvent[1] == SDL_FALSE && tabEvent[3] == SDL_FALSE)
                {
                    if(pthread_kill(animations_thread, 0) != 0)
                        pthread_create(&animations_thread, NULL, running_down_animation,(void *)&joueur);  
                    pthread_create(&sendtoserver,NULL,Send2Server,NULL);
                }
            }
        }

        if(tabEvent[3])
        {
            //touche D
            if(menu == INGAME_MENU)
            {
                joueur.playerRect.x = joueur.playerRect.x + 3;
                if (debug) printf("Touche SDLK_d pressee | %s\n", eventTime());

                if(pthread_kill(animations_thread, 0) != 0)
                    pthread_create(&animations_thread, NULL, running_right_animation,(void *)&joueur);  
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
        //click LEFT DOWN
        
        //play button
        if(onButton(PLAY_BUTTON_HOVER) && menu == MAIN_MENU)
        {
            SOLO = TRUE;
            create_map();
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
            HOST = TRUE;           //on héberge le serveur 
            startConnection();                                          //on créer un client qui se connecte au serveur
            pthread_create(&sendtoserver,NULL,Send2Server,NULL);
            pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
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

        //settings keybind button
        if(onButton(SETTINGS_KEYBIND_HOVER) && menu == SETTINGS_MAIN_MENU)
        {
            if(getButtonState_clicked(SETTINGS_KEYBIND_CLICKED) == FALSE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    switchButtonState_hover(SETTINGS_KEYBIND_HOVER);
                    changeButtonState_clicked(SETTINGS_KEYBIND_CLICKED, TRUE);
                    tabTick[7] = SDL_GetTicks();
                    changeMenu(SETTINGS_MAIN_KEYBIND_MENU);
                }
            }
        }

        //settings keybind button
        if(onButton(SETTINGS_KEYBIND_HOVER) && menu == SETTINGS_MAIN_KEYBIND_MENU)
        {
            if(getButtonState_clicked(SETTINGS_KEYBIND_CLICKED) == FALSE)
            {
                if((SDL_GetTicks() - tabTick[7]) >= 200)
                {
                    init_boop(&tabEvent[7]);
                    switchButtonState_hover(SETTINGS_KEYBIND_HOVER);
                    changeButtonState_clicked(SETTINGS_KEYBIND_CLICKED, TRUE);
                    tabTick[7] = SDL_GetTicks();
                    changeMenu(SETTINGS_MAIN_MENU);
                }
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
        if(menu == ERR_MENU || menu == SETTINGS_MAIN_MENU)
        {
            if((SDL_GetTicks() - tabTick[10]) >= 200)
            {
                changeButtonState_hover(SETTINGS_KEYBIND_HOVER, FALSE);
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
        else if(menu == SETTINGS_INGAME_MENU){
            if((SDL_GetTicks() - tabTick[10]) >= 200)
            {
                changeMenu(INGAME_MENU);
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

    //erreur menu
    if(menu == ERR_MENU)
    {
        if(connectedError == TRUE)
            displayError("Error: Server looking offline :/");
    }

    if ((menu == INGAME_MENU || menu == INVENTORY_MENU || menu == SETTINGS_INGAME_MENU))
    {
        if(connectedError == FALSE) 
        {
            afficherMap();
            drawPlayers(joueurs, size);
        }
        else{
            changeMenu(ERR_MENU);
        }
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
    memset(tabEvent, 0, 7*sizeof(SDL_bool));
    init_menus_vars();
}

static void initBullet(Bullet * b, int x, int y, int rotation)
{
    b->rectangle.x = x;                                                 //directives
    if (debug) printf("xbullet = %d\n", b->rectangle.x);
    b->rectangle.y = y;
    if (debug) printf("ybullet = %d\n", b->rectangle.y);
    b->rectangle.w = 2;
    b->rectangle.h = 8;
    b->rotation = rotation;
    if (debug) printf("bullet = %d\n", b->rotation);

    b->DirX = cos(b->rotation);
    b->DirY = -sin(b->rotation); 
    b->Vitesse = 2;
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