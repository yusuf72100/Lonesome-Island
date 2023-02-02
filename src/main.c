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
void *synchDatas(player * Joueurs, int taille)
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
    char logName[100];

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

                switch(event.key.keysym.sym)
                {
                    case SDLK_z:
                        tabEvent[0] = SDL_TRUE;
                        break;

                    case SDLK_q:
                        tabEvent[1] = SDL_TRUE;
                        break;

                    case SDLK_s:
                        tabEvent[2] = SDL_TRUE;
                        break;

                    case SDLK_d:
                        tabEvent[3] = SDL_TRUE;
                        break;

                    case SDLK_LEFT:
                        tabEvent[4] = SDL_TRUE;
                        break;

                    case SDLK_RIGHT:
                        tabEvent[5] = SDL_TRUE;
                        break;

                    case SDLK_SPACE:
                        tabEvent[6] = SDL_TRUE;
                        break;
                    
                    case SDLK_ESCAPE:
                        tabEvent[10] = SDL_TRUE;
                        break;

                    case SDLK_LALT:
                        tabEvent[11] = SDL_TRUE;
                        break;

                    case SDLK_RETURN:
                        tabEvent[12] = SDL_TRUE;
                        break;
                }

            break;

            case SDL_KEYUP:

                switch(event.key.keysym.sym)
                {
                    case SDLK_z:
                        tabEvent[0] = SDL_FALSE;
                        break;

                    case SDLK_q:
                        tabEvent[1] = SDL_FALSE;
                        break;

                    case SDLK_s:
                        tabEvent[2] = SDL_FALSE;
                        break;

                    case SDLK_d:
                        tabEvent[3] = SDL_FALSE;
                        break;

                    case SDLK_LEFT:
                        tabEvent[4] = SDL_FALSE;
                        break;

                    case SDLK_RIGHT:
                        tabEvent[5] = SDL_FALSE;
                        break;

                    case SDLK_SPACE:
                        tabEvent[6] = SDL_FALSE;
                        break;

                    case SDLK_ESCAPE:
                        tabEvent[10] = SDL_FALSE;
                        break;

                    case SDLK_LALT:
                        tabEvent[11] = SDL_FALSE;
                        break;

                    case SDLK_RETURN:
                        tabEvent[12] = SDL_FALSE;
                        break;

                }

            break;

            //mouse buttons
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    tabEvent[7] = SDL_TRUE;
                    break;

                case SDL_BUTTON_RIGHT:
                    tabEvent[8] = SDL_TRUE;
                    break;

                case SDL_BUTTON_MIDDLE:
                    tabEvent[9] = SDL_TRUE;
                    break;
                }

            break;

            case SDL_MOUSEBUTTONUP:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    tabEvent[7] = SDL_FALSE;
                    break;

                case SDL_BUTTON_RIGHT:
                    tabEvent[8] = SDL_FALSE;
                    break;

                case SDL_BUTTON_MIDDLE:
                    tabEvent[9] = SDL_FALSE;
                    break;
                }

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
    while (clock() < start_time + milli_seconds && tabEvent[0] == SDL_FALSE && tabEvent[1] == SDL_FALSE && tabEvent[2] == SDL_FALSE && tabEvent[3] == SDL_FALSE && tabEvent[6] == SDL_FALSE && tabEvent[7] == SDL_FALSE);
}

/**
 * @brief Délai de l'animation "courrir à gauche"
 * 
 */
void delay_running_left()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[1] != SDL_FALSE);
}

/**
 * @brief Délai de l'animation "courrir à droite"
 * 
 */
void delay_running_right()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[3] != SDL_FALSE);
}

/**
 * @brief Délai de l'animation "courrir en haut"
 * 
 */
void delay_running_up()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[0] != SDL_FALSE);
}

/**
 * @brief Délai de l'animation "courrir en bas"
 * 
 */
void delay_running_down()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[2] != SDL_FALSE);
}

/**
 * @brief Délai de l'animation du bouton de paramètres quand on a pas le curseur dessus 
 * 
 */
void delay_settings_button_left()
{
    int milli_seconds = 50;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && hover_settingsbutton == SDL_FALSE);
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
    if(tabEvent[0])
        {
            //touche Z
            if (strcmp(getMenu(),"InGame") == 0)
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
            if(strcmp(getMenu(),"InGame") == 0)
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
            if(strcmp(getMenu(),"InGame") == 0)
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
            if(strcmp(getMenu(),"InGame") == 0)
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
            if(strcmp(getMenu(),"InGame") == 0)
            {
                //if (debug) printf("Touche SDLK_LEFT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation - 1;
            }
        }

        if(tabEvent[5])
        {
            //touche RIGHT
            if(strcmp(getMenu(),"InGame") == 0)
            {
                //if (debug) printf("Touche SDLK_RIGHT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation + 1;
            }
        }
        if(tabEvent[6])
        {
            //touche ESPACE
            if(strcmp(getMenu(),"InGame") == 0)
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
        //mouse events
        if(tabEvent[7])
        {
            //click LEFT DOWN
            
            //Connect button
            if(onButton("connect") && strcmp(getMenu(),"Main") == 0)
            {
                init_boop(&tabEvent[7]);
                if (debug) printf("Connect button clicked\n");                                         
                if(startConnection() == 0)                                     //on créer un client qui se connecte au serveur 
                {
                    Sleep(1000);
                    pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                    changeMenu("InGame");
                }
                else
                {
                    changeMenu("Error");
                }
                changeButtonState("connect");
            }

            //host button
            if(onButton("host") && strcmp(getMenu(),"Main") == 0)
            {
                init_boop(&tabEvent[7]);
                if (debug) printf("Host button clicked\n");
                pthread_create(&server,NULL,startServer,NULL);              //on héberge le serveur 
                startConnection();                                          //on créer un client qui se connecte au serveur
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);
                pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                changeMenu("InGame");
                changeButtonState("host");
            }

            //play button
            if(onButton("play") && strcmp(getMenu(),"Main") == 0)
            {
                init_boop(&tabEvent[7]);
                changeButtonState("play");
            }
            
            //settings button
            if(onButton("settings") && strcmp(getMenu(),"Main") == 0)
            {
                init_boop(&tabEvent[7]);
                changeButtonState("settings");
            }
        }
        if(!tabEvent[7])
        {
            //click LEFT UP
        }
        if(tabEvent[8])
        {
            //click RIGHT
        }
        if(tabEvent[9])
        {
            //click MIDDLE
        }
        if(tabEvent[0] == SDL_FALSE && tabEvent[1] == SDL_FALSE && tabEvent[2] == SDL_FALSE && tabEvent[3] == SDL_FALSE && tabEvent[6] == SDL_FALSE && tabEvent[7] == SDL_FALSE)
        {
            if(strcmp(getMenu(),"InGame") == 0)
            {
                if(pthread_kill(animations_thread, 0) != 0)
                    pthread_create(&animations_thread, NULL, breathAnimation,(void *)&joueur);  
            }
        }

        if(tabEvent[10] == SDL_TRUE)
        {
            //touche ESCAPE
            if(strcmp(getMenu(),"Error") == 0)
            {
                changeMenu("Main");
            }
        }

        if(tabEvent[11] == SDL_TRUE && tabEvent[12] == SDL_TRUE)
        {
            toggleFullscreen();
        }

        if (strcmp(getMenu(),"InGame") == 0)
        {
            if(connectedError == FALSE) drawPlayers(joueurs, size);
            else changeMenu("Main");
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
    //tab d'event
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
    int timer = 0;

    while(program_launched)
    {
        tick = SDL_GetTicks();
        update_screen();
        while(SDL_PollEvent(&event))
        {
            checkEvents();
        }

        //execution des events
        doEvents();
        mainMenu();

        if((timer = (1000 / 75)-(SDL_GetTicks() - tick)) > 0)
            SDL_Delay(timer);
        else    
            SDL_Delay(17);
    }  

    stopServer();
    stopConnection();
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    Mix_CloseAudio();
    destroyAll(window, renderer);

    return EXIT_SUCCESS;
}