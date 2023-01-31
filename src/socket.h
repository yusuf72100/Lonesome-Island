#ifndef SOCKET_H
#define SOCKET_H

#ifdef _linux
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#else

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Ws2ipdef.h>   
#include <pthread.h>
#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string.h>
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>

#define TRUE 1
#define FALSE 0
#define dataLen 5
#define IP_LOCALE "192.168.1.16"

typedef uint32_t socklen_t;

static pthread_t *receive_from_client;

static char tramClient_send[100];
static char tramClient_receive[100];

static char recvBuffer[dataLen]; 
static SOCKET socketServer;

typedef struct
{
    SDL_Rect playerRect;
    int animation_state;

}player;


typedef struct 
{
    SOCKET socketServer;
    SOCKET clientSocket;
    SOCKADDR_IN addrClient;
    player joueur;

}socketDatas;

typedef struct 
{
    socketDatas * sd;
    int running;
    int size;

}argServer;


typedef struct send2Client
{
    SOCKET socket;
    int port;
    argServer * argt;
}send2Client;

void *clearInput(SOCKET socketClient);

//fonction qui accepte les clients
void *searchClients(void *argt);

void *receiveFromClient(void *arg);

static void *sendToClient(send2Client *argClient, int position);

void *startServer();

#endif