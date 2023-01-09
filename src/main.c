#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <pthread.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init.h"
#include "liste.h"

Liste *l;
short loading;
short play = 0;
short hover = 0;
short debug = 0; 

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

void dessinerPlayButton(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window)
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

    SDL_RenderPresent(renderer);
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

void buttonHover(SDL_Window *window, SDL_Texture *texture_play_hover, SDL_Renderer *renderer, SDL_Rect play_button_rect)
{
    int xMouse = 0, yMouse = 0;
    int xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetGlobalMouseState(&xMouse,&yMouse);

    if (!play)
    {
        if(xMouse>=350+xWindow && xMouse<=450+xWindow && yMouse>=250+yWindow && yMouse<=300+yWindow)
        {
            hover = 1;
            if (debug) printf("Hover play button\n");
            dessinerPlayButton(texture_play_hover, renderer, play_button_rect, window);
        }
        else
        {
            hover = 0;
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
    Bullet *bullet = NULL;
    char *s;
    int rotation = 0;
    int xMouse, yMouse;

    //tab d'event
    SDL_bool tabEvent[9] = {SDL_FALSE};

    //window init
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //tank rectangle
    SDL_Rect rectangletank;

    //menu buttons rectangle
    SDL_Rect play_button_rect;

    //texte assets
    SDL_Surface *texte = NULL;
    SDL_Texture *texte_texture = NULL;
    SDL_Color blackColor = {255, 255, 255};
    TTF_Font *police = NULL;

    //retangle title windows = 696, 553
    SDL_Rect title_rect;

    //game assets
    SDL_Surface *imagetank = NULL;
    SDL_Surface *imagebullet = NULL;
    SDL_Texture *texturetank = NULL;
    SDL_Texture *texturebullet = NULL;	

    //background 
    SDL_Surface *background = NULL;
    SDL_Texture *background_texture = NULL;

    //menu assets
    SDL_Surface *play_inert = NULL;
    SDL_Surface *play_hover = NULL;
    SDL_Texture *texture_play_inert = NULL;
    SDL_Texture *texture_play_hover = NULL;	

    SDL_bool program_launched = SDL_TRUE;
    while(program_launched)
    {
        SDL_Event event; 

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
            if (debug) printf("Touche SDLK_z pressee | %s\n", eventTime());
            rectangletank.y = rectangletank.y - 1;
            /*vecteur = InitVecteur(rotation, 2);
            rectangletank.x += (int)vecteur.x;
            rectangletank.y += (int)vecteur.y;*/
            if(debug) printf("Coord X : %d\n", rectangletank.x);
            if(debug) printf("Coord Y : %d\n", rectangletank.y);
        }

        if(tabEvent[1])
        {
            //touche Q
            if (debug) printf("Touche SDLK_q pressee | %s\n", eventTime());
            rectangletank.x = rectangletank.x - 1;
        }

        if(tabEvent[2])
        {
            //touche S
            if (debug) printf("Touche SDLK_s pressee | %s\n", eventTime());
            rectangletank.y = rectangletank.y + 1;
        }

        if(tabEvent[3])
        {
            //touche D
            if (debug) printf("Touche SDLK_d pressee | %s\n", eventTime());
            rectangletank.x = rectangletank.x + 1;
        }

        if(tabEvent[4])
        {
            //touche LEFT
            //if (debug) printf("Touche SDLK_LEFT pressee | %s\n", eventTime());
            if (debug) printf("Rotation : %d\n", rotation);
            rotation = rotation - 1;
        }

        if(tabEvent[5])
        {
            //touche RIGHT
            //if (debug) printf("Touche SDLK_RIGHT pressee | %s\n", eventTime());
            if (debug) printf("Rotation : %d\n", rotation);
            rotation = rotation + 1;
        }
        if(tabEvent[6])
        {
            //touche ESPACE
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
        //mouse events
        if(tabEvent[7])
        {
            //click LEFT DOWN
            int xWindow = 0, yWindow = 0;

            SDL_GetWindowPosition(window, &xWindow, &yWindow);
            SDL_GetGlobalMouseState(&xMouse,&yMouse);

            if(xMouse>=350+xWindow && xMouse<=450+xWindow && yMouse>=250+yWindow && yMouse<=300+yWindow)
            {
                if (debug) printf("Play button clicked\n");
                play = 1;
            }
        }
        if(!tabEvent[7])
        {
            //click LEFT UP
            hover = 0;
        }
        if(tabEvent[8])
        {
            //click RIGHT
            SDL_GetGlobalMouseState(&xMouse,&yMouse);
        }
        if(tabEvent[9])
        {
            //click MIDDLE
            SDL_GetGlobalMouseState(&xMouse,&yMouse);
        }

        Sleep(20);
        if (bullet != NULL)
        {
            //UpdateBullet(bullet);
            //dessinerBalle(texturebullet, renderer, bullet->rectangle, window, bullet, bullet->rotation, bullet->Vitesse);
        }

        if (play)
        {
            dessinerTank(texturetank, renderer, rectangletank, window, rotation);
        }
        
        if(!hover && !play)
        {
            dessinerPlayButton(texture_play_inert, renderer, play_button_rect, window);
        }

        //drawTexte(texte,texte_texture, renderer, window);

        buttonHover(window, texture_play_hover, renderer, play_button_rect);
        SDL_RenderPresent(renderer);
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        
    }  

    //free window
    destroyAll(window, renderer);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

//gcc src/main.c -o bin/prog src/liste.o -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread