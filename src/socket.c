#include "socket.h"

#define TRUE 1
#define FALSE 0

typedef uint32_t socklen_t;

//fonction qui accepte les clients
void *searchClients(void *argt)
{
    argThread *argt2 = (argThread*)argt;
    socklen_t csize = sizeof(argt2->sd->addrClient);
    SOCKET socketClient;
    *argt2->running = TRUE;
    printf("Serveur lance\n");
    while(argt2->running)
    {
        socketClient = accept(argt2->sd->socketServer, (struct sockaddr *)&argt2->sd->addrClient, &csize);
        if(socketClient)
        {
            argt2->sd->clientsSockets = realloc(argt2->sd->clientsSockets, sizeof(SOCKET)*(argt2->sd->size+1));
            argt2->sd->size++;
            printf("1 new client connected\n");
            printf("Connected clients : %d\n",argt2->sd->size);
            send(socketClient,"yo",sizeof(char)*2+1,0);
        }
    }

    close(argt2->sd->socketServer);
    printf("close\n");
    WSACleanup();
    //pthread_exit(NULL);
}

//fonction qui initialise et lance le serveur
void *startServer()
{
    pthread_t acceptThread;
    int * running = malloc(sizeof(int));
    socketDatas * sd = malloc(sizeof(socketDatas));

    SOCKET * clientsSockets = malloc(sizeof(SOCKET));
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    //socket du serveur
    SOCKET socketServer;
    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr("192.168.1.16");
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
    //SOCKET socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);     //fonction bloquante
    //printf("accept\n");
    //printf("client: %d\n",socketClient);

    sd->socketServer = socketServer;
    sd->clientsSockets = clientsSockets;
    sd->addrClient = addrClient;

    argThread argt = {
        .sd = sd,
        .running = running
    };

    //on lance le serveur
    argt.sd->size = 0;
    /*if(pthread_create(&acceptThread,NULL,searchClients,(void*)&argt)) printf("Thread created!\n");

    //on attend l'arrêt du serveur
    char *s_acceptThread;
    pthread_join(acceptThread, (void**)&s_acceptThread);*/
    searchClients((void*)&argt);
    printf("Fin du serveur\n");
}

//-lwsock32 -lpthread