#include "socket.h"

static void *clearInput(void *arg)
{
    SOCKET * socketClient = (SOCKET *)&arg;

    recv(*socketClient,recvBuffer,dataLen,0);
    recv(*socketClient,recvBuffer,dataLen,0);
    recv(*socketClient,recvBuffer,dataLen,0);
    recv(*socketClient,recvBuffer,dataLen,0);
}

//fonction qui se lance dans un thread
void *receiveDatas(void *arg)
{
    SOCKET * socketClient = (SOCKET *)&arg;
    SDL_Rect rect;

    if(*socketClient != INVALID_SOCKET) printf("Ready to receive\n");

    //on récupère les données de positions des joueurs
    while(TRUE)
    {
        recv(*socketClient,recvBuffer,dataLen,0);
        rect.x = atoi(recvBuffer);
        printf("received rect.x : %d\n",rect.x);
        recv(*socketClient,recvBuffer,dataLen,0);
        rect.y = atoi(recvBuffer);
        recv(*socketClient,recvBuffer,dataLen,0);
        rect.w = atoi(recvBuffer);
        recv(*socketClient,recvBuffer,dataLen,0);
        rect.h = atoi(recvBuffer);

        //on clear l'input
        clearInput(arg);
    }
}

//fonction qui accepte les clients
void *searchClients(void *argt)
{
    pthread_t receive_from_client;
    argServer *argt2 = (argServer*)argt;
    socklen_t csize = sizeof(argt2->sd->addrClient);
    SOCKET socketClient;
    *argt2->running = TRUE;
    printf("Server running\n");
    while(argt2->running)
    {
        socketClient = accept(argt2->sd->socketServer, (struct sockaddr *)&argt2->sd->addrClient, &csize);
        if(socketClient != INVALID_SOCKET)
        {
            argt2->sd->clientsSockets = realloc(argt2->sd->clientsSockets, sizeof(SOCKET)*(argt2->sd->size+1));
            argt2->sd->size++;
            printf("1 new client connected\n");
            printf("Connected clients : %d\n",argt2->sd->size);
        }
        //receiveDatas((void *)socketClient);
        pthread_create(&receive_from_client,NULL,receiveDatas,(void *)socketClient);
        //else printf("Error: client connection lost\n");
    }

    close(argt2->sd->socketServer);
    printf("close\n");
    WSACleanup();
}

//fonction qui initialise et lance le serveur
void *startServer()
{
    pthread_t receiveThread;
    int * running = malloc(sizeof(int));
    socketDatas * sd = malloc(sizeof(socketDatas));

    SOCKET * clientsSockets = malloc(sizeof(SOCKET));
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    //socket du serveur
    SOCKET socketServer;
    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    socketServer = socket(AF_INET,SOCK_STREAM,0);

    bind(socketServer, (SOCKADDR *)&addrServer, sizeof(addrServer));
    printf("bind : %d\n", socketServer);

    listen(socketServer, 5);
    printf("Listening\n");

    //socket des clients
    SOCKADDR_IN addrClient;
    socklen_t csize = sizeof(addrClient);

    sd->socketServer = socketServer;
    sd->clientsSockets = clientsSockets;
    sd->addrClient = addrClient;

    argServer argt = {
        .sd = sd,
        .running = running
    };

    argt.sd->size = 0;

    //on lance et attend l'arrêt du serveur
    searchClients((void*)&argt);
    printf("Fin du serveur\n");
}

//-lwsock32 -lpthread