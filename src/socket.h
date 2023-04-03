/**
 * @file socket.h
 * @author Yusuf Ulas
 * @brief Fichier qui contient les informations relatives aux clients et au serveur point de vu socket
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Ws2ipdef.h>   
#include <pthread.h>
#include <stdint.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "player.h"
#pragma comment(lib, "ws2_32.lib")

#define dataLen 5

typedef uint32_t socklen_t;

static pthread_t *receive_from_client;

static char tramClient_send[100];
static char tramClient_receive[100];

static char recvBuffer[dataLen]; 
static SOCKET socketServer;

static int max_player = 10;

/**
 * @brief Sauvegarde les informations de chaque joueurs relatives au socket et à la connection au serveur (adresse ip, port, données de jeu, socket...).
 * 
 */
typedef struct 
{
    SOCKET socketServer;
    SOCKET clientSocket;
    SOCKADDR_IN addrClient;
    player_t joueur;
}socketDatas;

/**
 * @brief Rassemble toutes les données de tous les joueurs dans le but de pouvoir communiquer tout cela dans des threads.
 * 
 */
typedef struct 
{
    socketDatas * sd;
    int running;
    int size;
}argServer;

static argServer *argt = NULL;

/**
 * @brief Rassemble les données d'un seul joueur.
 * 
 */
typedef struct send2Client
{
    SOCKET socket;
    int port;
    argServer * argt;
}send2Client;

char *getLocalIp();
static send2Client *argClient = NULL;
static int findPosition(send2Client *argClient);
void *searchClients(void *argt);
void *receiveFromClient(void *arg);
static void *sendToClient(send2Client *argClient, int position);
void *startServer();
void stopServer();

#endif