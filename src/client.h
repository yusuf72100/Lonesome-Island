/**
 * @file client.h
 * @author Yusuf Ulas
 * @brief Fichier content toutes les informations relatives au client et au serveur 
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

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
#include "socket.h"
#pragma comment(lib, "ws2_32.lib")
 
#define dataLen 5
#define THREAD_NULL 0
#define IP_PUBLIC "90.93.114.53"
#define IP_LOCALE "192.168.1.16"

typedef uint32_t socklen_t;

static SOCKET *socket_Server;
static char tramClient_send[100];
static char tramClient_receive[100];

static player joueur;
static player * joueurs;
static int size;

void *sendDatas(player joueur);

void *receiveFromServer();

void stopConnection();

int startConnection();

#endif