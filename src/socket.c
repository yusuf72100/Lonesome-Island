#ifdef _linux
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#else

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Ws2ipdef.h>   
#include <pthread.h>
#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#define TRUE 1
#define FALSE 0

typedef uint32_t socklen_t;

typedef struct 
{
    char nom[30];
    int age;
}User;

typedef struct 
{
    SOCKET socketServer;
    SOCKET * clientsSockets;
    SOCKADDR_IN addrClient;
    int size;

}socketDatas;

typedef struct 
{
    socketDatas * sd;
    int * running;

}argThread;

void *function(void *arg)
{
    SOCKET socket;
    char *msg = "Quel est votre nom et votre age?";

    User user = {
        .nom = "Yusuf",
        .age = 21
    };

    if(send(socket,user.nom,sizeof(user.nom),0))printf("sended\n");
    //recv(socket,user.nom,sizeof(user.nom),0);
    //recv(socket,&user.age,sizeof(user.age),0);
    //printf("Vous etes %s et vous avez ans\n", user.nom, user.age);
    close(socket);
    pthread_exit(NULL);
}

//fonction qui accepte les clients
void *searchClients(void *argt)
{
    argThread *argt2 = (argThread*)argt;
    socklen_t csize = sizeof(argt2->sd->addrClient);
    SOCKET socketClient;
    *argt2->running = TRUE;
    printf("size : %d\n",argt2->sd->size);

    while(argt2->running)
    {
        socketClient = accept(argt2->sd->socketServer, (struct sockaddr *)&argt2->sd->addrClient, &csize);
        if(socketClient)
        {
            argt2->sd->clientsSockets = realloc(argt2->sd->clientsSockets, sizeof(SOCKET)*(argt2->sd->size+1));
            argt2->sd->size++;
            printf("1 new client connected\n");
            printf("Connected clients : %d\n",argt2->sd->size);
        }
    }

    close(argt2->sd->socketServer);
    printf("close\n");
    WSACleanup();
    pthread_exit(NULL);
}

int main()
{
    int * running = malloc(sizeof(int));
    socketDatas * sd = malloc(sizeof(socketDatas));

    SOCKET * clientsSockets = malloc(sizeof(SOCKET));
    pthread_t clientThread;
    pthread_t acceptClients;
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

    argt.sd->size = 0;
    //if(send(socketClient,user.nom,sizeof(user.nom),0))printf("sended\n");
    /*SOCKET *arg = malloc(sizeof(SOCKET));
    *arg = socketClient;
    pthread_create(&clientThread, NULL, function, arg);*/
    //pthread_create(&acceptClients, NULL, searchClients, (void*)&argt);
    searchClients((void*)&argt);

    //close(socketClient);
    return 0;
    
}

//-lwsock32