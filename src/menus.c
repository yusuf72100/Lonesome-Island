/**
 * @file main.c
 * @author Yusuf Ulas
 * @brief Fichier principal du programme qui s'occupera de la partie graphique
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "main.h"

void SDL_ExitWithError(const char *message);


/**
 * @brief Détruit la fenêtre et le rendu. (plus souvent utilisée lors d'une erreur de chargement d'un asset)
 * 
 * @param window 
 * @param renderer 
 */
static void destroyAll(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/**
 * @brief Dessine le bon curseur en fonction de site on survol un bouton ou pas.
 * 
 */
static void drawMouse()
{
    if(hover_playbutton || hover_connectbutton || hover_hostbutton || hover_settingsbutton) 
    {
        if(SDL_QueryTexture(cursor_select_texture, NULL, NULL, &mouseRect.w, &mouseRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du curseur...");
        }
        SDL_RenderCopy(renderer, cursor_select_texture, NULL, &mouseRect);
    }
    else
    {
        if(SDL_QueryTexture(cursor_texture, NULL, NULL, &mouseRect.w, &mouseRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du curseur...");
        }
        SDL_RenderCopy(renderer, cursor_texture, NULL, &mouseRect);
    }
}

/**
 * @brief Dessine un bouton normal.
 * 
 * @param texture 
 * @param rectangle 
 * @param surface 
 */
static void dessinerButton(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du boutton jouer...");
    }
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
}

/**
 * @brief Trie les joueurs en fonction de leur coordonnées Y et organise l'ordre d'affichage des joueurs.
 * 
 */
static void trierJoueurs()
{
    int position;
    int i,j;
    for(i = 1; i < size; i++)
    {
        player buffer = joueurs[i];
        position = i;
        for(j = i; j < size; j++)
        {
            if(joueurs[i].playerRect.y > joueurs[j].playerRect.y) position++;
        }

        buffer = joueurs[position];
        joueurs[position] = joueurs[i];
        joueurs[i] = buffer;
    }
}

/**
 * @brief Dessine un joueur en fonction de ses coordonnées et de sa texture.
 * 
 * @param texture_joueur 
 * @param playerRect 
 */
static void drawPlayer(SDL_Texture *texture_joueur, SDL_Rect playerRect)
{
    //printf("Player's datas: \nx: %d y:%d\n",playerRect.x, playerRect.y);
    if(SDL_QueryTexture(texture_joueur, NULL, NULL, &playerRect.w,&playerRect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
    }
    SDL_RenderCopy(renderer, texture_joueur, NULL, &playerRect);
}

/**
 * @brief Applique la bonne texture au joueur en fonction de son animation.
 * 
 * @param Joueur 
 */
static void switchAnimation(player Joueur)
{
    switch (Joueur.animation_state)
    {
    case BREATH_START:
        drawPlayer(texture_joueur_h1,Joueur.playerRect);
        break;

    case BREATH_END:
        drawPlayer(texture_joueur_h2,Joueur.playerRect);
        break;

    case RUNNING_LEFT_START:
        drawPlayer(texture_joueur_left_1,Joueur.playerRect);
        break;

    case RUNNING_LEFT_END:
        drawPlayer(texture_joueur_left_2,Joueur.playerRect);
        break;

    case RUNNING_RIGHT_START:
        drawPlayer(texture_joueur_right_1,Joueur.playerRect);
        break;

    case RUNNING_RIGHT_END:
        drawPlayer(texture_joueur_right_2,Joueur.playerRect);
        break;

    case RUNNING_UP_START:
        drawPlayer(texture_joueur_up_1,Joueur.playerRect);
        break;

    case RUNNING_UP_END:
        drawPlayer(texture_joueur_up_2,Joueur.playerRect);
        break;

    case RUNNING_DOWN_START:
        drawPlayer(texture_joueur_down_1,Joueur.playerRect);
        break;

    case RUNNING_DOWN_END:
        drawPlayer(texture_joueur_down_2,Joueur.playerRect);
        break;
    
    default:
        drawPlayer(texture_joueur_h1,Joueur.playerRect);
        break;
    }
}   

/**
 * @brief Dessine tous les joueurs.
 * 
 * @return void* 
 */
static void *dessinerJoueurs()
{
    //trierJoueurs();
    for(int i = 1; i < size; i++)
    {
        if(joueurs[i].connected == TRUE)
            switchAnimation(joueurs[i]);
    }
}


/**
 * @brief Animation de respiration
 * 
 * @return void* 
 */
static void *breathAnimation()
{    
    joueur.animation_state = BREATH_START;
    pthread_create(&sendtoserver,NULL,Send2Server,NULL);
    delay_breath();
    joueur.animation_state++;
    pthread_create(&sendtoserver,NULL,Send2Server,NULL);
    delay_breath();
    joueur.animation_state--;
    pthread_create(&sendtoserver,NULL,Send2Server,NULL);
    pthread_exit(&animations_thread);
}

/**
 * @brief Animation "courrir à gauche"
 * 
 * @return void* 
 */
static void *running_left_animation()
{
    joueur.animation_state = RUNNING_LEFT_START;
    delay_running_left();
    joueur.animation_state++;
    delay_running_left();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

/**
 * @brief Animation "courrir à droite"
 * 
 * @return void* 
 */
static void *running_right_animation()
{
    joueur.animation_state = RUNNING_RIGHT_START;
    delay_running_right();
    joueur.animation_state++;
    delay_running_right();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

/**
 * @brief Animation "courrir àen haut"
 * 
 * @return void* 
 */
static void *running_up_animation()
{
    joueur.animation_state = RUNNING_UP_START;
    delay_running_up();
    joueur.animation_state++;
    delay_running_up();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

/**
 * @brief Animation "courrir en bas"
 * 
 * @return void* 
 */
static void *running_down_animation()
{
    joueur.animation_state = RUNNING_DOWN_START;
    delay_running_down();
    joueur.animation_state++;
    delay_running_down();
    joueur.animation_state--;
    pthread_exit(&animations_thread);
}

/**
 * @brief Dessine le bouton de paramètres (à executer dans un thread uniquement).
 * 
 * @return void* 
 */
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

/**
 * @brief Animation du bouton paramètres quand on a pas le curseur dessus.
 * 
 * @return void* 
 */
static void *settings_button_animation_left()
{
    for(settings_button_animation_state; settings_button_animation_state >= 0 && hover_settingsbutton == SDL_FALSE; settings_button_animation_state--)
    {
        delay_settings_button_left();
    }

    settings_button_animation_state=0;
    animations_thread_running = FALSE;
    pthread_exit(&animations_thread);
}

/**
 * @brief Animation du bouton paramètres quand on a le curseur dessus.
 * 
 * @return void* 
 */
static void *settings_button_animation_right()
{
    for(settings_button_animation_state; settings_button_animation_state < 6 && hover_settingsbutton == SDL_TRUE; settings_button_animation_state++)
    {
        delay_settings_button_right();
    }

    while(hover_settingsbutton == SDL_TRUE);
    animations_thread_running = FALSE;
    pthread_exit(&animations_thread);
}

/**
 * @brief Dessine le titre dans le menu principal
 * 
 * @return void* 
 */
static void drawTitle()
{
    if(SDL_QueryTexture(title_texture, NULL, NULL, &title_rect.w,&title_rect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
    }
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
}

/**
 * @brief Affiche l'erreur à l'écran
 * 
 * @param rect 
 * @param texture 
 */
static void drawError(SDL_Rect rect, SDL_Texture *texture)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rect.w,&rect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
    }
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

/**
 * @brief Initialise les assets de l'erreur dans le menu demandé avec SDL_TTF 
 * @param s
 * @param menuTarget
 * @return void* 
 */
static void displayError(char *s, char *menuTarget)
{
    if(strcmp(menu,menuTarget) == 0)
    {
        SDL_Rect rect;
        rect.x = 0;
        rect.y = (WindowH / 2) - 100;
        rect.w = WindowW;
        rect.h = 200;

        SDL_Rect error;
        error.w = 200;
        error.h = 500;
        error.x = (WindowW / 2) - 315;
        error.y = rect.y + (rect.h / 2) - 15;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_RenderFillRect(renderer, &rect);

        texte = TTF_RenderText_Blended(police, s, blackColor);
        texte_texture = SDL_CreateTextureFromSurface(renderer, texte);
        SDL_BlitSurface(texte,NULL,background,&error);

        drawError(error, texte_texture);
    }
}

/**
 * @brief Initialise l'asset demandé.
 * 
 * @param surface 
 * @param texture 
 */
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

/**
 * @brief Initialise toutes les variables relatives aux menus.
 * 
 */
static void init_vars()
{
    mouse_position.x = 0, mouse_position.y = 0;
    xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetMouseState(&mouse_position.x,&mouse_position.y);

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

    //assets init
    icon_surface = IMG_Load("resources/icon.png");
    imagebullet = IMG_Load("resources/bullet.png");
    cursor = IMG_Load("resources/cursor/cursor.png");
    cursor_select = IMG_Load("resources/cursor/cursor_select.png");
    background = IMG_Load("resources/bg.png");
    play_inert = IMG_Load("resources/play_inert.png");
    play_hover = IMG_Load("resources/play_hover.png");
    connect_inert = IMG_Load("resources/connect_inert.png");
    connect_hover = IMG_Load("resources/connect_hover.png");
    host_inert = IMG_Load("resources/host_inert.png");
    host_hover = IMG_Load("resources/host_hover.png");
    title_surface = IMG_Load("resources/title.png");
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
        SDL_ExitWithError("Failed init SDL");

    if((window = SDL_CreateWindow("Lonesome Island",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_MAXIMIZED)) == NULL)
        SDL_ExitWithError("Impossible de créer la fenêtre...");
    
    if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
        SDL_ExitWithError("Impossible de créer le rendu...");

    SDL_SetWindowIcon(window, icon_surface);
    SDL_GetCurrentDisplayMode(0,&DM);

    //title rectangle
    title_rect.w = 500;
    title_rect.h = 223;
    title_rect.x = (DM.w / 2) - (title_rect.w / 2);
    title_rect.y = 20;

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
    cursor_texture = SDL_CreateTextureFromSurface(renderer, cursor);

    if(cursor == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture de la souris...");
    }
    
    cursor_select_texture = SDL_CreateTextureFromSurface(renderer, cursor_select);

    if(cursor_select == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la texture de la souris...");
    }

    /*texte = TTF_RenderText_Blended(police, "Lonesome Island", blackColor);
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
    */

    //buttons
    init_texture(&title_surface , &title_texture);
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

    //cursor rectangle
    mouseRect.w = 50;
    mouseRect.h = 50;

    //play button
    play_button_rect.w = 400;
    play_button_rect.h = 200;
    play_button_rect.x = (DM.w / 2) - (play_button_rect.w / 2);
    play_button_rect.y = (((DM.h - title_rect.h) - (title_rect.y - 100)) / 3);

    //connect button
    connect_button_rect.w = 500;
    connect_button_rect.h = 250;
    connect_button_rect.x = (DM.w / 2) - (connect_button_rect.w / 2);
    connect_button_rect.y = (((DM.h - title_rect.h) - (title_rect.y - 100)) / 3) + (connect_button_rect.h);

    //host button
    host_button_rect.w = 400;
    host_button_rect.h = 200;
    host_button_rect.x = (DM.w / 2) - (host_button_rect.w / 2);
    host_button_rect.y = (((DM.h - title_rect.h) - (title_rect.y - 100)) / 3) + (connect_button_rect.h*2) + 40;

    //settings button
    settings_button_rect.w = 150;
    settings_button_rect.h = 150;
    settings_button_rect.x = 30;
    settings_button_rect.y = DM.h - settings_button_rect.h - 50;
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

/**
 * @brief Affiche la texture de survol du bouton demandé en dans le menu souhaité.
 * 
 * @param button_surface 
 * @param button_texture 
 * @param button_rect 
 * @param hover_button 
 * @param menuTarget 
 */
static void buttonHover(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button, char *menuTarget)
{
    if(strcmp(menu,menuTarget) == 0)
    {
        if(SDL_PointInRect(&mouse_position, button_rect))
        {
            *hover_button = SDL_TRUE;
            init_hover(hover_button);
            dessinerButton(button_texture, *button_rect, button_surface);
        }
        else
        {
            *hover_button = SDL_FALSE;
        }
    }
}

/**
 * @brief Affiche la texture de survol avec animation du bouton demandé en dans le menu souhaité.
 * 
 * @param button_surface 
 * @param button_texture 
 * @param button_rect 
 * @param hover_button 
 * @param menuTarget 
 */
static void buttonHoverWithAnimation(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button, char *menuTarget, void* (*p)(void*), void* (*p2)(void*))
{

    if(strcmp(menu,menuTarget) == 0)
    {
        if(SDL_PointInRect(&mouse_position, button_rect))
        {
            *hover_button = SDL_TRUE;
            init_hover(hover_button);
            if(animations_thread_running == FALSE){
                animations_thread_running = TRUE;
                pthread_create(&animations_thread, NULL, p, NULL);   
            }
        }
        else
        {
            *hover_button = SDL_FALSE;            
            if(animations_thread_running == FALSE){
                animations_thread_running = TRUE;
                pthread_create(&animations_thread, NULL, p2, NULL);   
            }
        }
    }
}

static void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}