#include "client.h"

static int connected;

void *sendPosition(SDL_Rect rectangle, int rotation)
{
    send(*socketServer,(char*)&rectangle.x,sizeof(rectangle),0);
    send(*socketServer,(char*)&rectangle.y,sizeof(rectangle),0);
    send(*socketServer,(char*)&rectangle.w,sizeof(rectangle),0);
    send(*socketServer,(char*)&rectangle.h,sizeof(rectangle),0);
}

void *stopConnection()
{
    closesocket(*socketServer);
    WSACleanup();
    printf("connection closed\n");
}

void *startConnection()
{   
    socketServer = malloc(sizeof(SOCKET));
    char *msg = malloc(sizeof(char)*2+1);
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    *socketServer = socket(AF_INET,SOCK_STREAM,0);
    connected = connect(*socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    printf("Connected\n");
}

//-lwsock32 -lpthread