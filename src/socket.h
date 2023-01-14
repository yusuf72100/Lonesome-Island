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

typedef uint32_t socklen_t;

static char recvBuffer[dataLen]; 

typedef struct 
{
    char nom[30];
    int age;
}User;

typedef struct 
{
    SOCKET socketServer;
    SOCKET clientSocket;
    SOCKADDR_IN addrClient;
    SDL_Rect rectangle;

}socketDatas;

typedef struct 
{
    socketDatas * sd;
    int running;
    int size;

}argServer;

typedef struct
{
    SOCKET socket;
    argServer * argt;

}send2Client;

//fonction qui accepte les clients
void *searchClients(void *argt);

void *receiveFromClient(void *arg);

void *sendToClient(void *arg);

void *startServer();

char traitData(char data[]);

#endif