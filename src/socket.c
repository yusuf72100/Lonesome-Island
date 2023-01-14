#include "socket.h"

static void *clearInput(SOCKET socketClient)
{
    recv(socketClient,recvBuffer,dataLen,0);
    recv(socketClient,recvBuffer,dataLen,0);
    recv(socketClient,recvBuffer,dataLen,0);
    recv(socketClient,recvBuffer,dataLen,0);
}

void *sendToClient(void *arg)
{
    int i = 0;
    char buffer[4] = "";
    send2Client *argClient = (send2Client *)arg;
    SDL_Rect rect;
    SOCKADDR_IN addr_Client;
    SDL_Rect tab[10];

    while(argClient->argt->running == TRUE)
    {
        do
        {
            struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
            struct in_addr ipAddr = pV4Addr->sin_addr;

            if(strcmp(inet_ntoa(argClient->argt->sd[i].addrClient.sin_addr),inet_ntoa(addr_Client.sin_addr)) && ((int)ntohs(argClient->argt->sd[i].addrClient.sin_port) != (int)ntohs(addr_Client.sin_port)))
            {
                itoa(argClient->argt->sd[1].rectangle.x, buffer, 10);
                printf("server sended rect.x : %s\n",buffer);
                send(argClient->socket,buffer,sizeof(sizeof(char)*3+1),0);
                itoa(argClient->argt->sd[1].rectangle.y, buffer, 10);
                printf("server sended rect.y : %s\n",buffer);
                send(argClient->socket,buffer,sizeof(sizeof(char)*3+1),0);
                itoa(argClient->argt->sd[1].rectangle.w, buffer, 10);
                send(argClient->socket,buffer,sizeof(sizeof(char)*3+1),0);
                itoa(argClient->argt->sd[1].rectangle.h, buffer, 10);
                send(argClient->socket,buffer,sizeof(sizeof(char)*3+1),0);
                Sleep(500);
            }
            i++;
        } while (i < argClient->argt->size);
        i=0;
    }
}

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

//fonction qui se lance dans un thread
void *receiveFromClient(void *arg)
{
    int i = 0;
    send2Client *argClient = (send2Client *)arg;
    SDL_Rect rect;
    SOCKADDR_IN addr_Client;

    if(argClient->socket != INVALID_SOCKET) printf("Ready to receive\n");

    //on récupère les données de positions des joueurs
    while(argClient->argt->running == TRUE)
    {
        recv(argClient->socket,recvBuffer,dataLen+1,0);
        char c = traitData(recvBuffer);
        switch (c)
        {
        case 'x':
            rect.x = atoi(recvBuffer);
            printf("received rect.x : %s\n",recvBuffer);
            break;
        case 'y':
            rect.y = atoi(recvBuffer);
            printf("received rect.y : %d\n",rect.y);
            break;     
        case 'w':
            rect.w = atoi(recvBuffer);
            printf("received rect.w : %d\n",rect.w);
            break;   
        case 'h':
            rect.h = atoi(recvBuffer);
            printf("received rect.y : %d\n",rect.y);
            break;

        default:
            printf("Incorrect data\n");
            break;
        }

        //on clear l'input
        clearInput(argClient->socket);

        //on stock les données
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
        struct in_addr ipAddr = pV4Addr->sin_addr;

        do
        {
            i++;
        } while (strcmp(inet_ntoa(argClient->argt->sd[i].addrClient.sin_addr),inet_ntoa(addr_Client.sin_addr)) && ((int)ntohs(argClient->argt->sd[i].addrClient.sin_port) != (int)ntohs(addr_Client.sin_port)));

        printf("find at position %d\n",i);
        argClient->argt->sd[i].rectangle.x = rect.x;
        argClient->argt->sd[i].rectangle.y = rect.y;
        argClient->argt->sd[i].rectangle.w = rect.w;
        argClient->argt->sd[i].rectangle.h = rect.h;
        i=0;
    }
}

//fonction qui accepte les clients
void *searchClients(void *arg)
{
    pthread_t receive_from_client;
    pthread_t send_to_client;
    argServer *argt2 = (argServer*)arg;
    socklen_t csize = sizeof(argt2->sd->addrClient);
    SOCKET socketClient;
    argt2->running = TRUE;

    printf("Server running\n");
    
    while(argt2->running)
    {
        socketClient = accept(argt2->sd->socketServer, (struct sockaddr *)&argt2->sd->addrClient, &csize);
        if(socketClient != INVALID_SOCKET)
        {
            struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&argt2->sd->addrClient;
            struct in_addr ipAddr = pV4Addr->sin_addr;
            argt2->sd = realloc(argt2->sd, sizeof(socketDatas)*(argt2->size+1));
            argt2->size++;
            argt2->sd->clientSocket = socketClient;
            argt2->sd->socketServer = argt2->sd->socketServer;
            printf("1 new client connected with ip %s and port %d\n",inet_ntoa(argt2->sd->addrClient.sin_addr), (int)ntohs(argt2->sd->addrClient.sin_port));
            printf("Connected clients : %d\n",argt2->size);
        }

        send2Client *argClient = malloc(sizeof(send2Client));
        argClient->socket = socketClient;
        argClient->argt = argt2;

        pthread_create(&receive_from_client,NULL,receiveFromClient,(void *)argClient);
        pthread_create(&send_to_client,NULL,sendToClient,(void *)argClient);
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
    addrServer.sin_addr.s_addr = inet_addr("192.168.1.16");
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