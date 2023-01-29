#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
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

static Liste *l;

extern int connected;

static short loading;
static short hover_playbutton = FALSE;
static short hover_connectbutton = FALSE;
static short hover_hostbutton = FALSE;
static short hover_settingsbutton = FALSE;
static short debug = FALSE; 

static int settings_button_animation_state = 0;
static int tick=0;
static int size;
static int rotation = 0;
static int xMouse, yMouse;
static int xWindow = 0, yWindow = 0;
static int WindowW = 1920, WindowH = 1080;

static char *s;
static char menu[10] = "Main";

static Vecteur vecteur;
static Bullet *bullet = NULL;

static SDL_bool program_launched = SDL_TRUE;
static SDL_bool tabEvent[9] = {SDL_FALSE};
static SDL_Event event; 

//menu buttons rectangle
static SDL_Rect mouseRect;
static SDL_Rect play_button_rect;
static SDL_Rect connect_button_rect;
static SDL_Rect host_button_rect;
static SDL_Rect settings_button_rect;

//window icon
static SDL_Surface *icon_surface = NULL;
    
//texte assets
static SDL_Surface *texte = NULL;
static SDL_Texture *texte_texture = NULL;
static SDL_Color blackColor = {115, 88, 42};
static TTF_Font *police = NULL;
static SDL_Rect title_rect;

//game assets
static SDL_Surface *imagebullet = NULL;
static SDL_Texture *texturebullet = NULL;	

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
static SDL_Surface *play_inert = NULL;
static SDL_Surface *play_hover = NULL;
static SDL_Surface *connect_inert = NULL;
static SDL_Surface *connect_hover = NULL;
static SDL_Surface *host_inert = NULL;
static SDL_Surface *host_hover = NULL;
static SDL_Surface *settings_inert = NULL;
static SDL_Surface *settings_hover1 = NULL;
static SDL_Surface *settings_hover2 = NULL;
static SDL_Surface *settings_hover3 = NULL;
static SDL_Surface *settings_hover4 = NULL;
static SDL_Surface *settings_hover5 = NULL;
static SDL_Surface *settings_hover6 = NULL;

static SDL_Texture *texture_play_inert = NULL;
static SDL_Texture *texture_play_hover = NULL;	
static SDL_Texture *texture_connect_inert = NULL;
static SDL_Texture *texture_connect_hover = NULL;	
static SDL_Texture *texture_host_inert = NULL;
static SDL_Texture *texture_host_hover = NULL;	
static SDL_Texture *texture_settings_inert = NULL;
static SDL_Texture *texture_settings_hover1 = NULL;
static SDL_Texture *texture_settings_hover2 = NULL;
static SDL_Texture *texture_settings_hover3 = NULL;
static SDL_Texture *texture_settings_hover4 = NULL;
static SDL_Texture *texture_settings_hover5 = NULL;
static SDL_Texture *texture_settings_hover6 = NULL;

//window init
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Surface *mousesurface = NULL;
static SDL_Texture *mousetexture = NULL;

//background 
static SDL_Surface *background = NULL;
static SDL_Texture *background_texture = NULL;

static player joueur;
static player * joueurs;

static pthread_t reloading;
static pthread_t server;
static pthread_t sendtoserver;
static pthread_t receivefromserver;
static pthread_t animations_thread;
static int animations_thread_running = FALSE;

static void SDL_ExitWithError(const char *message);

static void trierJoueurs();

static void checkEvents();

static void doEvents();

static void init_vars();

static void *Send2Server();

Vecteur InitVecteur(int angle, int vitesse);

static void dessinerJoueur(player Joueur);

static void destroyAll(SDL_Window *window, SDL_Renderer *renderer);

static void *rechargement();

void synchDatas(player * Joueurs, int taille);

static void UpdateBullet(Bullet * b);

static void UpdateBulletAll();

char* newLogName();

char* eventTime();

static void dessinerRect(SDL_Rect rectangle, SDL_Renderer *renderer);

static void dessinerButton(SDL_Texture *texture, SDL_Rect rectangle, SDL_Surface *surface);

static void dessinerBalle(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Window *window, Bullet *b, int rotation, int vitesse);

static void initBullet(Bullet * b, int x, int y, int rotation);

static void buttonHover(SDL_Surface *button_surface, SDL_Texture *button_texture, SDL_Rect *button_rect, short *hover_button, char *menuTarget);

static void init_texture(SDL_Surface **surface, SDL_Texture **texture);