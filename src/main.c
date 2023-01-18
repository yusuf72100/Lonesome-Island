#include "main.h"

static playersRect * pRects = NULL;

//tank rectangle
SDL_Rect rectangletank = {
    .x = 0,
    .y = 0,
    .w = 150,
    .h = 150
};

//game assets
SDL_Surface *imagetank = NULL;
SDL_Surface *imagebullet = NULL;
SDL_Texture *texturetank = NULL;
SDL_Texture *texturebullet = NULL;	

//menu assets
SDL_Surface *play_inert = NULL;
SDL_Surface *play_hover = NULL;
SDL_Surface *host_inert = NULL;
SDL_Surface *host_hover = NULL;
SDL_Texture *texture_play_inert = NULL;
SDL_Texture *texture_play_hover = NULL;	
SDL_Texture *texture_host_inert = NULL;
SDL_Texture *texture_host_hover = NULL;	

//window init
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Surface *image = NULL;
SDL_Surface *mousesurface = NULL;
SDL_Texture *mousetexture = NULL;

//background 
SDL_Surface *background = NULL;
SDL_Texture *background_texture = NULL;

void SDL_ExitWithError(const char *message);

//on récupère les données du socket
void *synch_datas(playersRect * playersRectangles)
{
    pRects = playersRectangles;

}

//on envoi nos données
void *Send2Server()
{
    sendPosition(rectangletank, rotation);
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
    //Vecteur.y = sin(angle/180*3.14);

    return Vecteur;
}

void destroyAll(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void *rechargement()
{
    Sleep(5000);
    loading = 0;
}

void UpdateBullet(Bullet * b)
{
  b->rectangle.x += b->Vitesse;
  //if (debug) printf("x = %d\n", b->rectangle.x);
  b->rectangle.y += b->Vitesse;
  //if (debug) printf("y = %d\n", b->rectangle.y);
}

void UpdateBulletAll()
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

void drawMouse(SDL_Rect mouseRect, SDL_Texture *mousetexture)
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
    //SDL_BlitSurface(mousesurface,NULL,background,&mouseRect);
}

//méthode pour dessiner plus facilement les rectangles (objets)
void dessinerRect(SDL_Rect rectangle, SDL_Renderer *renderer)
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

void dessinerButton(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, SDL_Surface *surface)
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
    //SDL_BlitSurface(surface,NULL,background,&rectangle);
}

void dessinerTank(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, int rotation)
{
    //SDL_RenderClear(renderer);
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du tank...");
    }

    if(SDL_RenderCopyEx(renderer, texture, NULL, &rectangle, rotation , NULL, SDL_FLIP_NONE) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de rotate le tank...");
    }

    //SDL_RenderPresent(renderer);
}

void dessinerJoueur(SDL_Rect rect)
{

    if(SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du tank...");
    }

    if(SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0 , NULL, SDL_FLIP_NONE) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de rotate le tank...");
    }
    SDL_RenderPresent(renderer);
}

void dessinerJoueurs()
{
    for(int i = 1; i <= pRects->size; i++)
    {
        dessinerJoueur(pRects->rectangles[i]);
        //printf("Joueur %d : x %d y %d\n",i,pRects->rectangles[i].x,pRects->rectangles[i].y);
    }
    SDL_RenderPresent(renderer);
}

void dessinerBalle(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, Bullet *b, int rotation, int vitesse)
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

    //SDL_RenderPresent(renderer);
}

void initBullet(Bullet * b, int x, int y, int rotation)
{
    b->rectangle.x = x;     //directives
    if (debug) printf("xbullet = %d\n", b->rectangle.x);
    b->rectangle.y = y;
    if (debug) printf("ybullet = %d\n", b->rectangle.y);
    b->rectangle.w = 2;
    b->rectangle.h = 8;
    b->rotation = rotation;
    if (debug) printf("rbullet = %d\n", b->rotation);

    b->DirX = cos(b->rotation);
    b->DirY = -sin(b->rotation); 
    b->Vitesse = 2;
}

void buttonHoverPlay(SDL_Window *window, SDL_Texture *texture_play_hover, SDL_Renderer *renderer, SDL_Rect play_button_rect)
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

void buttonHoverHost(SDL_Window *window, SDL_Texture *texture_host_hover, SDL_Renderer *renderer, SDL_Rect host_button_rect)
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

//programme principal 
int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1],"-d"))
    {
        debug = 1;
    }

    loading = 0;
    l = creerListe();
    //freopen(newLogName(), "a+", stdout); 

    Vecteur vecteur;
    pthread_t reloading;
    pthread_t server;
    pthread_t sendtoserver;
    pthread_t receivefromserver;

    Bullet *bullet = NULL;
    char *s;
    rotation = 0;
    int xMouse, yMouse;
    int xWindow = 0, yWindow = 0;

    //tab d'event
    SDL_bool tabEvent[9] = {SDL_FALSE};
    memset(tabEvent, 0, 7*sizeof(SDL_bool));
    
    //tank
    rectangletank.x = 0;
    rectangletank.y = 0;
    rectangletank.w = 150;
    rectangletank.h = 150;

    //menu buttons rectangle
    SDL_Rect play_button_rect;
    SDL_Rect host_button_rect;
    SDL_Rect mouseRect;

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

    //window icon
    SDL_Surface *icon_surface = NULL;

    //texte assets
    SDL_Surface *texte = NULL;
    SDL_Texture *texte_texture = NULL;
    SDL_Color blackColor = {255, 255, 255};
    TTF_Font *police = NULL;

    //retangle title windows = 696, 553
    SDL_Rect title_rect;

    //title rectangle
    title_rect.x = 200;
    title_rect.y = 100;
    title_rect.w = 200;
    title_rect.h = 200;

    //assets init
    icon_surface = IMG_Load("resources/icon.png");
    imagetank = IMG_Load("resources/tank.png");
    imagebullet = IMG_Load("resources/bullet.png");
    background = IMG_Load("resources/background.png");
    play_inert = IMG_Load("resources/play_inert.png");
    play_hover = IMG_Load("resources/play_hover.png");
    host_inert = IMG_Load("resources/host_inert.png");
    host_hover = IMG_Load("resources/host_hover.png");
    mousesurface = IMG_Load("resources/cursor.png");

    if(SDL_Init(SDL_INIT_VIDEO != 0))
        SDL_ExitWithError("Initialisation SDL");

    if(SDL_CreateWindowAndRenderer(696, 553, SDL_WINDOW_SHOWN, &window, &renderer) != 0)
        SDL_ExitWithError("Impossible de créer la fenêtre...");

    SDL_SetWindowIcon(window, icon_surface);
    SDL_SetWindowTitle(window,"Welcome");

    /*if(SDL_SetRenderDrawColor(renderer, 112, 168, 237, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur pour le rendu");*/

    //if(SDL_BlitSurface(texte,NULL,background,&rect_texte))SDL_Quit();

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

    image = IMG_Load("resources/tank.png");

    if(image == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }

    mousetexture = SDL_CreateTextureFromSurface(renderer, mousesurface);
    SDL_FreeSurface(mousesurface);

    if(mousesurface == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture de la souris...");
    }

    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    if(texture == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture...");
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

    if(imagetank == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }

    texturetank = SDL_CreateTextureFromSurface(renderer, imagetank);
    SDL_FreeSurface(imagetank);

    if(texturetank == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture...");
    }

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

    SDL_bool program_launched = SDL_TRUE;
    while(program_launched)
    {
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        SDL_Event event; 
        SDL_GetGlobalMouseState(&xMouse,&yMouse);
        SDL_GetWindowPosition(window, &xWindow, &yWindow);
        SDL_ShowCursor(SDL_DISABLE);

        while(SDL_PollEvent(&event))
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

        //execution des events
        if(tabEvent[0])
        {
            //touche Z
            if(play)
            {
                if (debug) printf("Touche SDLK_z pressee | %s\n", eventTime());
                rectangletank.y = rectangletank.y - 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
                //Send2Server();   
                /*vecteur = InitVecteur(rotation, 2);
                rectangletank.x += (int)vecteur.x;
                rectangletank.y += (int)vecteur.y;*/
                if(debug) printf("Coord X : %d\n", rectangletank.x);
                if(debug) printf("Coord Y : %d\n", rectangletank.y);
            }
        }

        if(tabEvent[1])
        {
            //touche Q
            if(play)
            {
                if (debug) printf("Touche SDLK_q pressee | %s\n", eventTime());
                rectangletank.x = rectangletank.x - 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);  
                //Send2Server(); 
            }
        }

        if(tabEvent[2])
        {
            //touche S
            if(play)
            {
                if (debug) printf("Touche SDLK_s pressee | %s\n", eventTime());
                rectangletank.y = rectangletank.y + 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);   
                //Send2Server(); 
            }
        }

        if(tabEvent[3])
        {
            //touche D
            if(play)
            {
                if (debug) printf("Touche SDLK_d pressee | %s\n", eventTime());
                rectangletank.x = rectangletank.x + 1;
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);   
                //Send2Server(); 
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
                    //bullet = malloc(sizeof(Bullet*) + 5);      
                    //initBullet(bullet, rectangletank.x, rectangletank.y, rotation);
                    //ajouterElement(l, bullet);       
                    if (debug) printf("SHOT FIRE!!!\n"); 

                    loading = 1;
                    pthread_create(&reloading,NULL,rechargement,NULL);
                    //if (debug) printf("TIR! | %s\n", eventTime());
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
                startConnection();      //on créer un client qui se connecte au serveur 
                Sleep(1000);
                pthread_create(&sendtoserver,NULL,Send2Server,NULL);    
                //Send2Server();   
                pthread_create(&receivefromserver,NULL,receiveFromServer,NULL); 
                play = 1;
            }

            //host button
            if(xMouse>=350+xWindow && xMouse<=450+xWindow && yMouse>=450+yWindow && yMouse<=500+yWindow && !play)
            {
                if (debug) printf("Host button clicked\n");
                pthread_create(&server,NULL,startServer,NULL);              //on héberge le serveur 
                Sleep(200);
                startConnection();       //on créer un client qui se connecte au serveur
                Sleep(500);
                pthread_create(&sendtoserver,NULL,Send2Server,NULL); 
                //Sleep(200);
                //Send2Server();   
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

        if (bullet != NULL)
        {
            //UpdateBullet(bullet);
            //dessinerBalle(texturebullet, renderer, bullet->rectangle, window, bullet, bullet->rotation, bullet->Vitesse);
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
            //dessinerTank(texturetank, renderer, rectangletank, window, rotation);
            if(pRects != NULL) dessinerJoueurs();
        }

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

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

//gcc src/main.c -o bin/prog src/liste.o -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread