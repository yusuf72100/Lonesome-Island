#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Ws2ipdef.h>   
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

    SOCKET socketClient;
    SOCKADDR_IN addrClient;
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(4148);
    socketClient = socket(AF_INET,SOCK_STREAM,0);
    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    printf("Connecté");

    User user;
    recv(socketClient, user.nom, sizeof(User),0);
    printf("%s %d\n",user.nom, user.age);

    close(socketClient);

    WSACleanup();
    return 0;
    
}

//-lwsock32