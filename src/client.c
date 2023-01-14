#include "client.h"

#define dataLen 5

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
    char bufferX[3] = "";
    char dataX[4] = "x";
    itoa(rect.x, bufferX, 10);
    strcat(dataX, bufferX);
    //printf("Sended %s\n",dataX);
    dataX[4] = '\0';
    send(*socketServer,dataX,sizeof(sizeof(char)*4+1),0);

    char bufferY[3] = "";
    char dataY[4] = "y";
    itoa(rect.y, bufferY, 10);
    strcat(dataY, bufferY);
    //printf("Sended %s\n",dataY);
    dataY[4] = '\0';
    send(*socketServer,dataY,sizeof(sizeof(char)*4+1),0);

    char bufferW[3] = "";
    char dataW[4] = "w";
    itoa(rect.w, bufferW, 10);
    strcat(dataW, bufferW);
    //printf("Sended %s\n",dataW);
    dataW[4] = '\0';
    send(*socketServer,dataW,sizeof(sizeof(char)*4+1),0);

    char bufferH[3] = "";
    char dataH[4] = "h";
    itoa(rect.h, bufferH, 10);
    strcat(dataH, bufferH);
    //printf("Sended %s\n",dataH);
    dataH[4] = '\0';
    send(*socketServer,dataH,sizeof(sizeof(char)*4+1),0);
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

    //on test la connexion
    connected = connect(*socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    if(connected) printf("Error: connection lost\n");
    else printf("Connected\n");
}

//-lwsock32 -lpthread