#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Ws2ipdef.h>   
#include <pthread.h>
#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")
 
typedef uint32_t socklen_t;

typedef struct 
{
    char nom[30];
    int age;
}User;


int main()
{
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

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

    SOCKADDR_IN addrClient;
    socklen_t csize = sizeof(addrClient);
    SOCKET socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
    printf("accept\n");

    printf("client: %d\n",socketClient);

    User user = {
        .nom = "Yusuf",
        .age = 21
    };

    send(socketClient, user.nom, sizeof(user),0);

    close(socketClient);
    close(socketServer);
    printf("close\n");
    
    WSACleanup();
    return 0;
    
}

//-lwsock32