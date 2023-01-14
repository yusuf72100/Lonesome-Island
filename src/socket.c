#include "socket.h"

static void *clearInput(void *arg)
{
    SOCKET * socketClient = (SOCKET *)&arg;

    recv(*socketClient,recvBuffer,dataLen,0);
    recv(*socketClient,recvBuffer,dataLen,0);
    recv(*socketClient,recvBuffer,dataLen,0);
    recv(*socketClient,recvBuffer,dataLen,0);
}

void *sendToClient(void *arg)
{
    SOCKET * socketClient = (SOCKET *)&arg;
}

//fonction qui se lance dans un thread
void *receiveFromClient(void *arg)
{
    int i;
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
    pthread_t send_to_client;
    argServer *argt2 = (argServer*)argt;
    socklen_t csize = sizeof(argt2->sd->addrClient);
    SOCKET socketClient;
    argt2->running = TRUE;
    printf("Server running\n");
    
    while(argt2->running)
    {
        socketClient = accept(argt2->sd->socketServer, (struct sockaddr *)&argt2->sd->addrClient, &csize);
        if(socketClient != INVALID_SOCKET)
        {
            argt2->sd = realloc(argt2->sd, sizeof(socketDatas)*(argt2->size+1));
            argt2->size++;
            printf("1 new client connected\n");
            printf("Connected clients : %d\n",argt2->size);
        }
        pthread_create(&receive_from_client,NULL,receiveFromClient,(void *)socketClient);
        //pthread_create(&send_to_client,NULL,sendToClient,(void *)socketClient);
    }

    printf("close\n");
    close(argt2->sd->socketServer);
    WSACleanup();
}

//fonction qui initialise et lance le serveur
void *startServer()
{
    pthread_t receiveThread;
    int  running = 0;
    socketDatas * sd = malloc(sizeof(socketDatas));

    SOCKET clientSocket;
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

    argServer argt = {
        .sd = sd,
        .running = running,
        .size = 0
    };

    //socket des clients
    SOCKADDR_IN addrClient;
    socklen_t csize = sizeof(addrClient);

    argt.sd->socketServer = socketServer;
    argt.sd->clientSocket = clientSocket;
    argt.sd->addrClient = addrClient;

    //on lance et attend l'arrêt du serveur
    searchClients((void*)&argt);
    printf("Fin du serveur\n");
}

//-lwsock32 -lpthread