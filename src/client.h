#ifndef CLIENT_H
#define CLIENT_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Ws2ipdef.h>   
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
#pragma comment(lib, "ws2_32.lib")
 
#define TRUE 1
#define FALSE 0
#define dataLen 5
#define THREAD_NULL 0
#define IP_PUBLIC "90.93.215.192"
#define IP_LOCALE "192.168.1.16"

typedef uint32_t socklen_t;

static SOCKET *socket_Server;
static int connected;
static char tramClient_send[100];
static char tramClient_receive[100];

static player joueur;
static player * joueurs;
static int size;

static SDL_Rect *rect = NULL;
static playersRect *p_datas = NULL;

static argDessinerJoueurs * arg = NULL;
static int connected;

typedef struct 
{   
    SDL_Rect rect;
    int rotation;
}argDessinerJoueurs;

typedef struct 
{
    SDL_Rect *rectangles;
    int size;
    
}playersRect;

void *sendDatas(player joueur);

void *stopConnection();

void *startConnection();

void *receiveFromServer();

#endif