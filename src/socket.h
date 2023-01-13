#ifndef SOCKET_H
#define SOCKET_H

#ifdef _linux
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define TRUE 1
#define FALSE 0
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
#define TRUE 1
#define FALSE 0
#endif

typedef uint32_t socklen_t;

typedef struct 
{
    char nom[30];
    int age;
}User;

typedef struct 
{
    SOCKET socketServer;
    SOCKET * clientsSockets;
    SOCKADDR_IN addrClient;
    int size;

}socketDatas;

typedef struct 
{
    socketDatas * sd;
    int * running;

}argServer;

//fonction qui accepte les clients
void *searchClients(void *argt);

void *startServer();

#endif