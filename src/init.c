#include "init.h"

/* CE FICHIER CONTIENDRA TOUTES LES DECLARATIONS DE TOUTES LES VARAIBLES DU PROGRAMME */

//memset(tabEvent, 0, 7*sizeof(SDL_bool));

void SDL_ExitWithError(const char *message);

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void destroyAll(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void init_rectangles()
{
    //tank
    rectangletank.x = 0;
    rectangletank.y = 0;
    rectangletank.w = 150;
    rectangletank.h = 150;

    //play button
    play_button_rect.x = 350;
    play_button_rect.y = 250;
    play_button_rect.w = 100;
    play_button_rect.h = 50;

    //title rectangle
    title_rect.x = 200;
    title_rect.y = 100;
    title_rect.w = 200;
    title_rect.h = 200;
}

void initialiser_surfaces()
{
    imagetank = SDL_LoadBMP("tank.bmp");
    imagebullet = SDL_LoadBMP("bullet.bmp");
    background = SDL_LoadBMP("background.bmp");
    play_inert = SDL_LoadBMP("play_inert.bmp");
    play_hover = SDL_LoadBMP("play_hover.bmp");
}

void init_assets()
{
    if(SDL_Init(SDL_INIT_VIDEO != 0))
        SDL_ExitWithError("Initialisation SDL");

    if(SDL_CreateWindowAndRenderer(696, 553, SDL_WINDOW_SHOWN, &window, &renderer) != 0)
        SDL_ExitWithError("Impossible de créer la fenêtre...");

    SDL_SetWindowTitle(window,"Welcome");

    /*if(SDL_SetRenderDrawColor(renderer, 112, 168, 237, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur pour le rendu");*/

    //if(SDL_BlitSurface(texte,NULL,background,&rect_texte))SDL_Quit();

    if (TTF_Init() == -1)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }

    police = TTF_OpenFont("couscousse.ttf", 30);

    if (police == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la police...");
    }

    texte = TTF_RenderText_Blended(police, "Bienvenue sur mon jeu!", blackColor);

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
}