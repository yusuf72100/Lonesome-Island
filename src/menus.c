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
#include "render.h"
#include "player.h"
#include "camera.h"

SDL_Texture *background_texture = NULL;

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
extern void startAnimation(char *animation, player_t *joueur)
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
extern void switchButtonState_hover(T_BUTTONS_HOVER button)
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
        case SETTINGS_KEYBINDS_HOVER:
            if(hover_settings_keybindsbutton == FALSE) hover_settings_keybindsbutton = TRUE;
            else hover_settings_keybindsbutton = FALSE;
            break;
        case SETTINGS_KEYBIND_UP_HOVER:
            if(hover_keybind_up == FALSE) hover_keybind_up = TRUE;
            else hover_keybind_up = FALSE;
            break;
        case SETTINGS_KEYBIND_DOWN_HOVER:
            if(hover_keybind_down == FALSE) hover_keybind_down = TRUE;
            else hover_keybind_down = FALSE;
            break;
        case SETTINGS_KEYBIND_LEFT_HOVER:
            if(hover_keybind_left == FALSE) hover_keybind_left = TRUE;
            else hover_keybind_left = FALSE;
            break;
        case SETTINGS_KEYBIND_RIGHT_HOVER:
            if(hover_keybind_right == FALSE) hover_keybind_right = TRUE;
            else hover_keybind_right = FALSE;
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
extern void changeButtonState_hover(T_BUTTONS_HOVER button, int state)
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
        case SETTINGS_KEYBIND_UP_HOVER:
            hover_keybind_up = state;
            break;
        case SETTINGS_KEYBIND_DOWN_HOVER:
            hover_keybind_up = state;
            break;
        case SETTINGS_KEYBIND_LEFT_HOVER:
            hover_keybind_up = state;
            break;
        case SETTINGS_KEYBIND_RIGHT_HOVER:
            hover_keybind_up = state;
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
extern void changeButtonState_clicked(T_BUTTONS_CLICKED button, int state)
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
        case SETTINGS_KEYBINDS_CLICKED:
            clicked_settings_keybindsbutton = state;
            break;
        case SETTINGS_KEYBIND_UP_CLICKED:
            clicked_settings_keybind_up = state;
            break;
        case SETTINGS_KEYBIND_DOWN_CLICKED:
            clicked_settings_keybind_up = state;
            break;
        case SETTINGS_KEYBIND_LEFT_CLICKED:
            clicked_settings_keybind_left = state;
            break;
        case SETTINGS_KEYBIND_RIGHT_CLICKED:
            clicked_settings_keybind_right = state;
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
extern int getButtonState_clicked(T_BUTTONS_CLICKED button)
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
        case SETTINGS_KEYBINDS_CLICKED:
            return clicked_settings_keybindsbutton;
        case SETTINGS_KEYBIND_UP_CLICKED:
            return clicked_settings_keybind_up;
        case SETTINGS_KEYBIND_DOWN_CLICKED:
            return clicked_settings_keybind_down;
        case SETTINGS_KEYBIND_LEFT_CLICKED:
            return clicked_settings_keybind_left;
        case SETTINGS_KEYBIND_RIGHT_CLICKED:
            return clicked_settings_keybind_right;
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
extern int onButton(T_BUTTONS_HOVER button)
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
        case SETTINGS_KEYBINDS_HOVER:
            return SDL_PointInRect(&mouse_position, &settings_menu_keybinds_button_rect);
        case SETTINGS_KEYBIND_UP_HOVER:
                return SDL_PointInRect(&mouse_position, &settings_menu_keybind_up_rect);
        case SETTINGS_KEYBIND_DOWN_HOVER:
            return SDL_PointInRect(&mouse_position, &settings_menu_keybind_down_rect);
        case SETTINGS_KEYBIND_LEFT_HOVER:
            return SDL_PointInRect(&mouse_position, &settings_menu_keybind_left_rect);
        case SETTINGS_KEYBIND_RIGHT_HOVER:
            return SDL_PointInRect(&mouse_position, &settings_menu_keybind_right_rect);
        default:
            return FALSE;
    }
}

/**
 * @brief Dessine les bouttons pour configurer les touches de jeu.
 *
 */
static void drawBindButtons()
{
    drawButton(texture_settings_menu_key_button, settings_menu_keybind_up_rect, surface_settings_menu_key_button);
    drawButton(texture_settings_menu_key_button, settings_menu_keybind_down_rect, surface_settings_menu_key_button);
    drawButton(texture_settings_menu_key_button, settings_menu_keybind_left_rect, surface_settings_menu_key_button);
    drawButton(texture_settings_menu_key_button, settings_menu_keybind_right_rect, surface_settings_menu_key_button);
}

/**
 * @brief Essaye de dessiner tous les bouttons.
 *
 */
static void drawButtons()
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
        case SETTINGS_INGAME_MENU:
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
extern void update_screen()
{
    renderMap(&renderer, currentGround);
    renderUtils(&renderer, currentUtils);
    SDL_GetMouseState(&mouse_position.x,&mouse_position.y);
    SDL_GetWindowPosition(window, &xWindow, &yWindow);
    SDL_ShowCursor(SDL_DISABLE);
}

/**
 * @brief Dessine le bon curseur en fonction de site on survol un bouton ou pas.
 *
 */
static void drawMouse()
{
    //hover cursor
    if(hover_playbutton || hover_connectbutton || hover_hostbutton || hover_settingsbutton || hover_inventoryitem || hover_settings_keybindsbutton || hover_keybind_up || hover_keybind_down || hover_keybind_left || hover_keybind_right || wearingItem)
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
static void inventoryInit(case_inventory mat[3][10])
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
static void drawButton(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface)
{
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
}

/**
 * @brief Affiche le nombre d'items pour chaque case de l'inventaire.
 *
 * @param case_x
 */
static void drawCaseText(case_inventory case_x)
{
    char buffer[2];
    itoa(case_x.number, buffer, 10);

    case_x.text_rectangle.h = 32;
    case_x.text_rectangle.w = 18 * strlen(buffer);
    case_x.text_rectangle.x = case_x.item_rectangle.x + case_x.item_rectangle.w - case_x.text_rectangle.h;
    case_x.text_rectangle.y = case_x.item_rectangle.y ;
    case_x.text_surface = TTF_RenderText_Blended(item_dafont, buffer, whiteColor);

    if (case_x.text_surface == NULL)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de charger l'image des items...");
    }
    case_x.text_texture = SDL_CreateTextureFromSurface(renderer, case_x.text_surface);
    SDL_RenderCopy(renderer,case_x.text_texture, NULL, &case_x.text_rectangle);
}

/**
 * @brief Dessine l'item à la bonne case de l'inventaire.
 *
 * @param case_x
 */
static void drawItem(case_inventory case_x)
{
    SDL_RenderCopy(renderer, case_x.Item->texture, NULL, &case_x.item_rectangle);
}

/**
 * @brief Dessine un boutton en fonction d'une certaine rotation.
 *
 * @param texture
 * @param rectangle
 * @param surface
 */
static void drawButton_withRotation(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface)
{
    if(SDL_RenderCopyEx(renderer, texture, NULL, &rectangle, settings_button_animation_state, NULL, SDL_FLIP_NONE) != 0)
    {
        destroyAll(window, renderer);
        SDL_ExitWithError("Impossible de rotate du boutton jouer...");
    }
}

/**
 * @brief Dessine tous les joueurs.
 * @param joueurs
 * @param size
 * @return
 */
void *drawPlayers(player_t *joueurs, int size)
{
    //sortPlayers();
    for(int i = 1; i <= size; i++)
    {
        if(joueurs[i].connected == TRUE)
        {
            //renderPlayer(&renderer, NULL, &joueurs[i]);
            //switchAnimation(joueurs[i]);
        }
    }
}

/**
 * Animation de respiration
 * @param j
 * @return
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
static void drawImage(SDL_Texture *texture, SDL_Rect rect)
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
 * @brief Dessine l'erreur avec TTF.
 *
 * @param s
 */
extern void displayError(char *s)
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = (DM.h / 2) - 100;
    rect.w = DM.w;
    rect.h = 200;

    SDL_Rect error;
    error.w = 200;
    error.h = 500;
    error.x = (DM.w / 2) - 315;
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
    thirstbar_surface = IMG_Load("resources/thirst_bar.png");
    lifebar_surface = IMG_Load("resources/life_bar.png");
    inventory_surface = IMG_Load("resources/inventory.png");
    icon_surface = IMG_Load("resources/icon.png");
    imagebullet = IMG_Load("resources/bullet.png");
    cursor = IMG_Load("resources/cursor/cursor.png");
    cursor_select = IMG_Load("resources/cursor/cursor_select.png");
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
    surface_settings_menu_key_button_hover = IMG_Load("resources/button_keybind_hover.png");
    surface_keybind_waiting = IMG_Load("resources/button_keybind_waiting.png");

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
    bois->surface = IMG_Load("ressources/items/wooden_log.png");
    apple->surface = IMG_Load("resources/items/apple.png");
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
    init_texture(&surface_settings_menu_key_button_hover, &texture_settings_menu_key_button_hover);
    init_texture(&surface_keybind_waiting, &texture_keybind_waiting);

    //game assets
    //init_texture(&map_surface , &map_texture);
    //init_texture(&background , &background_texture);
    init_texture(&lifebar_surface , &lifebar_texture);
    init_texture(&thirstbar_surface , &thirstbar_texture);

    //items
    init_texture(&((apple->surface)), &((apple->texture)));
    init_texture(&((bois->surface)), &((bois->texture)));
    init_texture(&((chest_item->surface)), &((chest_item->texture)));
}

/**
 * @brief Initialise les rectangles.
 *
 */
static void rectanglesInit()
{

    //title rectangle
    title_rect.w = DM.w * 0.3;
    title_rect.h = DM.h * 0.2;
    title_rect.x = (DM.w / 2) - (title_rect.w / 2);
    title_rect.y = 20;

    //cursor rectangle
    mouseRect.w = 50;
    mouseRect.h = 50;

    //play button
    play_button_rect.w = DM.w * 0.2;
    play_button_rect.h = DM.h * 0.2;
    play_button_rect.x = (DM.w / 2) - (play_button_rect.w / 2);
    play_button_rect.y = (((DM.h - title_rect.h) - (title_rect.y - 100)) / 3);

    //connect button
    connect_button_rect.w = DM.w * 0.2;
    connect_button_rect.h = DM.h * 0.2;
    connect_button_rect.x = (DM.w / 2) - (connect_button_rect.w / 2);
    connect_button_rect.y = (((DM.h - title_rect.h) - (title_rect.y - 100)) / 3) + (connect_button_rect.h);

    //host button
    host_button_rect.w = DM.w * 0.2;
    host_button_rect.h = DM.h * 0.2;
    host_button_rect.x = (DM.w / 2) - (host_button_rect.w / 2);
    host_button_rect.y = (((DM.h - title_rect.h) - (title_rect.y - 100)) / 3) + (connect_button_rect.h*2) + 40;

    //settings button
    settings_button_rect.w = DM.w * 0.08;
    settings_button_rect.h = DM.w * 0.08;
    settings_button_rect.x = 30;
    settings_button_rect.y = DM.h - settings_button_rect.h - ((DM.h * 3) / 100);

    //settings menu
    settings_menu_bg_rect.w = DM.w * 0.4;
    settings_menu_bg_rect.h = DM.h * 0.7;
    settings_menu_bg_rect.x = (DM.w / 2) - (settings_menu_bg_rect.w / 2);
    settings_menu_bg_rect.y = (DM.h / 2) - (settings_menu_bg_rect.h / 2);

    settings_menu_keybinds_button_rect.w = DM.w * 0.13;
    settings_menu_keybinds_button_rect.h = DM.h * 0.1;
    settings_menu_keybinds_button_rect.x = settings_menu_bg_rect.x + ((settings_menu_bg_rect.w*6)/100);
    settings_menu_keybinds_button_rect.y = settings_menu_bg_rect.y + ((settings_menu_bg_rect.h*20)/100);

    settings_menu_keybind_right_rect.w = DM.w * 0.1;
    settings_menu_keybind_right_rect.h = DM.h * 0.1;
    settings_menu_keybind_right_rect.x = (settings_menu_bg_rect.x + settings_menu_bg_rect.w) - ((settings_menu_bg_rect.w*5) / 100) - settings_menu_keybind_right_rect.w;
    settings_menu_keybind_right_rect.y = settings_menu_bg_rect.h - ((settings_menu_bg_rect.h * 2) / 100);

    settings_menu_keybind_left_rect.w = DM.w * 0.1;
    settings_menu_keybind_left_rect.h = DM.h * 0.1;
    settings_menu_keybind_left_rect.x = (settings_menu_bg_rect.x + settings_menu_bg_rect.w) - ((settings_menu_bg_rect.w*5) / 100) - settings_menu_keybind_left_rect.w;
    settings_menu_keybind_left_rect.y = settings_menu_keybind_right_rect.y - (settings_menu_bg_rect.h / 5);

    settings_menu_keybind_down_rect.w = DM.w * 0.1;
    settings_menu_keybind_down_rect.h = DM.h * 0.1;
    settings_menu_keybind_down_rect.x = (settings_menu_bg_rect.x + settings_menu_bg_rect.w) - ((settings_menu_bg_rect.w*5) / 100) - settings_menu_keybind_down_rect.w;
    settings_menu_keybind_down_rect.y = settings_menu_keybind_left_rect.y - (settings_menu_bg_rect.h / 5);

    settings_menu_keybind_up_rect.w = DM.w * 0.1;
    settings_menu_keybind_up_rect.h = DM.h * 0.1;
    settings_menu_keybind_up_rect.x = (settings_menu_bg_rect.x + settings_menu_bg_rect.w) - ((settings_menu_bg_rect.w*5) / 100) - settings_menu_keybind_up_rect.w;
    settings_menu_keybind_up_rect.y = settings_menu_keybind_down_rect.y - (settings_menu_bg_rect.h / 5);

    keybind_waiting_rect.w = DM.w * 0.1;
    keybind_waiting_rect.h = DM.h * 0.1;
    keybind_waiting_rect.x = 0;
    keybind_waiting_rect.y = 0;

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

    //thirstbar
    thirstbar_rect.w = DM.w * 0.1;
    thirstbar_rect.h = DM.h * 0.2;
    thirstbar_rect.x = 0;
    thirstbar_rect.y = 0;

}

/**
 * @brief Initialise les dafonts.
 *
 */
static void ttfInit()
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
extern void init_menus_vars()
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

/**
 * @brief Change le mode plein écran.
 *
 */
extern void toggleFullscreen()
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
static void buttonHover(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button)
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
static void buttonHoverWithAnimation(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button, void* (*p)(void*), void* (*p2)(void*))
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
extern void clickItem()
{
    int i = 0, j = 0;
    int founded = FALSE;

    if(clicked_inventoryitem == TRUE)
    {
        for(i = 0; i < 3 && founded == FALSE; i++)
        {
            for(j = 0; j < 10 && founded == FALSE; j++)
            {
                if(wearingItem == SDL_FALSE)
                {
                    if(mat_inventory[i][j].number > 0)
                    {
                        if(SDL_PointInRect(&mouse_position, &mat_inventory[i][j].item_rectangle))
                        {
                            founded = TRUE;
                        }
                    }
                }
                else{
                    if(SDL_PointInRect(&mouse_position, &mat_inventory[i][j].item_rectangle))
                    {
                        founded = TRUE;
                    }
                }
            }
        }
        i--;
        j--;
        if(founded == TRUE)
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
static void drawInventory()
{
    SDL_RenderCopy(renderer, inventory_texture, NULL, &inventory_rect);
}

/**
 * @brief Dessine les cases de l'inventaire
 *
 */
static void drawCases()
{
    int compteur = 0;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mat_inventory[i][j].item_rectangle.h = 80;
            mat_inventory[i][j].item_rectangle.w = 80;
            mat_inventory[i][j].item_rectangle.x = inventory_rect.x + (mat_inventory[i][j].item_rectangle.w * j) + 3;
            mat_inventory[i][j].item_rectangle.y = (inventory_rect.y + inventory_rect.h) + ((mat_inventory[i][j].item_rectangle.h * (i - 3)));

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
static void wearing()
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
 * @brief Dessine la barre de soif du joueur.
 *
 */
static void drawThirstBar()
{
    SDL_Rect rect;
    rect.x = thirstbar_rect.x+78;
    rect.y = thirstbar_rect.y;
    rect.w = thirstbar_rect.w-79 * (joueur.health/100);
    rect.h = thirstbar_rect.h-2;
    SDL_SetRenderDrawColor(renderer, 134, 0, 33, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);

    drawImage(thirstbar_texture, thirstbar_rect);
}

/**
 * @brief Dessine la barre de vie du joueur.
 *
 */
static void drawlifeBar()
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
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

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
    SDL_RenderCopy(renderer, texture_settings_bg, NULL, &settings_menu_bg_rect);

    buttonHover(surface_settings_menu_keybinds_button, texture_settings_menu_keybinds_button, &settings_menu_keybinds_button_rect, &hover_settings_keybindsbutton);
    drawButtons();
    drawMouse();
}

/**
 * @brief Affiche un texte en ttf.
 * @param rect
 * @param texture
 * @param surface
 * @param text
 */
static void drawText(SDL_Rect *rect, SDL_Texture *texture, SDL_Surface *surface, char *text, SDL_Color color)
{
    surface = TTF_RenderText_Blended(police, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, NULL, rect);
}

/**
 * @brief Affiche un seul caractère en ttf.
 * @param rect
 * @param texture
 * @param surface
 * @param c
 */
static void drawChar(SDL_Rect *rect, SDL_Texture *texture, SDL_Surface *surface, char c, SDL_Color color)
{
    surface = TTF_RenderGlyph_Blended(police, c, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(SDL_QueryTexture(texture, NULL, NULL, &rect->w,&rect->h) != 0)
    {
        destroyAll(texture, renderer);
        SDL_ExitWithError("Impossible d'afficher la texture du boutton de keybind...");
    }
    SDL_RenderCopy(renderer, texture, NULL, rect);
}

/**
 * @brief Dessine le texte du menu keybind
 *
 */
static void drawKeybindMenuText()
{
    SDL_Texture *keybind_texte_texture = NULL;
    SDL_Surface *keybind_texte_surface = NULL;
    SDL_Rect up_text_rect;
    SDL_Rect down_text_rect;
    SDL_Rect left_text_rect;
    SDL_Rect right_text_rect;

    SDL_Rect keybind_key_up_text = {
        .w = DM.w*0.05,
        .h = DM.h*0.05,
        .x = (settings_menu_keybind_up_rect.x + (settings_menu_keybind_up_rect.w / 2)) - 20,
        .y = settings_menu_keybind_up_rect.y + ((settings_menu_keybind_up_rect.h / 2) - 25)
    };
    SDL_Rect keybind_key_down_text = {
        .w = DM.w*0.05,
        .h = DM.h*0.05,
        .x = (settings_menu_keybind_down_rect.x + (settings_menu_keybind_down_rect.w / 2)) - 20,
        .y = settings_menu_keybind_down_rect.y + ((settings_menu_keybind_down_rect.h / 2) - 25)
    };
    SDL_Rect keybind_key_left_text = {
        .w = DM.w*0.05,
        .h = DM.h*0.05,
        .x = (settings_menu_keybind_left_rect.x + (settings_menu_keybind_left_rect.w / 2)) - 20,
        .y = settings_menu_keybind_left_rect.y + ((settings_menu_keybind_left_rect.h / 2) - 25)
    };
    SDL_Rect keybind_key_right_text = {
        .w = DM.w*0.05,
        .h = DM.h*0.05,
        .x = (settings_menu_keybind_right_rect.x + (settings_menu_keybind_right_rect.w / 2)) - 20,
        .y = settings_menu_keybind_right_rect.y + ((settings_menu_keybind_right_rect.h / 2) - 25)
    };

    up_text_rect.w = 100;
    up_text_rect.h = 50;
    up_text_rect.x = settings_menu_keybind_up_rect.x - 105;
    up_text_rect.y = settings_menu_keybind_up_rect.y + (settings_menu_keybind_up_rect.h/2) - (up_text_rect.h/2);

    down_text_rect.w = 120;
    down_text_rect.h = 50;
    down_text_rect.x = settings_menu_keybind_down_rect.x - 125;
    down_text_rect.y = settings_menu_keybind_down_rect.y + (settings_menu_keybind_down_rect.h/2) - (down_text_rect.h/2);

    left_text_rect.w = 120;
    left_text_rect.h = 50;
    left_text_rect.x = settings_menu_keybind_left_rect.x - 125;
    left_text_rect.y = settings_menu_keybind_left_rect.y + (settings_menu_keybind_left_rect.h/2) - (left_text_rect.h/2);

    right_text_rect.w = 120;
    right_text_rect.h = 50;
    right_text_rect.x = settings_menu_keybind_right_rect.x - 130;
    right_text_rect.y = settings_menu_keybind_right_rect.y + (settings_menu_keybind_right_rect.h/2) - (right_text_rect.h/2);

    drawText(&up_text_rect, keybind_texte_texture, keybind_texte_surface, "Up", whiteColor);
    drawText(&down_text_rect, keybind_texte_texture, keybind_texte_surface, "Down", whiteColor);
    drawText(&left_text_rect, keybind_texte_texture, keybind_texte_surface, "Left", whiteColor);
    drawText(&right_text_rect, keybind_texte_texture, keybind_texte_surface, "Right", whiteColor);

    //keybind key text
    drawChar(&keybind_key_up_text, keybind_texte_texture, keybind_texte_surface, bindButtonText[0], whiteColor);
    drawChar(&keybind_key_left_text, keybind_texte_texture, keybind_texte_surface, bindButtonText[1], whiteColor);
    drawChar(&keybind_key_down_text, keybind_texte_texture, keybind_texte_surface, bindButtonText[2], whiteColor);
    drawChar(&keybind_key_right_text, keybind_texte_texture, keybind_texte_surface, bindButtonText[3], whiteColor);

    SDL_BlitSurface(keybind_texte_surface,NULL,background,&up_text_rect);
    SDL_BlitSurface(keybind_texte_surface,NULL,background,&down_text_rect);
    SDL_BlitSurface(keybind_texte_surface,NULL,background,&left_text_rect);
    SDL_BlitSurface(keybind_texte_surface,NULL,background,&right_text_rect);
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

    if((int) (SDL_GetTicks() - animationDelay) > 200) {
        nextAnimationState(&joueur);
        animationDelay = SDL_GetTicks();
    }

    renderMap(&renderer, currentGround);
    renderPlayer(&renderer, camera, &joueur);
    renderUtils(&renderer, currentUtils);

    wearing();
    drawlifeBar();
    //drawThirstBar();
    drawMouse();
}

/**
 * @brief Affiche le menu des paramètres des keybinds dans le menu principal
 *
 */
static void SettingsMainKeybindMenu()
{
    pthread_t newthread;
    settings_button_animation_state = 0;
    mouseRect.x = mouse_position.x;
    mouseRect.y = mouse_position.y;

    if(SOLO == TRUE || HOST == TRUE)
    {
        IngameMenu();
    }
    SDL_RenderCopy(renderer, texture_settings_bg, NULL, &settings_menu_bg_rect);
    drawButtons();

    //draw waiting button and text
    switch (KEYBOARD_WAITING) {
        case SETTINGS_KEYBIND_UP_CLICKED:
            keybind_waiting_rect.x = settings_menu_keybind_up_rect.x;
            keybind_waiting_rect.y = settings_menu_keybind_up_rect.y;
            drawKeybindMenuText();
            drawButton(texture_keybind_waiting, keybind_waiting_rect, surface_keybind_waiting);
            break;
        case SETTINGS_KEYBIND_DOWN_CLICKED:
            keybind_waiting_rect.x = settings_menu_keybind_down_rect.x;
            keybind_waiting_rect.y = settings_menu_keybind_down_rect.y;
            drawKeybindMenuText();
            drawButton(texture_keybind_waiting, keybind_waiting_rect, surface_keybind_waiting);
            break;
        case SETTINGS_KEYBIND_LEFT_CLICKED:
            keybind_waiting_rect.x = settings_menu_keybind_left_rect.x;
            keybind_waiting_rect.y = settings_menu_keybind_left_rect.y;
            drawKeybindMenuText();
            drawButton(texture_keybind_waiting, keybind_waiting_rect, surface_keybind_waiting);
            break;
        case SETTINGS_KEYBIND_RIGHT_CLICKED:
            keybind_waiting_rect.x = settings_menu_keybind_right_rect.x;
            keybind_waiting_rect.y = settings_menu_keybind_right_rect.y;
            drawKeybindMenuText();
            drawButton(texture_keybind_waiting, keybind_waiting_rect, surface_keybind_waiting);
            break;
        case NONE:
            //try button hover
            buttonHover(surface_settings_menu_keybinds_button_hover, texture_settings_menu_keybinds_button_hover, &settings_menu_keybinds_button_rect, &hover_settings_keybindsbutton);
            buttonHover(surface_settings_menu_key_button_hover, texture_settings_menu_key_button_hover, &settings_menu_keybind_up_rect, &hover_keybind_up);
            buttonHover(surface_settings_menu_key_button_hover, texture_settings_menu_key_button_hover, &settings_menu_keybind_down_rect, &hover_keybind_down);
            buttonHover(surface_settings_menu_key_button_hover, texture_settings_menu_key_button_hover, &settings_menu_keybind_left_rect, &hover_keybind_left);
            buttonHover(surface_settings_menu_key_button_hover, texture_settings_menu_key_button_hover, &settings_menu_keybind_right_rect, &hover_keybind_right);
            drawKeybindMenuText();
            break;
        default:
            break;
    }
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
    SettingsMainMenu();
}

/**
 * @brief Affiche le menu d'affectation du joueur.playername
 */
static void getPlayerNameMenu()
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = (DM.h / 2) - 100;
    rect.w = DM.w;
    rect.h = 200;

    SDL_Rect texte_rect = {
        .w = strlen(joueur.playername) * 50,
        .h = 75,
        .x = (DM.w / 2) - (strlen(joueur.playername) * 25),
        .y = rect.y + (rect.h / 2) - 15
    };

    SDL_Rect title_rect = {
        .w = DM.w * 0.1,
        .h = DM.h * 0.05,
        .x = (DM.w / 2) - 400,
        .y = DM.h - ((DM.h * 80) / 100)
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    SDL_Surface *surface_title = NULL;
    SDL_Texture *texture_title = NULL;

    drawText(&texte_rect, texture, surface, joueur.playername, blackColor);
    drawText(&title_rect, texture_title, surface_title, "Enter your nick:", whiteColor);
    SDL_BlitSurface(surface,NULL,background,&texte_rect);
}

/**
 * @brief Dessine le bon menu.
 * 
 */
extern void drawMenu()
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
    case GET_PLAYERNAME_MENU:
        getPlayerNameMenu();
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