#include "main.h"

void SDL_ExitWithError(const char *message);

//on récupère les données du socket
void *synch_datas(playersRect * playersRectangles)
{
    pRects = playersRectangles;
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

    if(SDL_RenderCopyEx(renderer, mousetexture, NULL, &mouseRect, 0 , NULL, SDL_FLIP_NONE) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de rotate le curseur...");
    }
}

//méthode pour dessiner plus facilement les rectangles (objets)
static void dessinerRect(SDL_Rect rectangle, SDL_Renderer *renderer)
{
    if(SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur pour le rendu");
        
    SDL_RenderClear(renderer);

    if(SDL_SetRenderDrawColor(renderer, 112, 168, 237, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur pour le rendu");

    if(SDL_RenderFillRect(renderer, &rectangle) != 0)
        SDL_ExitWithError("Impossible de dessiner un rectangle"); 

    SDL_RenderPresent(renderer);
}

static void dessinerButton(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, SDL_Surface *surface)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du boutton jouer...");
    }

    if(SDL_RenderCopyEx(renderer, texture, NULL, &rectangle, 0 , NULL, SDL_FLIP_NONE) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de rotate le boutton play...");
    }
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
    for(int i = 1; i <= pRects->size; i++)
    {
        SDL_Rect buffer = pRects->rectangles[i];
        position = 1;
        for(int j = 1; j <= pRects->size; j++)
        {
            if(pRects->rectangles[j].y < pRects->rectangles[i].y) position++;
        }

        buffer = pRects->rectangles[position];
        pRects->rectangles[position] = pRects->rectangles[i];
        pRects->rectangles[i] = buffer;
    }
}

static void dessinerJoueur(player Joueur)
{
    if(Joueur.animation_state == BREATH_START)
    {
        if(SDL_QueryTexture(texture_joueur_h1, NULL, NULL, &Joueur->playerRect.w,&Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
        }
        SDL_RenderCopy(renderer, texture_joueur_h1, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == BREATH_END)
    {
        if(SDL_QueryTexture(texture_joueur_h2, NULL, NULL, &Joueur->playerRect.w, &&Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
        }
        SDL_RenderCopy(renderer, texture_joueur_h2, NULL, &&Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_LEFT_START)
    {
        if(SDL_QueryTexture(texture_joueur_left_1, NULL, NULL, &Joueur->playerRect.w &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running left 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_left_1, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_LEFT_END)
    {
        if(SDL_QueryTexture(texture_joueur_left_2, NULL, NULL, &Joueur->playerRect.w, &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running left 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_left_2, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_RIGHT_START)
    {
        if(SDL_QueryTexture(texture_joueur_right_1, NULL, NULL, &Joueur->playerRect.w, &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running right 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_right_1, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_RIGHT_END)
    {
        if(SDL_QueryTexture(texture_joueur_right_2, NULL, NULL, &Joueur->playerRect.w, &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running right 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_right_2, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_UP_START)
    {
        if(SDL_QueryTexture(texture_joueur_up_1, NULL, NULL, &Joueur->playerRect.w, &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running up 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_up_1, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_UP_END)
    {
        if(SDL_QueryTexture(texture_joueur_up_2, NULL, NULL, &Joueur->playerRect.w, &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running up 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_up_2, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_DOWN_START)
    {
        if(SDL_QueryTexture(texture_joueur_down_1, NULL, NULL, &Joueur->playerRect.w, &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running down 1 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_down_1, NULL, &Joueur->playerRect);
    }
    else if(Joueur.animation_state == RUNNING_DOWN_END)
    {
        if(SDL_QueryTexture(texture_joueur_down_2, NULL, NULL, &Joueur->playerRect.w, &Joueur->playerRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du joueur running down 2 ...");
        }
        SDL_RenderCopy(renderer, texture_joueur_down_2, NULL, &Joueur->playerRect);
    }
}   

static void *dessinerJoueurs()
{
    trierJoueurs();
    for(int i = 1; i <= size; i++)
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

static void *breathAnimation()
{    
    joueur.animation_state = BREATH_START;
    delay_breath();
    joueur.animation_state++;
    delay_breath();
    joueur.animation_state--;
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

static void doEvents()
{
    if(tabEvent[0])
        {
            //touche Z
            if(play)
            {
                if (debug) printf("Touche SDLK_z pressee | %s\n", eventTime());
                joueur.playerRect.y = joueur.playerRect.y - 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 

                if(pthread_kill(animations_thread, 0) != 0){
                    pthread_create(&animations_thread, NULL, running_up_animation,NULL);   
                }
            }
        }

        if(tabEvent[1])
        {
            //touche Q
            if(play)
            {
                if (debug) printf("Touche SDLK_q pressee | %s\n", eventTime());
                joueur.playerRect.x = joueur.playerRect.x - 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);  
                
                if(pthread_kill(animations_thread, 0) != 0){
                    pthread_create(&animations_thread, NULL, running_left_animation,NULL);   
                }
            }
        }

        if(tabEvent[2])
        {
            //touche S
            if(play)
            {
                if (debug) printf("Touche SDLK_s pressee | %s\n", eventTime());
                joueur.playerRect.y = joueur.playerRect.y + 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);   
                
                if(pthread_kill(animations_thread, 0) != 0){
                    pthread_create(&animations_thread, NULL, running_down_animation,NULL);   
                }
            }
        }

        if(tabEvent[3])
        {
            //touche D
            if(play)
            {
                if (debug) printf("Touche SDLK_d pressee | %s\n", eventTime());
                joueur.playerRect.x = joueur.playerRect.x + 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);   

                if(pthread_kill(animations_thread, 0) != 0){
                    pthread_create(&animations_thread, NULL, running_right_animation,NULL);   
                }
            }
        }

        if(tabEvent[4])
        {
            //touche LEFT
            if(play)
            {
                //if (debug) printf("Touche SDLK_LEFT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation - 1;
            }
        }

        if(tabEvent[5])
        {
            //touche RIGHT
            if(play)
            {
                //if (debug) printf("Touche SDLK_RIGHT pressee | %s\n", eventTime());
                if (debug) printf("Rotation : %d\n", rotation);
                rotation = rotation + 1;
            }
        }
        if(tabEvent[6])
        {
            //touche ESPACE
            if(play)
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

            //play button
            if(xMouse>=350+xWindow && xMouse<=450+xWindow && yMouse>=250+yWindow && yMouse<=300+yWindow && !play)
            {
                if (debug) printf("Play button clicked\n");
                startConnection();                                          //on créer un client qui se connecte au serveur 
                Sleep(1000);
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);    
                pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                play = 1;
            }

            //host button
            if(xMouse>=350+xWindow && xMouse<=450+xWindow && yMouse>=450+yWindow && yMouse<=500+yWindow && !play)
            {
                if (debug) printf("Host button clicked\n");
                pthread_create(&server,NULL,startServer,NULL);              //on héberge le serveur 
                Sleep(200);
                startConnection();                                          //on créer un client qui se connecte au serveur
                Sleep(500);
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
                pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                play = 1;
            }
        }
        if(!tabEvent[7])
        {
            //click LEFT UP
            hover_playbutton = 0;
            hover_hostbutton = 0;
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
            if(pthread_kill(animations_thread, 0) != 0){
                pthread_create(&animations_thread, NULL, breathAnimation,NULL);   
            }
        }
        if(!hover_playbutton && !play)
        {
            dessinerButton(texture_play_inert, renderer, play_button_rect, window, play_inert);
        }
        if(!hover_hostbutton && !play)
        {
            dessinerButton(texture_host_inert, renderer, host_button_rect, window, host_inert);
        }
        if (play)
        {
            if(pRects != NULL) 
            {
                affichage();
            }
        }
}

static void init_vars()
{
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

    //connect button
    host_button_rect.x = 350;
    host_button_rect.y = 450;
    host_button_rect.w = 100;
    host_button_rect.h = 50;

    //play button
    play_button_rect.x = 350;
    play_button_rect.y = 250;
    play_button_rect.w = 100;
    play_button_rect.h = 50;


                                /* INIT TEXTURES AND MANAGE ERRORS */
    /* ----------------------------------------------------------------------------------------- */      

    //title rectangle
    title_rect.x = 200;
    title_rect.y = 100;
    title_rect.w = 200;
    title_rect.h = 200;

    //assets init
    icon_surface = IMG_Load("resources/icon.png");
    imagebullet = IMG_Load("resources/bullet.png");
    background = IMG_Load("resources/background.png");
    play_inert = IMG_Load("resources/play_inert.png");
    play_hover = IMG_Load("resources/play_hover.png");
    host_inert = IMG_Load("resources/host_inert.png");
    host_hover = IMG_Load("resources/host_hover.png");
    mousesurface = IMG_Load("resources/cursor/cursor.png");
    
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

    if(SDL_Init(SDL_INIT_VIDEO != 0))
        SDL_ExitWithError("Initialisation SDL");

    if(SDL_CreateWindowAndRenderer(696, 553, SDL_WINDOW_SHOWN, &window, &renderer) != 0)
        SDL_ExitWithError("Impossible de créer la fenêtre...");

    SDL_SetWindowIcon(window, icon_surface);
    SDL_SetWindowTitle(window,"Lonesome Island");

    if (TTF_Init() == -1)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    police = TTF_OpenFont("resources/couscousse.ttf", 30);

    if (police == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la police...");
    }
    mousetexture = SDL_CreateTextureFromSurface(renderer, mousesurface);
    SDL_FreeSurface(mousesurface);

    if(mousesurface == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture de la souris...");
    }
    texte = TTF_RenderText_Blended(police, "Welcome young trout!", blackColor);

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
    SDL_FreeSurface(texte);

    if(play_inert == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    texture_play_inert = SDL_CreateTextureFromSurface(renderer, play_inert);
    SDL_FreeSurface(play_inert);
    
    if(play_hover == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    texture_play_hover = SDL_CreateTextureFromSurface(renderer, play_hover);
    SDL_FreeSurface(play_hover);

    if(host_inert == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    texture_host_inert = SDL_CreateTextureFromSurface(renderer, host_inert);
    SDL_FreeSurface(host_inert);
    
    if(host_hover == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    texture_host_hover = SDL_CreateTextureFromSurface(renderer, host_hover);
    SDL_FreeSurface(host_hover);

    if(background == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    background_texture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);

    if(surface_joueur_h1 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image surface_joueur_h1...");
    }
    texture_joueur_h1 = SDL_CreateTextureFromSurface(renderer, surface_joueur_h1);
    SDL_FreeSurface(surface_joueur_h1);

    if(surface_joueur_h2 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture surface_joueur_h2...");
    }
    texture_joueur_h2 = SDL_CreateTextureFromSurface(renderer, surface_joueur_h2);
    SDL_FreeSurface(surface_joueur_h2);

    if(surface_joueur_left_1 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_left_1...");
    }
    texture_joueur_left_1 = SDL_CreateTextureFromSurface(renderer, surface_joueur_left_1);
    SDL_FreeSurface(surface_joueur_left_1);

    if(surface_joueur_left_2 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_left_2...");
    }
    texture_joueur_left_2 = SDL_CreateTextureFromSurface(renderer, surface_joueur_left_2);
    SDL_FreeSurface(surface_joueur_left_2);

    if(surface_joueur_right_1 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_right_1...");
    }
    texture_joueur_right_1 = SDL_CreateTextureFromSurface(renderer, surface_joueur_right_1);
    SDL_FreeSurface(surface_joueur_right_1);

    if(surface_joueur_right_2 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_right_2...");
    }
    texture_joueur_right_2 = SDL_CreateTextureFromSurface(renderer, surface_joueur_right_2);
    SDL_FreeSurface(surface_joueur_right_2);

    if(surface_joueur_up_1 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_up_1...");
    }
    texture_joueur_up_1 = SDL_CreateTextureFromSurface(renderer, surface_joueur_up_1);
    SDL_FreeSurface(surface_joueur_up_1);

    if(surface_joueur_up_2 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_up_2...");
    }
    texture_joueur_up_2 = SDL_CreateTextureFromSurface(renderer, surface_joueur_up_2);
    SDL_FreeSurface(surface_joueur_up_2);

    if(surface_joueur_down_1 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_down_1...");
    }
    texture_joueur_down_1 = SDL_CreateTextureFromSurface(renderer, surface_joueur_down_1);
    SDL_FreeSurface(surface_joueur_down_1);

    if(surface_joueur_down_2 == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture texture_joueur_down_2...");
    }
    texture_joueur_down_2 = SDL_CreateTextureFromSurface(renderer, surface_joueur_down_2);
    SDL_FreeSurface(surface_joueur_down_2);

    if(imagebullet == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image de la bullet...");
    }
    texturebullet = SDL_CreateTextureFromSurface(renderer, imagebullet);
    SDL_FreeSurface(imagebullet);

    if(texturebullet == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture de la bullet...");
    }
}

static void dessinerBalle(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, Bullet *b, int rotation, int vitesse)
{
    //SDL_RenderClear(renderer);
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

static void buttonHoverPlay(SDL_Window *window, SDL_Texture *texture_play_hover, SDL_Renderer *renderer, SDL_Rect play_button_rect)
{
    int xMouse = 0, yMouse = 0;
    int xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetGlobalMouseState(&xMouse,&yMouse);

    if (!play)
    {
        if(xMouse>=350+xWindow && xMouse<=450+xWindow && yMouse>=250+yWindow && yMouse<=300+yWindow)
        {
            hover_playbutton = 1;
            if (debug) printf("Hover play button\n");
            dessinerButton(texture_play_hover, renderer, play_button_rect, window, play_hover);
        }
        else
        {
            hover_playbutton = 0;
            if (debug) printf("X: %d et y: %d\n",xMouse,yMouse);
        }
    }   
}

static void buttonHoverHost(SDL_Window *window, SDL_Texture *texture_host_hover, SDL_Renderer *renderer, SDL_Rect host_button_rect)
{
    int xMouse = 0, yMouse = 0;
    int xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetGlobalMouseState(&xMouse,&yMouse);

    if (!play)
    {
        if(xMouse>=350+xWindow && xMouse<=450+xWindow && yMouse>=450+yWindow && yMouse<=500+yWindow)
        {
            hover_playbutton = 1;
            if (debug) printf("Hover host button\n");
            dessinerButton(texture_host_hover, renderer, host_button_rect, window, host_hover);
        }
        else
        {                
            hover_hostbutton = 0;
            if (debug) printf("X: %d et y: %d\n",xMouse,yMouse);
        }
    }   
}

static void affichage()
{
    dessinerJoueurs();
}

//programme principal 
int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1],"-d"))
    {
        debug = 1;
    }

    init_vars();

    while(program_launched)
    {
        SDL_RenderCopy(renderer, background_texture, NULL, NULL)        if (bullet != NULL)
        {
            //UpdateBullet(bullet);
            //dessinerBalle(texturebullet, renderer, bullet->rectangle, window, bullet, bullet->rotation, bullet->Vitesse);
        }
        ;
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

        buttonHoverPlay(window, texture_play_hover, renderer, play_button_rect);
        buttonHoverHost(window, texture_host_hover, renderer, host_button_rect);
        drawMouse(mouseRect, mousetexture);
        SDL_RenderPresent(renderer);

        Sleep(20);
    }  

    //free window
    destroyAll(window, renderer);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

static void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}