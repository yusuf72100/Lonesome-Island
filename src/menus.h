/**
 * @file menus.h
 * @author Yusuf Ulas
 * @brief Fichier contenant toutes les variables d'affichage
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "liste.h"
#include "client.h"
#include <windows.h>

//animations states
#define BREATH_START 1
#define BREATH_END 2
#define RUNNING_LEFT_START 3
#define RUNNING_LEFT_END 4
#define RUNNING_RIGHT_START 5
#define RUNNING_RIGHT_END 6
#define RUNNING_UP_START 7
#define RUNNING_UP_END 8
#define RUNNING_DOWN_START 9
#define RUNNING_DOWN_END 10

static int animations_thread_running = FALSE;
static int settings_button_animation_state = 0;
static int xWindow = 0, yWindow = 0;
static int WindowW = 1920, WindowH = 1080;

static Liste *l;

static SDL_Point mouse_position;   

static SDL_bool hover_playbutton = SDL_FALSE;
static SDL_bool hover_connectbutton = SDL_FALSE;
static SDL_bool hover_hostbutton = SDL_FALSE;
static SDL_bool hover_settingsbutton = SDL_FALSE;
static SDL_bool hover_settings_keybindsbutton = SDL_FALSE;
static SDL_bool hover_inventoryitem = SDL_FALSE;
static SDL_bool clicked_playbutton = SDL_FALSE;
static SDL_bool clicked_connectbutton = SDL_FALSE;
static SDL_bool clicked_hostbutton = SDL_FALSE;
static SDL_bool clicked_settingsbutton = SDL_FALSE;
static SDL_bool clicked_settings_keybindsbutton = SDL_FALSE;
static SDL_bool clicked_inventoryitem = SDL_FALSE;
static SDL_bool fullscreen = SDL_FALSE;
static SDL_bool wearingItem = SDL_FALSE;

//menu buttons rectangle
static SDL_Rect mouseRect;
static SDL_Rect play_button_rect;
static SDL_Rect connect_button_rect;
static SDL_Rect host_button_rect;
static SDL_Rect settings_button_rect;
static SDL_Rect inventory_rect;
static SDL_Rect lifebar_rect;
static SDL_Rect title_rect;
static SDL_Rect settings_menu_bg_rect;
static SDL_Rect settings_menu_keybinds_button_rect;

static SDL_DisplayMode DM;

//window icon
static SDL_Surface *icon_surface = NULL;
    
//texte assets
static SDL_Color blackColor = {0, 0, 0};
static SDL_Color whiteColor = {239, 198, 155};
static TTF_Font *police = NULL;
static TTF_Font *item_dafont = NULL;

//game assets
static SDL_Surface *lifebar_surface = NULL;
static SDL_Surface *inventory_surface = NULL;
static SDL_Surface *imagebullet = NULL;
static SDL_Surface *texte = NULL;
static SDL_Texture *texte_texture = NULL;
static SDL_Surface *title_surface = NULL;
static SDL_Surface *surface_joueur_h1 = NULL;
static SDL_Surface *surface_joueur_h2 = NULL;
static SDL_Surface *surface_joueur_left_1 = NULL;
static SDL_Surface *surface_joueur_left_2 = NULL;
static SDL_Surface *surface_joueur_right_1 = NULL;
static SDL_Surface *surface_joueur_right_2 = NULL;
static SDL_Surface *surface_joueur_up_1 = NULL;
static SDL_Surface *surface_joueur_up_2 = NULL;
static SDL_Surface *surface_joueur_down_1 = NULL;
static SDL_Surface *surface_joueur_down_2 = NULL;

//items surfaces
static SDL_Surface *apple_food_surface = NULL;

//items textures
static SDL_Texture *apple_food_texture = NULL;

//items rectangles
static SDL_Rect apple_food_rect;

static SDL_Texture *lifebar_texture = NULL;
static SDL_Texture *inventory_texture = NULL;
static SDL_Texture *texturebullet = NULL;	
static SDL_Texture *title_texture = NULL;
static SDL_Texture *texture_joueur_h1 = NULL;
static SDL_Texture *texture_joueur_h2 = NULL;
static SDL_Texture *texture_joueur_left_1 = NULL;
static SDL_Texture *texture_joueur_left_2 = NULL;
static SDL_Texture *texture_joueur_right_1 = NULL;
static SDL_Texture *texture_joueur_right_2 = NULL;
static SDL_Texture *texture_joueur_up_1 = NULL;
static SDL_Texture *texture_joueur_up_2 = NULL;
static SDL_Texture *texture_joueur_down_1 = NULL;
static SDL_Texture *texture_joueur_down_2 = NULL;

//menu assets
static SDL_Surface *surface_play_inert = NULL;
static SDL_Surface *surface_play_hover = NULL;
static SDL_Surface *surface_connect_inert = NULL;
static SDL_Surface *surface_connect_hover = NULL;
static SDL_Surface *surface_host_inert = NULL;
static SDL_Surface *surface_host_hover = NULL;
static SDL_Surface *surface_settings_inert = NULL;
static SDL_Surface *surface_settings_bg = NULL;
static SDL_Surface *surface_settings_menu_keybinds_button = NULL;
static SDL_Surface *surface_settings_menu_keybinds_button_hover = NULL;

static SDL_Texture *texture_play_inert = NULL;
static SDL_Texture *texture_play_hover = NULL;	
static SDL_Texture *texture_connect_inert = NULL;
static SDL_Texture *texture_connect_hover = NULL;	
static SDL_Texture *texture_host_inert = NULL;
static SDL_Texture *texture_host_hover = NULL;	
static SDL_Texture *texture_settings_inert = NULL;
static SDL_Texture *texture_settings_bg = NULL;
static SDL_Texture *texture_settings_menu_keybinds_button = NULL;
static SDL_Texture *texture_settings_menu_keybinds_button_hover = NULL;

//window init
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Surface *cursor = NULL;
static SDL_Surface *cursor_select = NULL;
static SDL_Texture *cursor_texture = NULL;
static SDL_Texture *cursor_select_texture = NULL;

//background 
static SDL_Surface *background = NULL;
static SDL_Texture *background_texture = NULL;

//static functions
static void surfacesInit();
static void SDL_ExitWithError(const char *message);
static void sortPlayers();
static void dessinerBalle(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, Bullet *b, int rotation, int vitesse);
static void init_texture(SDL_Surface **surface, SDL_Texture **texture);
static void switchAnimation(player_t Joueur);
static void windowInit();
static void texturesInit();
static void rectanglesInit();

//global functions
void switchButtonState_hover(T_BUTTONS_HOVER button);
void changeButtonState_hover(T_BUTTONS_HOVER button, int state);
void changeButtonState_clicked(T_BUTTONS_CLICKED button, int state);
int getButtonState_clicked(T_BUTTONS_CLICKED button);
void startAnimation(char *animation, player_t *joueur);
int onButton(T_BUTTONS_HOVER button);
void toggleFullscreen();
void drawButtons();
void drawMenu();
void mainMenu();
void update_screen();
void init_menus_vars();
void drawMouse();
void drawPlayer(SDL_Texture *texture_joueur, SDL_Rect playerRect);
void destroyAll();
void drawButton(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface);
void buttonHover(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button);
void buttonHoverWithAnimation(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, SDL_bool *hover_button, void* (*p)(void*), void* (*p2)(void*));
void displayError(char *s);
void drawError(SDL_Rect rect, SDL_Texture *texture);
void drawImage(SDL_Texture *texture, SDL_Rect rect);
void *settings_button_animation_right();
void *settings_button_animation_left();
void *running_down_animation(void *j);
void *running_up_animation(void *j);
void *running_right_animation(void *j);
void *running_left_animation(void *j);
void *breathAnimation(void *j);
void *drawPlayers(player_t *joueurs, int size);
void clickItem();

#endif