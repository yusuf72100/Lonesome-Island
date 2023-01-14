#include "client.h"

static int connected;

void *sendPosition(SDL_Rect rect, int rotation)
{
    char buffer[4] = "";

    itoa(rect.x, buffer, 10);
    printf("sended rect.x : %s\n",buffer);
    send(*socketServer,buffer,sizeof(sizeof(char)*4+1),0);
    itoa(rect.y, buffer, 10);
    send(*socketServer,buffer,sizeof(sizeof(char)*4+1),0);
    itoa(rect.w, buffer, 10);
    send(*socketServer,buffer,sizeof(sizeof(char)*4+1),0);
    itoa(rect.h, buffer, 10);
    send(*socketServer,buffer,sizeof(sizeof(char)*4+1),0);
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
    addrServer.sin_addr.s_addr = inet_addr("90.93.91.79");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    *socketServer = socket(AF_INET,SOCK_STREAM,0);
    connected = connect(*socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    if(connected) printf("Error: connection lost\n");
    else printf("Connected\n");
}

//-lwsock32 -lpthread