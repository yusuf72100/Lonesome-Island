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
 * @brief Change de menu.
 *
 * @param menuTarget
 */
extern void changeMenu(T_MENU menuTarget)
{
    if(menuTarget == MAIN_MENU) SOLO = FALSE;
    menu = menuTarget;
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
 * @brief Change l'état du boutton ciblé.
 * 
 * @param button 
 */
void switchButtonState_hover(T_BUTTONS_HOVER button)
{
    switch (button) {
        case CONNECT_BUTTON_HOVER:
            if(hover_connectbutton == FALSE) hover_connectbutton = TRUE;
            else hover_connectbutton = FALSE;
            break;
        case HOST_BUTTON_HOVER:
            if(hover_hostbutton == FALSE) hover_hostbutton = TRUE;
            else hover_hostbutton = FALSE;
            break;
        case PLAY_BUTTON_HOVER:
            if(hover_playbutton == FALSE) hover_playbutton = TRUE;
            else hover_playbutton = FALSE;
            break;
        case SETTINGS_BUTTON_HOVER:
            if(hover_settingsbutton == FALSE) hover_settingsbutton = TRUE;
            else hover_settingsbutton = FALSE;
            break;
        case SETTINGS_KEYBIND_HOVER:
            if(hover_settings_keybindsbutton == FALSE) hover_settings_keybindsbutton = TRUE;
            else hover_settings_keybindsbutton = FALSE;
            break;
        default:
            break;
    }
}

/**
 * @brief Change le boutton ciblé avec l'état voulut.
 * 
 * @param button 
 * @param state 
 */
void changeButtonState_hover(T_BUTTONS_HOVER button, int state)
{
    switch (button)
    {
        case CONNECT_BUTTON_HOVER:
            hover_connectbutton = state;
            break;
        case HOST_BUTTON_HOVER:
            hover_hostbutton = state;
            break;
        case PLAY_BUTTON_HOVER:
            hover_playbutton = state;
            break;
        case SETTINGS_BUTTON_HOVER:
            hover_settingsbutton = state;
            break;
        case SETTINGS_MAIN_KEYBIND_MENU:
            hover_settings_keybindsbutton = state;
            break;
        case INVENTORY_BUTTON_HOVER:
            hover_inventoryitem = state;
            break;
        default:
            break;
    }
}

/**
 * @brief Change le boutton ciblé avec l'état voulut.
 * 
 * @param button 
 * @param state 
 */
void changeButtonState_clicked(T_BUTTONS_CLICKED button, int state)
{
    switch (button)
    {
        case CONNECT_BUTTON_CLICKED:
            clicked_connectbutton = state;
            break;
        case HOST_BUTTON_CLICKED:
            clicked_hostbutton = state;
            break;
        case PLAY_BUTTON_CLICKED:
            clicked_playbutton = state;
            break;
        case SETTINGS_BUTTON_CLICKED:
            clicked_settingsbutton = state;
            break;
        case SETTINGS_KEYBIND_CLICKED:
            clicked_settings_keybindsbutton = state;
            break;
        case INVENTORY_BUTTON_CLICKED:
            clicked_inventoryitem = state;
            break;
        default:
            break;
    }
}

/**
 * @brief Renvoi l'état du boutton demandé.
 * 
 * @param button 
 * @return int 
 */
int getButtonState_clicked(T_BUTTONS_CLICKED button)
{
    switch (button)
    {
        case CONNECT_BUTTON_CLICKED:
            return clicked_connectbutton;
        case HOST_BUTTON_CLICKED:
            return clicked_hostbutton;
        case PLAY_BUTTON_CLICKED:
            return clicked_playbutton;
        case SETTINGS_BUTTON_CLICKED:
            return clicked_settingsbutton;
        case INVENTORY_BUTTON_CLICKED:
            return clicked_inventoryitem;
        default:
            return FALSE;
    }
}

/**
 * @brief Renvoi TRUE si le curseur est sur le boutton passé en paramètre sinon renvoi FALSE si le curseur est sur aucun boutton.
 * 
 * @param button 
 * @return int
 */
int onButton(T_BUTTONS_HOVER button)
{
    switch (button)
    {
        case CONNECT_BUTTON_HOVER:
            return SDL_PointInRect(&mouse_position, &connect_button_rect);
        case HOST_BUTTON_HOVER:
            return SDL_PointInRect(&mouse_position, &host_button_rect);
        case PLAY_BUTTON_HOVER:
            return SDL_PointInRect(&mouse_position, &play_button_rect);
        case SETTINGS_BUTTON_HOVER:
            return SDL_PointInRect(&mouse_position, &settings_button_rect);
        case SETTINGS_KEYBIND_HOVER:
            return SDL_PointInRect(&mouse_position, &settings_menu_keybinds_button_rect);
        default:
            return FALSE;
    }
}

/**
 * @brief Dessine les bouttons pour configurer les touches de jeu.
 * 
 */
void drawBindButtons()
{
    for(int i = 0; i < 5 ; i++)
    {
        drawButton(texture_settings_menu_key_button, keybinds_buttons[i].rect, surface_settings_menu_key_button);
    }
}

/**
 * @brief Essaye de dessiner tous les bouttons.
 * 
 */
void drawButtons()
{
    switch (menu)
    {
        case MAIN_MENU:
            drawButton(texture_play_inert, play_button_rect, surface_play_inert);
            drawButton(texture_connect_inert, connect_button_rect, surface_connect_inert);
            drawButton(texture_host_inert, host_button_rect, surface_host_inert);
            break;
        case SETTINGS_MAIN_MENU:
            drawButton(texture_settings_menu_keybinds_button, settings_menu_keybinds_button_rect, surface_settings_menu_keybinds_button);
            break;
        case SETTINGS_MAIN_KEYBIND_MENU:
            drawButton(texture_settings_menu_keybinds_button_hover, settings_menu_keybinds_button_rect, surface_settings_menu_keybinds_button);
            drawBindButtons();
            break;
        default:
            break;
        }
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
    //hover cursor
    if(hover_playbutton || hover_connectbutton || hover_hostbutton || hover_settingsbutton || hover_inventoryitem || hover_settings_keybindsbutton || wearingItem)
    {
        if(SDL_QueryTexture(cursor_select_texture, NULL, NULL, &mouseRect.w, &mouseRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du curseur...");
        }
        SDL_RenderCopy(renderer, cursor_select_texture, NULL, &mouseRect);
    }
    //pointeur cursor
    else{
        if(SDL_QueryTexture(cursor_texture, NULL, NULL, &mouseRect.w, &mouseRect.h) != 0)
        {
            destroyAll(window, renderer);
            SDL_ExitWithError("Impossible d'afficher la texture du curseur...");
        }
        SDL_RenderCopy(renderer, cursor_texture, NULL, &mouseRect);
    }
}

/**
 * @brief Initialise les textures des cases de l'inventaire.
 * 
 */
void inventoryInit(case_inventory mat[3][10])
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mat[i][j].Item = apple;
            mat[i][j].number = 0;
        }
    }
    mat[0][0].Item = apple;
    mat[0][0].number = 5;
    mat[2][5].Item = apple;
    mat[2][5].number = 63;
    mat[1][5].Item = chest_item;
    mat[1][5].number = 1;
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
        SDL_ExitWithError("Impossible d'afficher la texture du boutton...");
    }
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
}

/**
 * @brief Affiche le nombre d'items pour chaque case de l'inventaire.
 * 
 * @param case_x 
 */
void drawCaseText(case_inventory case_x)
{
    char buffer[2];
    itoa(case_x.number, buffer, 10);

    case_x.text_rectangle.h = 32;
    case_x.text_rectangle.w = 18 * strlen(buffer);
    case_x.text_rectangle.x = case_x.item_rectangle.x + (case_x.item_rectangle.w/3);
    case_x.text_rectangle.y = case_x.item_rectangle.y + (case_x.item_rectangle.h/3);
    case_x.text_surface = TTF_RenderText_Blended(item_dafont, buffer, whiteColor);

    if (case_x.text_surface == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image des items...");
    }
    case_x.text_texture = SDL_CreateTextureFromSurface(renderer, case_x.text_surface);
    /*if (case_x.text_texture == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image des dafonts des items...");
    }*/
    //SDL_BlitSurface(case_x.text_surface,NULL,case_x.Item->surface,&case_x.item_rectangle);
    SDL_RenderCopy(renderer,case_x.text_texture, NULL, &case_x.text_rectangle);
}

/**
 * @brief Dessine l'item à la bonne case de l'inventaire.
 * 
 * @param item 
 */
void drawItem(case_inventory case_x)
{
    if(SDL_QueryTexture(case_x.Item->texture, NULL, NULL, &case_x.item_rectangle.w, &case_x.item_rectangle.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture de l'item...");
    }
    SDL_RenderCopy(renderer,case_x.Item->texture, NULL, &case_x.item_rectangle);
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

/**
 * @brief Animation de respiration
 * 
 * @return void* 
 */
void *breathAnimation(void *j)
{    
    player_t *joueur = j;
    joueur->animation_state = BREATH_START;
    Send2Server();
    delay_breath();
    joueur->animation_state++;
    Send2Server();
    delay_breath();
    joueur->animation_state--;
    Send2Server();
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
 * @brief Dessine l'image souhaitée
 * 
 */
void drawImage(SDL_Texture *texture, SDL_Rect rect)
{
    if(SDL_QueryTexture(texture, NULL, NULL, &rect.w,&rect.h) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du joueur...");
    }
    SDL_RenderCopy(renderer, texture, NULL, &rect);
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
 * @brief Initialise les surfaces du jeu.
 * 
 */
static void surfacesInit()
{
    //assets init
    lifebar_surface = IMG_Load("resources/life_bar.png");
    inventory_surface = IMG_Load("resources/inventory.png");
    icon_surface = IMG_Load("resources/icon.png");
    imagebullet = IMG_Load("resources/bullet.png");
    cursor = IMG_Load("resources/cursor/cursor.png");
    cursor_select = IMG_Load("resources/cursor/cursor_select.png");
    background = IMG_Load("resources/bg.png");
    surface_play_inert = IMG_Load("resources/play_inert.png");
    surface_play_hover = IMG_Load("resources/play_hover.png");
    surface_connect_inert = IMG_Load("resources/connect_inert.png");
    surface_connect_hover = IMG_Load("resources/connect_hover.png");
    surface_host_inert = IMG_Load("resources/host_inert.png");
    surface_host_hover = IMG_Load("resources/host_hover.png");
    title_surface = IMG_Load("resources/title.png");
    surface_settings_inert = IMG_Load("resources/settings_inert.png");
    surface_settings_bg = IMG_Load("resources/settings_menu_bg.png");
    surface_settings_menu_keybinds_button = IMG_Load("resources/settings_keybind_button.png");
    surface_settings_menu_keybinds_button_hover = IMG_Load("resources/settings_keybind_button_hover.png");
    surface_settings_menu_key_button = IMG_Load("resources/button_keybind.png");

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

    //items 
    apple->surface = IMG_Load("resources/items/apple_food.png");
    chest_item->surface = IMG_Load("resources/items/chest_block.png");
}

/**
 * @brief Initialise les éléments de la fenêtre.
 * 
 */
static void windowInit()
{
    if(SDL_Init(SDL_INIT_EVERYTHING != 0))
        SDL_ExitWithError("Failed init SDL");

    if((window = SDL_CreateWindow("Lonesome Island",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_MAXIMIZED)) == NULL)
        SDL_ExitWithError("Impossible de créer la fenêtre...");

    if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
        SDL_ExitWithError("Impossible de créer le rendu...");

    SDL_SetWindowIcon(window, icon_surface);
    SDL_GetCurrentDisplayMode(0,&DM);
}

/**
 * @brief Initialise les textures du jeu.
 * 
 */
static void texturesInit()
{
    //buttons
    init_texture(&title_surface , &title_texture);
    init_texture(&surface_play_inert , &texture_play_inert);
    init_texture(&surface_play_hover , &texture_play_hover);
    init_texture(&surface_connect_inert , &texture_connect_inert);
    init_texture(&surface_connect_hover , &texture_connect_hover);
    init_texture(&surface_host_inert , &texture_host_inert);
    init_texture(&surface_host_hover , &texture_host_hover);
    init_texture(&surface_settings_inert , &texture_settings_inert);
    init_texture(&inventory_surface , &inventory_texture);
    init_texture(&surface_settings_bg, &texture_settings_bg);
    init_texture(&surface_settings_menu_keybinds_button, &texture_settings_menu_keybinds_button);
    init_texture(&surface_settings_menu_keybinds_button_hover, &texture_settings_menu_keybinds_button_hover);
    init_texture(&surface_settings_menu_key_button, &texture_settings_menu_key_button);

    //game assets
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
    init_texture(&lifebar_surface , &lifebar_texture);

    //items
    init_texture(&((apple->surface)), &((apple->texture)));
    init_texture(&((chest_item->surface)), &((chest_item->texture)));
}

/**
 * @brief Initialise les rectangles.
 * 
 */
static void rectanglesInit()
{
    //player rectangle
    joueur.playerRect.x = 0;
    joueur.playerRect.y = 0;
    joueur.playerRect.w = 50;
    joueur.playerRect.h = 81;

    //title rectangle
    title_rect.w = 500;
    title_rect.h = 223;
    title_rect.x = (DM.w / 2) - (title_rect.w / 2);
    title_rect.y = 20;

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

    //settings menu
    settings_menu_bg_rect.w = 800;
    settings_menu_bg_rect.h = 800;
    settings_menu_bg_rect.x = (DM.w / 2) - (settings_menu_bg_rect.w / 2);
    settings_menu_bg_rect.y = (DM.h / 2) - (settings_menu_bg_rect.h / 2);

    settings_menu_keybinds_button_rect.w = 332;
    settings_menu_keybinds_button_rect.h = 98;
    settings_menu_keybinds_button_rect.x = settings_menu_bg_rect.x + ((settings_menu_bg_rect.w*6)/100);
    settings_menu_keybinds_button_rect.y = settings_menu_bg_rect.y + ((settings_menu_bg_rect.h*20)/100);

    for(int i = 0; i < 5; i++)
    {
        keybinds_buttons[i].rect.w = 215;
        keybinds_buttons[i].rect.h = 90;
        keybinds_buttons[i].rect.x = (settings_menu_bg_rect.x + settings_menu_bg_rect.w) - ((settings_menu_bg_rect.w*5) / 100) - keybinds_buttons[i].rect.w;
        if(i == 0) 
            keybinds_buttons[i].rect.y = settings_menu_bg_rect.y + 125;
        else
            keybinds_buttons[i].rect.y = settings_menu_bg_rect.y + keybinds_buttons[i-1].rect.y;
    }

    //inventory 
    inventory_rect.w = 800;
    inventory_rect.h = 350;
    inventory_rect.x = (DM.w / 2) - (inventory_rect.w / 2);
    inventory_rect.y = DM.h - inventory_rect.h - 20;

    //lifebar
    lifebar_rect.w = 300;
    lifebar_rect.h = 75;
    lifebar_rect.x = 0;
    lifebar_rect.y = DM.h - (lifebar_rect.h*2);
    
}

/**
 * @brief Initialise les dafonts.
 * 
 */
void ttfInit()
{
    if (TTF_Init() == -1)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image...");
    }
    police = TTF_OpenFont("resources/04b30.TTF", 30);

    if (police == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la police des erreurs...");
    }

    item_dafont = TTF_OpenFont("resources/Donitha-Shiny.ttf", 50);

    if (item_dafont == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger la police des items...");
    }
}

/**
 * @brief Initialise toutes les variables relatives aux menus.
 * 
 */
void init_menus_vars()
{
    defineItem();
    inventoryInit(mat_inventory);
    mouse_position.x = 0, mouse_position.y = 0;
    xWindow = 0, yWindow = 0;

    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_GetMouseState(&mouse_position.x,&mouse_position.y);

    surfacesInit();
    windowInit();
    ttfInit();
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

    texturesInit();
    rectanglesInit();
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
 * 
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
    else{
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
    else{
        *hover_button = SDL_FALSE;            
        if(animations_thread_running == FALSE){
            animations_thread_running = TRUE;
            pthread_create(&animations_thread, NULL, p2, NULL);   
        }
    }
}

/**
 * @brief Évalue sur quel item on a cliqué.
 * 
 */
void clickItem()
{
    int i = 0, j = 0;
    int finded = FALSE;

    if(clicked_inventoryitem == TRUE)
    {
        for(i = 0; i < 3 && finded == FALSE; i++) 
        {
            for(j = 0; j < 10 && finded == FALSE; j++) 
            {
                if(wearingItem == SDL_FALSE)
                {
                    if(mat_inventory[i][j].number > 0)
                    {
                        if(SDL_PointInRect(&mouse_position, &mat_inventory[i][j].item_rectangle))
                        {
                            finded = TRUE;
                        }
                    }
                }
                else{
                    if(SDL_PointInRect(&mouse_position, &mat_inventory[i][j].item_rectangle))
                    {
                        finded = TRUE;
                    }
                }
            }
        }
        i--;
        j--;
        if(finded == TRUE)
        {
            if(wearingItem == SDL_FALSE)
            {
                *wearedItem = mat_inventory[i][j];
                mat_inventory[i][j].number = 0;
                wearingItem = SDL_TRUE;
            }
            else{
                if(mat_inventory[i][j].number > 0)
                {
                    if((mat_inventory[i][j].Item->itemType == wearedItem->Item->itemType) && mat_inventory[i][j].number > 0 && mat_inventory[i][j].number < 64 && wearedItem->number > 0 && wearedItem->number < 64)
                    {
                        if(mat_inventory[i][j].number + wearedItem->number <= 64) 
                        {
                            mat_inventory[i][j].number += wearedItem->number;
                            wearedItem->number = 0;
                            wearingItem = FALSE;
                        }
                        else{
                            wearedItem->number = (mat_inventory[i][j].number + wearedItem->number) - 64;
                            mat_inventory[i][j].number = 64;
                        }
                    }
                    else{
                        case_inventory buffer;
                        buffer = *wearedItem;
                        *wearedItem = mat_inventory[i][j];
                        mat_inventory[i][j] = buffer;
                    }
                }
                else{
                    mat_inventory[i][j] = *wearedItem;
                    wearingItem = SDL_FALSE;
                }
            }
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
 * @brief Dessine les cases de l'inventaire
 * 
 */
void drawCases()
{
    int compteur = 0;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mat_inventory[i][j].item_rectangle.x = inventory_rect.x + (80*(j) + 21);
            mat_inventory[i][j].item_rectangle.y = DM.h - (80*((3-i))) - 10;
            mat_inventory[i][j].item_rectangle.h = 80;
            mat_inventory[i][j].item_rectangle.w = 80;

            if(mat_inventory[i][j].number > 0)
            {
                if(SDL_PointInRect(&mouse_position, &mat_inventory[i][j].item_rectangle))
                {
                    compteur++;
                }

                drawItem(mat_inventory[i][j]);
                drawCaseText(mat_inventory[i][j]);
            }
        }
    }
    if(compteur > 0) hover_inventoryitem = SDL_TRUE;
    else hover_inventoryitem = SDL_FALSE;
}

/**
 * @brief Affiche l'item transporté par le curseur du joueur.
 * 
 */
void wearing()
{
    if(wearingItem == SDL_TRUE)
    {
        joueur.stuck = TRUE;
        wearedItem->item_rectangle.x = mouse_position.x-(wearedItem->item_rectangle.w/4);
        wearedItem->item_rectangle.y = mouse_position.y-(wearedItem->item_rectangle.h/4);
        drawItem(*wearedItem);
        drawCaseText(*wearedItem);
    }
    else{
        joueur.stuck = FALSE;
    }
}

/**
 * @brief Dessine la barre de vie du joueur.
 * 
 */
void drawlifeBar()
{
    SDL_Rect rect;
    rect.x = lifebar_rect.x+78;
    rect.y = lifebar_rect.y;
    rect.w = lifebar_rect.w-79 * (joueur.health/100);
    rect.h = lifebar_rect.h-2;
    SDL_SetRenderDrawColor(renderer, 134, 0, 33, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);

    drawImage(lifebar_texture, lifebar_rect);
}

/**
 * @brief Affiche le menu principal
 * 
 */
static void MainMenu()
{
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;
        
    drawButtons();
    buttonHover(surface_play_hover, texture_play_hover, &play_button_rect, &hover_playbutton);
    buttonHover(surface_connect_hover, texture_connect_hover, &connect_button_rect, &hover_connectbutton);
    buttonHover(surface_host_hover, texture_host_hover, &host_button_rect, &hover_hostbutton);
    buttonHoverWithAnimation(surface_settings_inert, texture_settings_inert, &settings_button_rect, &hover_settingsbutton, settings_button_animation_right, settings_button_animation_left);
    drawButton_withRotation(texture_settings_inert, settings_button_rect, surface_settings_inert);
    drawImage(title_texture, title_rect);
    drawMouse();
}

/**
 * @brief Affiche le menu des paramètres dans le menu principal
 * 
 */
static void SettingsMainMenu()
{
    settings_button_animation_state = 0;
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;
    drawImage(texture_settings_bg, settings_menu_bg_rect);
    
    buttonHover(surface_settings_menu_keybinds_button, texture_settings_menu_keybinds_button, &settings_menu_keybinds_button_rect, &hover_settings_keybindsbutton);
    drawButtons();
    drawMouse();
}

/**
 * @brief Affiche le menu des paramètres des keybinds dans le menu principal 
 * 
 */
static void SettingsMainKeybindMenu()
{
    settings_button_animation_state = 0;
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;
    drawImage(texture_settings_bg, settings_menu_bg_rect);
    
    buttonHover(surface_settings_menu_keybinds_button_hover, texture_settings_menu_keybinds_button_hover, &settings_menu_keybinds_button_rect, &hover_settings_keybindsbutton);
    drawButtons();
    drawMouse();
}

/**
 * @brief Dessine le menu en jeu.
 * 
 */
static void IngameMenu()
{
    settings_button_animation_state = 0;
    hover_inventoryitem = FALSE;
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;
    wearing();
    drawlifeBar();
    drawMouse();
}

/**
 * @brief Dessin le menu de l'inventaire.
 * 
 */
static void InventoryMenu()
{
    settings_button_animation_state = 0;
    IngameMenu();
    drawInventory();
    drawCases();
    wearing();
    drawMouse();
}

/**
 * @brief Affiche le menu des paramètres dans en jeu
 * 
 */
static void SettingsInGameMenu()
{
    settings_button_animation_state = 0;
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;

    IngameMenu();
    drawImage(texture_settings_bg, settings_menu_bg_rect);
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
    case MAIN_MENU:
        MainMenu();
        break;
    case INGAME_MENU:
        IngameMenu();
        break;
    case INVENTORY_MENU:
        InventoryMenu();
        break;
    case SETTINGS_MAIN_MENU:
        SettingsMainMenu();
        break;
    case SETTINGS_INGAME_MENU:
        SettingsInGameMenu();
        break;
    case SETTINGS_MAIN_KEYBIND_MENU:
        SettingsMainKeybindMenu();
        break;
    default:
        MainMenu();
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