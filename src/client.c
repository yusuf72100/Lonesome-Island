#include "client.h"

#define dataLen sizeof(char)*4+1

static int connected;

void *receiveFromServer()
{
    char buffer[4] = "";
    while(TRUE)
    {
        SDL_Rect rect;
        recv(*socketServer,buffer,dataLen,0);
        rect.x = atoi(buffer);
        //printf("client received rect.x : %d\n",rect.x);
        /*recv(*socketServer,buffer,dataLen,0);
        rect.y = atoi(buffer);
        printf("client received rect.y : %d\n",rect.y);
        recv(*socketServer,buffer,dataLen,0);
        rect.w = atoi(buffer);
        printf("client received rect.w : %d\n",rect.w);
        recv(*socketServer,buffer,dataLen,0);
        rect.h = atoi(buffer);
        printf("client received rect.h : %d\n",rect.h);*/
    }
}

void *sendPosition(SDL_Rect rect, int rotation)
{
    char buffer[3] = "";
    char data[4] = "x";
    itoa(rect.x, buffer, 10);
    strcat(data, buffer);
    send(*socketServer,data,sizeof(sizeof(char)*3+1),0);
    Sleep(20);

    buffer[0] = '\0';
    data[0] = '\0';
    data[0] = 'y';
    itoa(rect.y, buffer, 10);
    strcat(data, buffer);
    send(*socketServer,data,sizeof(sizeof(char)*3+1),0);
    Sleep(20);

    buffer[0] = '\0';
    data[0] = '\0';
    data[0] = 'w';
    itoa(rect.w, buffer, 10);
    strcat(data, buffer);
    send(*socketServer,data,sizeof(sizeof(char)*3+1),0);
    Sleep(20);

    buffer[0] = '\0';
    data[0] = '\0';
    data[0] = 'h';
    itoa(rect.h, buffer, 10);
    strcat(data, buffer);
    send(*socketServer,data,sizeof(sizeof(char)*3+1),0);
    Sleep(20);
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
    if(connected) printf("Error: connection lost\n");
    else printf("Connected\n");
}

//-lwsock32 -lpthread