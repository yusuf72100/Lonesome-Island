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
    char *msg;
    User user;
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    SOCKET socketClient;
    SOCKADDR_IN addrClient;
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(4148);
    socketClient = socket(AF_INET,SOCK_STREAM,0);
    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    printf("Connected\n");
    //recv(socketClient, msg, 33 ,0);
    recv(socketClient, (char*)&user, sizeof(User) ,0);

    printf("%s\n",user.nom);
    //scanf("%s%s",user.nom, buffer);
    //send(socketClient,user.nom,sizeof(user.nom),0);
    //send(socketClient,&user.age,sizeof(user),0);

    close(socketClient);

    WSACleanup();
    return 0;
    
}

//-lwsock32