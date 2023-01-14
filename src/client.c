#include "client.h"

#define dataLen 5

static int connected;

char traitData(char data[])
{
    int i;
    char buffer = data[0];
    for(i = 0; data[i]!='\0'; i++){
        data[i] = data[i+1];
    }
    data[i] = '\0';
    return buffer;
}

void *receiveFromServer()
{
    char buffer[4] = "";
    while(TRUE)
    {
        SDL_Rect rect;
        recv(*socketServer,buffer,dataLen+1,0);
        char c = traitData(buffer);
        switch (c)
        {
        case 'x':
            rect.x = atoi(buffer);
            printf("received rect.x : %s\n",buffer);
            break;
        case 'y':
            rect.y = atoi(buffer);
            printf("received rect.y : %d\n",rect.y);
            break;     
        case 'w':
            rect.w = atoi(buffer);
            printf("received rect.w : %d\n",rect.w);
            break;   
        case 'h':
            rect.h = atoi(buffer);
            printf("received rect.y : %d\n",rect.y);
            break;

        default:
            printf("Incorrect data\n");
            break;
        }
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