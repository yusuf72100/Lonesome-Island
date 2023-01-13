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
#include <SDL.h>  
#include <SDL_image.h>
#include <SDL_ttf.h>
#pragma comment(lib, "ws2_32.lib")
 
typedef uint32_t socklen_t;

static SOCKET *socketServer;
static int connected;

void *sendPosition(SDL_Rect rectangle, int rotation);

void *stopConnection();

void *startConnection();

#endif