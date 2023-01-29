#include "main.h"

void SDL_ExitWithError(const char *message);

void synchDatas(player * Joueurs, int taille)
{
    size = taille;
    joueurs = Joueurs;
}

//on envoi nos données
static void *Send2Server()
{
    sendDatas(joueur);
}

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

static void destroyAll(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

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

char* newLogName()
{
    struct tm * timeinfo;
    time_t now;

    time (&now);
    timeinfo = localtime ( &now );
    char *logName = malloc(sizeof(char)*100);

    //strcpy("logs/", logName);
    strftime(logName,30,"%b.%d.%Y.%H.%M.%S",timeinfo);
    strcat(logName, ".txt");
    logName = realloc(logName, sizeof(char) * strlen(logName)+1);

    return logName;
}

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

static void drawMouse(SDL_Rect mouseRect, SDL_Texture *mousetexture)
{
    if(SDL_QueryTexture(mousetexture, NULL, NULL, &mouseRect.w, &mouseRect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du curseur...");
    }

    SDL_RenderCopy(renderer, mousetexture, NULL, &mouseRect);
}

static void dessinerButton(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du boutton jouer...");
    }
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
}

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
                    
                    case SDL_MOUSEBUTTONUP:
                        tabEvent[7] = SDL_TRUE;
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

static void trierJoueurs()
{
    int position;
    int i,j;
    for(i = 1; i < size; i++)
    {
        player buffer = joueurs[i];
        position = 1;
        for(j = 1; j <= size; j++)
        {
            if(joueurs[i].playerRect.y > joueurs[j].playerRect.y) position++;
        }

        buffer = joueurs[position];
        joueurs[position] = joueurs[i];
        joueurs[i] = buffer;
    }
}

static void dessinerJoueur(player Joueur)
{
    if(Joueur.animation_state == BREATH_START)
    {
        if(SDL_QueryTexture(texture_joueur_h1, NULL, NULL, &Joueur.playerRect.w,&Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
        }
        SDL_RenderCopy(renderer, texture_joueur_h1, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == BREATH_END)
    {
        if(SDL_QueryTexture(texture_joueur_h2, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
        }
        SDL_RenderCopy(renderer, texture_joueur_h2, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_LEFT_START)
    {
        if(SDL_QueryTexture(texture_joueur_left_1, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running left 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_left_1, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_LEFT_END)
    {
        if(SDL_QueryTexture(texture_joueur_left_2, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running left 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_left_2, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_RIGHT_START)
    {
        if(SDL_QueryTexture(texture_joueur_right_1, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running right 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_right_1, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_RIGHT_END)
    {
        if(SDL_QueryTexture(texture_joueur_right_2, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running right 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_right_2, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_UP_START)
    {
        if(SDL_QueryTexture(texture_joueur_up_1, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running up 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_up_1, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_UP_END)
    {
        if(SDL_QueryTexture(texture_joueur_up_2, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running up 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_up_2, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_DOWN_START)
    {
        if(SDL_QueryTexture(texture_joueur_down_1, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running down 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_down_1, NULL, &Joueur.playerRect);
    }
    else if(Joueur.animation_state == RUNNING_DOWN_END)
    {
        if(SDL_QueryTexture(texture_joueur_down_2, NULL, NULL, &Joueur.playerRect.w, &Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running down 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_down_2, NULL, &Joueur.playerRect);
    }
    else
    {
        if(SDL_QueryTexture(texture_joueur_h1, NULL, NULL, &Joueur.playerRect.w,&Joueur.playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
        }
        SDL_RenderCopy(renderer, texture_joueur_h1, NULL, &Joueur.playerRect);
    }
}   

static void *dessinerJoueurs()
{
    //trierJoueurs();
    for(int i = 1; i < size; i++)
    {
        dessinerJoueur(joueurs[i]);
    }
}

void delay_breath()
{
    int milli_seconds = 2000;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[0] == SDL_FALSE && tabEvent[1] == SDL_FALSE && tabEvent[2] == SDL_FALSE && tabEvent[3] == SDL_FALSE && tabEvent[6] == SDL_FALSE && tabEvent[7] == SDL_FALSE);
}

void delay_running_left()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[1] != SDL_FALSE);
}

void delay_running_right()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[3] != SDL_FALSE);
}

void delay_running_up()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[0] != SDL_FALSE);
}

void delay_running_down()
{
    int milli_seconds = 200;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && tabEvent[2] != SDL_FALSE);
}

void delay_settings_button_left()
{
    int milli_seconds = 50;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds && hover_settingsbutton == FALSE);
}

void delay_settings_button_right()
{
    int milli_seconds = 50;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

static void *breathAnimation()
{    
    joueur.animation_state = BREATH_START;
    Send2Server();
    delay_breath();
    joueur.animation_state++;
    Send2Server();
    delay_breath();
    joueur.animation_state--;
    Send2Server();
    pthread_exit(&animations_thread);
}

static void *running_left_animation()
{
    joueur.animation_state = RUNNING_LEFT_START;
    delay_running_left();
    joueur.animation_state++;
    delay_running_left();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

static void *running_right_animation()
{
    joueur.animation_state = RUNNING_RIGHT_START;
    delay_running_right();
    joueur.animation_state++;
    delay_running_right();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

static void *running_up_animation()
{
    joueur.animation_state = RUNNING_UP_START;
    delay_running_up();
    joueur.animation_state++;
    delay_running_up();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

static void *running_down_animation()
{
    joueur.animation_state = RUNNING_DOWN_START;
    delay_running_down();
    joueur.animation_state++;
    delay_running_down();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

static void draw_settings_button_animation()
{
    if(strcmp(menu,"Main") == 0)
    {
        switch (settings_button_animation_state)
        {
        case 0:
            dessinerButton(texture_settings_hover1, settings_button_rect, settings_hover1);
            break;
        case 1:
            dessinerButton(texture_settings_hover2, settings_button_rect, settings_hover2);
            break;
        case 2:
            dessinerButton(texture_settings_hover3, settings_button_rect, settings_hover3);
            break;
        case 3:
            dessinerButton(texture_settings_hover4, settings_button_rect, settings_hover4);
            break;
        case 4:
            dessinerButton(texture_settings_hover5, settings_button_rect, settings_hover5);
            break;
        default:
            dessinerButton(texture_settings_hover6, settings_button_rect, settings_hover6);
            break;
        }
    }
}

static void *settings_button_animation_left()
{
    for(settings_button_animation_state; settings_button_animation_state >= 0 && hover_settingsbutton == FALSE; settings_button_animation_state--)
    {
        delay_settings_button_left();
    }

    settings_button_animation_state=0;
    animations_thread_running = FALSE;
    pthread_exit(&animations_thread);
}

static void *settings_button_animation_right()
{
    for(settings_button_animation_state; settings_button_animation_state < 6 && hover_settingsbutton == TRUE; settings_button_animation_state++)
    {
        delay_settings_button_right();
    }

    while(hover_settingsbutton == TRUE);
    animations_thread_running = FALSE;
    pthread_exit(&animations_thread);
}

static void doEvents()
{
    if(tabEvent[0])
        {
            //touche Z
            if(strcmp(menu,"InGame") == 0)
            {
                if (debug) printf("Touche SDLK_z pressee | %s\n", eventTime());
                joueur.playerRect.y = joueur.playerRect.y - 1;

                if(pthread_kill(animations_thread, 0) != 0){
                    pthread_create(&animations_thread, NULL, running_up_animation,NULL);   
                }
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
            }
        }

        if(tabEvent[1])
        {
            //touche Q
            if(strcmp(menu,"InGame") == 0)
            {
                joueur.playerRect.x = joueur.playerRect.x - 1;
                if(tabEvent[0] == SDL_FALSE && tabEvent[2] == SDL_FALSE)
                {
                    if (debug) printf("Touche SDLK_q pressee | %s\n", eventTime());
                    
                    if(pthread_kill(animations_thread, 0) != 0){
                        pthread_create(&animations_thread, NULL, running_left_animation,NULL);   
                    }
                    pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
                }
            }
        }

        if(tabEvent[2])
        {
            //touche S
            if(strcmp(menu,"InGame") == 0)
            {
                if (debug) printf("Touche SDLK_s pressee | %s\n", eventTime());
                joueur.playerRect.y = joueur.playerRect.y + 1;
                
                if(pthread_kill(animations_thread, 0) != 0){
                    pthread_create(&animations_thread, NULL, running_down_animation,NULL);   
                }
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
            }
        }

        if(tabEvent[3])
        {
            //touche D
            if(strcmp(menu,"InGame") == 0)
            {
                joueur.playerRect.x = joueur.playerRect.x + 1;
                if(tabEvent[0] == SDL_FALSE && tabEvent[2] == SDL_FALSE)
                {
                    if (debug) printf("Touche SDLK_d pressee | %s\n", eventTime());

                    if(pthread_kill(animations_thread, 0) != 0){
                        pthread_create(&animations_thread, NULL, running_right_animation,NULL);   
                    }
                    pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
                }
            }
        }

        if(tabEvent[4])
        {
            //touche LEFT
            if(strcmp(menu,"InGame") == 0)
            {
                //if (debug) printf("Touche SDLK_LEFT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation - 1;
            }
        }

        if(tabEvent[5])
        {
            //touche RIGHT
            if(strcmp(menu,"InGame") == 0)
            {
                //if (debug) printf("Touche SDLK_RIGHT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation + 1;
            }
        }
        if(tabEvent[6])
        {
            //touche ESPACE
            if(strcmp(menu,"InGame") == 0)
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
            if(xMouse>=connect_button_rect.x+xWindow && xMouse<=connect_button_rect.x+connect_button_rect.w+xWindow && yMouse>=connect_button_rect.y+yWindow && yMouse<=connect_button_rect.y+connect_button_rect.h+yWindow && strcmp(menu,"Main") == 0)
            {
                if (debug) printf("Connect button clicked\n");                                         
                if(startConnection() == 0)                                     //on créer un client qui se connecte au serveur 
                {
                    Sleep(1000);
                    pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                    strcpy(menu,"InGame");
                }
            }

            //host button
            if(xMouse>=host_button_rect.x+xWindow && xMouse<=host_button_rect.x+host_button_rect.w+xWindow && yMouse>=host_button_rect.y+yWindow && yMouse<=host_button_rect.y+host_button_rect.h+yWindow && strcmp(menu,"Main") == 0)
            {
                if (debug) printf("Host button clicked\n");
                pthread_create(&server,NULL,startServer,NULL);              //on héberge le serveur 
                Sleep(200);
                startConnection();                                          //on créer un client qui se connecte au serveur
                Sleep(500);
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
                pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                strcpy(menu,"InGame");
            }

            if(xMouse>=host_button_rect.x+xWindow && xMouse<=host_button_rect.x+host_button_rect.w+xWindow && yMouse>=host_button_rect.y+yWindow && yMouse<=host_button_rect.y+host_button_rect.h+yWindow && strcmp(menu,"Main") == 0)
            {
                if (debug) printf("Host button clicked\n");
                pthread_create(&server,NULL,startServer,NULL);              //on héberge le serveur 
                Sleep(200);
                startConnection();                                          //on créer un client qui se connecte au serveur
                Sleep(500);
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
                pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                strcpy(menu,"InGame");
            }
        }
        if(!tabEvent[7])
        {
            //click LEFT UP
            hover_playbutton = FALSE;
            hover_hostbutton = FALSE;
            hover_settingsbutton = FALSE;
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
            if(strcmp(menu,"InGame") == 0)
            {
                if(pthread_kill(animations_thread, 0) != 0){
                    pthread_create(&animations_thread, NULL, breathAnimation,NULL);   
                }
            }
        }

        if(strcmp(menu,"Main") == 0)
        {
            dessinerButton(texture_play_inert, play_button_rect, play_inert);
            dessinerButton(texture_connect_inert, connect_button_rect, connect_inert);
            dessinerButton(texture_host_inert, host_button_rect, host_inert);
        }
        if (strcmp(menu,"InGame") == 0)
        {
            if(connectedError == FALSE) dessinerJoueurs();
            else strcpy(menu, "Main");
        }
}

static void init_texture(SDL_Surface **surface, SDL_Texture **texture)
{
    if(*surface == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    *texture = SDL_CreateTextureFromSurface(renderer, *surface);
    SDL_FreeSurface(*surface);
}

static void init_vars()
{
    xMouse = 0, yMouse = 0;
    xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetGlobalMouseState(&xMouse,&yMouse);

    loading = 0;
    l = creerListe();
    //freopen(newLogName(), "a+", stdout); 

    rotation = 0;

    //tab d'event
    memset(tabEvent, 0, 7*sizeof(SDL_bool));
    
    joueur.playerRect.x = 0;
    joueur.playerRect.y = 0;
    joueur.playerRect.w = 50;
    joueur.playerRect.h = 81;

    //cursor rectangle
    mouseRect.w = 50;
    mouseRect.h = 50;

    //play button
    play_button_rect.w = 400;
    play_button_rect.h = 200;
    play_button_rect.x = (WindowW / 2) - (play_button_rect.w / 2);
    play_button_rect.y = 250;

    //connect button
    connect_button_rect.w = 400;
    connect_button_rect.h = 200;
    connect_button_rect.x = (WindowW / 2) - (connect_button_rect.w / 2);
    connect_button_rect.y = 500;

    //host button
    host_button_rect.w = 400;
    host_button_rect.h = 200;
    host_button_rect.x = (WindowW / 2) - (host_button_rect.w / 2);
    host_button_rect.y = 750;

    //settings button
    settings_button_rect.w = 150;
    settings_button_rect.h = 150;
    settings_button_rect.x = 30;
    settings_button_rect.y = (WindowH - 50) - (settings_button_rect.h);

    //title rectangle
    title_rect.w = 20;
    title_rect.h = 20;
    title_rect.x = 170;
    title_rect.y = 20;

    //assets init
    icon_surface = IMG_Load("resources/icon.png");
    imagebullet = IMG_Load("resources/bullet.png");
    mousesurface = IMG_Load("resources/cursor/cursor.png");
    background = IMG_Load("resources/background.png");
    play_inert = IMG_Load("resources/play_inert.png");
    play_hover = IMG_Load("resources/play_hover.png");
    connect_inert = IMG_Load("resources/connect_inert.png");
    connect_hover = IMG_Load("resources/connect_hover.png");
    host_inert = IMG_Load("resources/host_inert.png");
    host_hover = IMG_Load("resources/host_hover.png");
    settings_inert = IMG_Load("resources/settings_inert.png");
    settings_hover1 = IMG_Load("resources/settings_hover1.png");
    settings_hover2 = IMG_Load("resources/settings_hover2.png");
    settings_hover3 = IMG_Load("resources/settings_hover3.png");
    settings_hover4 = IMG_Load("resources/settings_hover4.png");
    settings_hover5 = IMG_Load("resources/settings_hover5.png");
    settings_hover6 = IMG_Load("resources/settings_hover6.png");

    //player    
    surface_joueur_h1 = IMG_Load("resources/characters/player_h1.png");
    surface_joueur_h2 = IMG_Load("resources/characters/player_h2.png");
    surface_joueur_left_1 = IMG_Load("resources/characters/player_left_1.png");
    surface_joueur_left_2 = IMG_Load("resources/characters/player_left_2.png");
    surface_joueur_right_1 = IMG_Load("resources/characters/player_right_1.png");
    surface_joueur_right_2 = IMG_Load("resources/characters/player_right_2.png");
    surface_joueur_up_1 = IMG_Load("resources/characters/player_up_1.png");
    surface_joueur_up_2 = IMG_Load("resources/characters/player_up_2.png");
    surface_joueur_down_1 = IMG_Load("resources/characters/player_down_1.png");
    surface_joueur_down_2 = IMG_Load("resources/characters/player_down_2.png");

    if(SDL_Init(SDL_INIT_EVERYTHING != 0))
        SDL_ExitWithError("Initialisation SDL");

    if((window = SDL_CreateWindow("Lonesome Island",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_MAXIMIZED)) == NULL)
        SDL_ExitWithError("Impossible de créer la fenêtre...");
    
    if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
        SDL_ExitWithError("Impossible de créer le rendu...");

    SDL_SetWindowIcon(window, icon_surface);

    if (TTF_Init() == -1)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    police = TTF_OpenFont("resources/04b30.TTF", 30);

    if (police == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la police...");
    }
    mousetexture = SDL_CreateTextureFromSurface(renderer, mousesurface);

    if(mousesurface == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture de la souris...");
    }
    texte = TTF_RenderText_Blended(police, "Lonesome Island", blackColor);

    if (texte == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    texte_texture = SDL_CreateTextureFromSurface(renderer, texte);

    if (texte_texture == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    SDL_BlitSurface(texte,NULL,background,&title_rect);
    
    //buttons
    init_texture(&play_inert , &texture_play_inert);
    init_texture(&play_hover , &texture_play_hover);
    init_texture(&connect_inert , &texture_connect_inert);
    init_texture(&connect_hover , &texture_connect_hover);
    init_texture(&host_inert , &texture_host_inert);
    init_texture(&host_hover , &texture_host_hover);
    init_texture(&settings_inert , &texture_settings_inert);
    init_texture(&settings_hover1 , &texture_settings_hover1);
    init_texture(&settings_hover2 , &texture_settings_hover2);
    init_texture(&settings_hover3 , &texture_settings_hover3);
    init_texture(&settings_hover4 , &texture_settings_hover4);
    init_texture(&settings_hover5 , &texture_settings_hover5);
    init_texture(&settings_hover6 , &texture_settings_hover6);

    //assets
    init_texture(&background , &background_texture);
    init_texture(&surface_joueur_h1 , &texture_joueur_h1);
    init_texture(&surface_joueur_h2 , &texture_joueur_h2);
    init_texture(&surface_joueur_left_1 , &texture_joueur_left_1);
    init_texture(&surface_joueur_left_2 , &texture_joueur_left_2);
    init_texture(&surface_joueur_right_1 , &texture_joueur_right_1);
    init_texture(&surface_joueur_right_2 , &texture_joueur_right_2);
    init_texture(&surface_joueur_up_1 , &texture_joueur_up_1);
    init_texture(&surface_joueur_up_2 , &texture_joueur_up_2);
    init_texture(&surface_joueur_down_1 , &texture_joueur_down_1);
    init_texture(&surface_joueur_down_2 , &texture_joueur_down_2);
}

static void dessinerBalle(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, Bullet *b, int rotation, int vitesse)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture de la balle...");
    }

    if(SDL_RenderCopyEx(renderer, texture, NULL, &rectangle, rotation , NULL, SDL_FLIP_NONE) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de rotate le la balle...");
    }
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

static void buttonHover(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, short *hover_button, char *menuTarget)
{
    xMouse = 0, yMouse = 0;
    xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetGlobalMouseState(&xMouse,&yMouse);

    if(strcmp(menu,menuTarget) == 0)
    {
        if(xMouse>=(button_rect->x)+xWindow && xMouse<=(button_rect->x)+(button_rect->w)+xWindow && yMouse>=(button_rect->y)+yWindow && yMouse<=(button_rect->y)+(button_rect->h)+yWindow)
        {
            *hover_button = TRUE;
            dessinerButton(button_texture, *button_rect, button_surface);
        }
        else
        {
            *hover_button = FALSE;
        }
    }
}

static void buttonHoverWithAnimation(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, char *menuTarget, void* (*p)(void*), void* (*p2)(void*))
{
    xMouse = 0, yMouse = 0;
    xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetGlobalMouseState(&xMouse,&yMouse);

    if(strcmp(menu,menuTarget) == 0)
    {
        if(xMouse>=(button_rect->x)+xWindow && xMouse<=(button_rect->x)+(button_rect->w)+xWindow && yMouse>=(button_rect->y)+yWindow && yMouse<=(button_rect->y)+(button_rect->h)+yWindow)
        {
            hover_settingsbutton = TRUE;
            if(animations_thread_running == FALSE){
                animations_thread_running = TRUE;
                pthread_create(&animations_thread, NULL, p, NULL);   
            }
        }
        else
        {
            hover_settingsbutton = FALSE;            
            if(animations_thread_running == FALSE){
                animations_thread_running = TRUE;
                pthread_create(&animations_thread, NULL, p2, NULL);   
            }
        }
    }
}

//programme principal 
int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1],"-d"))
    {
        debug = 1;
    }

    init_vars();
    int timer = 0;
    void* (*p)() = settings_button_animation_right;
    void* (*p2)() = settings_button_animation_left;

    while(program_launched)
    {
        tick = SDL_GetTicks();
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        SDL_GetGlobalMouseState(&xMouse,&yMouse);
        SDL_GetWindowPosition(window, &xWindow, &yWindow);
        SDL_ShowCursor(SDL_DISABLE);

        while(SDL_PollEvent(&event))
        {
            checkEvents();
        }

        //execution des events
        doEvents();

        mouseRect.x = xMouse-xWindow;
        mouseRect.y = yMouse-yWindow;

        buttonHover(play_hover, texture_play_hover, &play_button_rect, &hover_playbutton, "Main");
        buttonHover(connect_hover, texture_connect_hover, &connect_button_rect, &hover_connectbutton, "Main");
        buttonHover(host_hover, texture_host_hover, &host_button_rect, &hover_hostbutton, "Main");
        buttonHoverWithAnimation(settings_hover1, texture_settings_hover1, &settings_button_rect, "Main", p, p2);
        draw_settings_button_animation();
        drawMouse(mouseRect, mousetexture);

        SDL_RenderPresent(renderer);

        if((timer = (1000 / 60)-(SDL_GetTicks() - tick)) > 0)
            SDL_Delay(timer);
        else    
            SDL_Delay(15);
    }  

    //free window
    destroyAll(window, renderer);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    stopConnection();

    return EXIT_SUCCESS;
}

static void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}