/**
 * @file menus.c
 * @author Yusuf Ulas
 * @brief Fichier qui s'occupera de l'affichage des éléments 
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "main.h"
#include "menus.h"

void SDL_ExitWithError(const char *message);

/**
 * @brief Détruit la fenêtre et le rendu. (plus souvent utilisée lors d'une erreur de chargement d'un asset)
 * 
 */
void destroyAll()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/**
 * @brief Commence l'animation souhaitée au joueur demandé.
 * 
 * @param animation 
 * @param joueur 
 */
void startAnimation(char *animation, player_t *joueur)
{
    if(animations_thread_running == FALSE){
        if(strcmp(animation,"running_up") == 0)
            pthread_create(&animations_thread, NULL, running_up_animation,(void *)joueur);  

        else if(strcmp(animation,"running_down") == 0)
            pthread_create(&animations_thread, NULL, running_down_animation,(void *)joueur); 

        else if(strcmp(animation,"running_left") == 0)
            pthread_create(&animations_thread, NULL, running_left_animation,(void *)joueur);  

        else if(strcmp(animation,"running_right") == 0)
            pthread_create(&animations_thread, NULL, running_right_animation,(void *)joueur);  

        else if(strcmp(animation,"breath_animation") == 0)
            pthread_create(&animations_thread, NULL, breathAnimation,(void *)joueur);  
    }
}

/**
 * @brief Change l'état du boutton de survol ciblé.
 * 
 * @param button 
 */
void changeButtonState(char *button)
{
    if(strcmp(button, "connect") == 0)
    {
        if(hover_connectbutton == FALSE) hover_connectbutton = TRUE;
        else hover_connectbutton = FALSE;
    }
    else if(strcmp(button, "host") == 0)
    {
        if(hover_hostbutton == FALSE) hover_hostbutton = TRUE;
        else hover_hostbutton = FALSE;
    }
    else if(strcmp(button, "play") == 0)
    {
        if(hover_playbutton == FALSE) hover_playbutton = TRUE;
        else hover_playbutton = FALSE;
    }
    else if(strcmp(button, "settings") == 0)
    {
        if(hover_settingsbutton == FALSE) hover_settingsbutton = TRUE;
        else hover_settingsbutton = FALSE;
    }
}

/**
 * @brief Renvoi TRUE si le curseur est sur le boutton passé en paramètre sinon renvoi FALSE si le curseur est sur aucun boutton.
 * 
 * @param button 
 * @return int
 */
int onButton(char *button)
{
    if(strcmp(button, "connect") == 0)
        return SDL_PointInRect(&mouse_position, &connect_button_rect);
    else if(strcmp(button, "host") == 0)
        return SDL_PointInRect(&mouse_position, &host_button_rect);
    else if(strcmp(button, "play") == 0)
        return SDL_PointInRect(&mouse_position, &play_button_rect);
    else if(strcmp(button, "settings") == 0)
        return SDL_PointInRect(&mouse_position, &settings_button_rect);
    else return FALSE;
}

/**
 * @brief Essaye de dessiner tous les bouttons.
 * 
 */
void drawButtons()
{
    drawButton(texture_play_inert, play_button_rect, play_inert);
    drawButton(texture_connect_inert, connect_button_rect, connect_inert);
    drawButton(texture_host_inert, host_button_rect, host_inert);
}

/**
 * @brief Met à jour les valeurs de l'écran.
 * 
 */
void update_screen()
{
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    SDL_GetMouseState(&mouse_position.x,&mouse_position.y);
    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_ShowCursor(SDL_DISABLE);
}

/**
 * @brief Dessine le bon curseur en fonction de site on survol un bouton ou pas.
 * 
 */
void drawMouse()
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
void drawButton(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du boutton jouer...");
    }
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
}

/**
 * @brief Dessine un boutton en fonction d'une certaine rotation.
 * 
 * @param texture 
 * @param rectangle 
 * @param surface 
 * @param menuTarget 
 */
void drawButton_withRotation(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface)
{
    if(SDL_RenderCopyEx(renderer, texture, NULL, &rectangle, settings_button_animation_state, NULL, SDL_FLIP_NONE) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de rotate du boutton jouer...");
    }   
}

/**
 * @brief Trie les joueurs en fonction de leur coordonnées Y et organise l'ordre d'affichage des joueurs.
 * 
 */
static void sortPlayers()
{
    int position;
    int i,j;
    for(i = 1; i < size; i++)
    {
        player_t buffer = joueurs[i];
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
void drawPlayer(SDL_Texture *texture_joueur, SDL_Rect playerRect)
{
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
static void switchAnimation(player_t Joueur)
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
void *drawPlayers(player_t *joueurs, int size)
{
    //sortPlayers();
    for(int i = 1; i <= size; i++)
    {
        if(joueurs[i].connected == TRUE)
        {
            switchAnimation(joueurs[i]);
        }
    }
}

void *drawSoloPlayer(player_t joueur)
{

}

/**
 * @brief Animation de respiration
 * 
 * @return void* 
 */
void *breathAnimation(void *j)
{    
    player_t *joueur = j;
    joueur->animation_state = BREATH_START;
    if(!SOLO) Send2Server();
    delay_breath();
    joueur->animation_state++;
    if(!SOLO) Send2Server();
    delay_breath();
    joueur->animation_state--;
    if(!SOLO) Send2Server();
    animations_thread_running = FALSE;
}

/**
 * @brief Animation "courrir à gauche"
 * 
 * @return void* 
 */
void *running_left_animation(void *j)
{
    player_t *joueur = j;
    joueur->animation_state = RUNNING_LEFT_START;
    delay_running_left();
    joueur->animation_state++;
    delay_running_left();
    joueur->animation_state--;
    pthread_exit(&animations_thread);
    animations_thread_running = FALSE;
}

/**
 * @brief Animation "courrir à droite"
 * 
 * @return void* 
 */
void *running_right_animation(void *j)
{
    player_t *joueur = j;
    joueur->animation_state = RUNNING_RIGHT_START;
    delay_running_right();
    joueur->animation_state++;
    delay_running_right();
    joueur->animation_state--;
    pthread_exit(&animations_thread);
    animations_thread_running = FALSE;
}

/**
 * @brief Animation "courrir àen haut"
 * 
 * @return void* 
 */
void *running_up_animation(void *j)
{
    player_t *joueur = j;
    joueur->animation_state = RUNNING_UP_START;
    delay_running_up();
    joueur->animation_state++;
    delay_running_up();
    joueur->animation_state--;
    pthread_exit(&animations_thread);
    animations_thread_running = FALSE;
}

/**
 * @brief Animation "courrir en bas"
 * 
 * @return void* 
 */
void *running_down_animation(void *j)
{
    player_t *joueur = j;
    joueur->animation_state = RUNNING_DOWN_START;
    delay_running_down();
    joueur->animation_state++;
    delay_running_down();
    joueur->animation_state--;
    pthread_exit(&animations_thread);
    animations_thread_running = FALSE;
}

/**
 * @brief Animation du bouton paramètres quand on a pas le curseur dessus.
 * 
 * @return void* 
 */
void *settings_button_animation_left()
{
    for(settings_button_animation_state; settings_button_animation_state >= 0 && hover_settingsbutton == SDL_FALSE; settings_button_animation_state-=10)
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
void *settings_button_animation_right()
{
    for(settings_button_animation_state; settings_button_animation_state < 180 && hover_settingsbutton == SDL_TRUE; settings_button_animation_state+=10)
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
 */
void drawTitle()
{
    if(SDL_QueryTexture(title_texture, NULL, NULL, &title_rect.w,&title_rect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
    }
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
}

/**
 * @brief Affiche l'erreur à l'écran.
 * 
 * @param rect 
 * @param texture 
 */
void drawError(SDL_Rect rect, SDL_Texture *texture)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rect.w,&rect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
    }
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

/**
 * @brief Dessine l'erreur avec TTF.
 * 
 * @param s 
 */
void displayError(char *s)
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
void init_menus_vars()
{
    mouse_position.x = 0, mouse_position.y = 0;
    xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetMouseState(&mouse_position.x,&mouse_position.y);

    joueur.playerRect.x = 0;
    joueur.playerRect.y = 0;
    joueur.playerRect.w = 50;
    joueur.playerRect.h = 81;

    //assets init
    inventory_surface = IMG_Load("resources/inventory.png");;
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
    init_texture(&inventory_surface , &inventory_texture);

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
    settings_button_rect.y = DM.h - settings_button_rect.h - ((DM.h * 3) / 100);

    //inventory 
    inventory_rect.w = 800;
    inventory_rect.h = 350;
    inventory_rect.x = (DM.w / 2) - (inventory_rect.w / 2);
    inventory_rect.y = DM.h - inventory_rect.h - 20;
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
 * @brief Change le mode plein écran.
*/
void toggleFullscreen()
{
    if(fullscreen == SDL_FALSE)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        fullscreen = TRUE;
    }
    else{
        SDL_SetWindowFullscreen(window, 0);
        fullscreen = FALSE;
    }
    Sleep(500);
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
void buttonHover(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button)
{

    if(SDL_PointInRect(&mouse_position, button_rect))
    {
        *hover_button = SDL_TRUE;
        init_hover(hover_button);
        drawButton(button_texture, *button_rect, button_surface);
    }
    else
    {
        *hover_button = SDL_FALSE;
    }
}

/**
 * @brief Affiche la texture de survol avec animation du bouton demandé en dans le menu souhaité.
 * 
 * @param button_surface 
 * @param button_texture 
 * @param button_rect 
 * @param hover_button 
 * @param p 
 * @param p2 
 */
void buttonHoverWithAnimation(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button, void* (*p)(void*), void* (*p2)(void*))
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

/**
 * @brief Dessine l'inventaire.
 * 
 */
void drawInventory()
{
    if(SDL_QueryTexture(inventory_texture, NULL, NULL, &inventory_rect.w, &inventory_rect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture de la balle...");
    }
    SDL_RenderCopy(renderer, inventory_texture, NULL, &inventory_rect);
}

/**
 * @brief Affiche le menu principal
 * 
 */
void mainMenu()
{
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;

    drawButtons();
    buttonHover(play_hover, texture_play_hover, &play_button_rect, &hover_playbutton);
    buttonHover(connect_hover, texture_connect_hover, &connect_button_rect, &hover_connectbutton);
    buttonHover(host_hover, texture_host_hover, &host_button_rect, &hover_hostbutton);
    buttonHoverWithAnimation(settings_inert, texture_settings_inert, &settings_button_rect, &hover_settingsbutton, settings_button_animation_right, settings_button_animation_left);
    drawButton_withRotation(texture_settings_inert, settings_button_rect, settings_inert);
    drawTitle();
    drawMouse();
}

/**
 * @brief Dessine le menu en jeu.
 * 
 */
void IngameMenu()
{
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;
    drawMouse();
}

/**
 * @brief Dessin le menu de l'inventaire.
 * 
 */
void InventoryMenu()
{
    IngameMenu();
    drawInventory();
    drawMouse();
}

/**
 * @brief Dessine le bon menu.
 * 
 */
void drawMenu()
{
    switch (menu)
    {
    case MAIN:
        mainMenu();
        break;
    case INGAME:
        IngameMenu();
        break;
    case INVENTORY:
        InventoryMenu();
        break;
    default:
        break;
    }

    SDL_RenderPresent(renderer);
}

/**
 * @brief Nous renvoi une erreur SDL.
 * 
 * @param message 
 */
static void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}