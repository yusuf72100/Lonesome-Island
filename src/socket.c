#include "socket.h"

void *clearInput(SOCKET socketClient)
{
    recv(socketClient,recvBuffer,dataLen,0);
    recv(socketClient,recvBuffer,dataLen,0);
    recv(socketClient,recvBuffer,dataLen,0);
    recv(socketClient,recvBuffer,dataLen,0);
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

//envoi les données aux clients
void *sendToClient(void *arg)
{
    int err;
    int i = 1, j = 1;
    send2Client *argClient = (send2Client *)arg;
    SDL_Rect rect;
    SOCKADDR_IN addr_Client;
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    //à tout le monde
    do
    {
        //printf("destinataire %d\n",i);
        //on envoi les coordonnées de tout le monde
        do
        {
            char taille[2] = "";
            char dataS[4] = "s";
            itoa(argClient->argt->size, taille, 10);
            strcat(dataS, taille);
            send(argClient->socket,dataS,sizeof(sizeof(char)*2),0);
            
            char bufferX[3] = "";
            char dataX[4] = "x";
            itoa(argClient->argt->sd[j].rectangle.x, bufferX, 10);
            strcat(dataX, bufferX);
            //printf("Sended %d j: %d et size: %d\n",argClient->argt->sd[1].rectangle.x,j,argClient->argt->size);
            dataX[4] = '\0';
            if(argClient->socket == INVALID_SOCKET) printf("Error: INVALID SOCKET\n");
            if(send(argClient->socket,dataX,sizeof(sizeof(char)*4+1),0) == SOCKET_ERROR ) {
                printf("Error: SOCKET_ERROR\n");
                err = WSAGetLastError();
                printf("%d\n",err);
            }
            
            char bufferY[3] = "";
            char dataY[4] = "y";
            itoa(argClient->argt->sd[j].rectangle.y, bufferY, 10);
            strcat(dataY, bufferY);
            //printf("Sended %s\n",dataY);
            dataY[4] = '\0';
            send(argClient->socket,dataY,sizeof(sizeof(char)*4+1),0);

            char bufferW[3] = "";
            char dataW[4] = "w";
            itoa(argClient->argt->sd[j].rectangle.w, bufferW, 10);
            strcat(dataW, bufferW);
            //printf("Sended %s\n",dataW);
            dataW[4] = '\0';
            send(argClient->socket,dataW,sizeof(sizeof(char)*4+1),0);

            char bufferH[3] = "";
            char dataH[4] = "h";
            itoa(argClient->argt->sd[j].rectangle.h, bufferH, 10);
            strcat(dataH, bufferH);
            //printf("Sended %s\n",dataH);
            dataH[4] = '\0';
            send(argClient->socket,dataH,sizeof(sizeof(char)*4+1),0);
            send(argClient->socket,"over",sizeof(sizeof(char)*4+1),0);
            j++;
        } while(j < argClient->argt->size);
        j=1;
        send(argClient->socket,"end",sizeof(sizeof(char)*4+1),0);
        i++;
    } while (i < argClient->argt->size);
    i=1;
}

//fonction qui se lance dans un thread
void *receiveFromClient(void *arg)
{
    pthread_t send_to_client;
    int i = 0;
    char buffer[6] = "";
    send2Client *argClient = (send2Client *)arg;
    SDL_Rect rect;

    if(argClient->socket != INVALID_SOCKET) printf("Ready to receive\n");
    
    //on récupère les données de positions des joueurs
    while(argClient->argt->running == TRUE)
    {
        Sleep(5);
        recv(argClient->socket,buffer,sizeof(sizeof(char)*4),0);
        //printf("pure data : %s\n",buffer);
        char c = traitData(buffer);
        //printf("transformed data : %s\n",buffer);
        switch (c)
        {
        case 'x':
            rect.x = atoi(buffer);
            printf("received rect.x : %d\n",rect.x);
            break;
        case 'y':
            rect.y = atoi(buffer);
            //printf("received rect.y : %d\n",rect.y);
            break;     
        case 'w':
            rect.w = atoi(buffer);
            //printf("received rect.w : %d\n",rect.w);
            break;   
        case 'h':
            rect.h = atoi(buffer);
            //printf("received rect.y : %d\n",rect.y);
            break;
        default:
            //printf("Incorrect data %c\n",c);
            break;
        }

        //on stock les données
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&argClient->socket;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        socklen_t len = sizeof(*pV4Addr);

        //on cherche la position du client dans le tableau de données
        do
        {
            i++;
        } while (strcmp(inet_ntoa(argClient->argt->sd[i].addrClient.sin_addr),inet_ntoa(ipAddr)) || ((int)ntohs(argClient->argt->sd[i].addrClient.sin_port) != argClient->port));

        //printf("find at position %d\n",i);
        argClient->argt->sd[i].rectangle.x = rect.x;
        argClient->argt->sd[i].rectangle.y = rect.y;
        argClient->argt->sd[i].rectangle.w = rect.w;
        argClient->argt->sd[i].rectangle.h = rect.h;
        //pthread_create(&send_to_client,NULL,sendToClient,(void *)argClient);
        sendToClient((void *)argClient);
        i=0;
    }
}

//fonction qui accepte les clients
void *searchClients(void *arg)
{
    pthread_t receive_from_client;
    argServer *argt2 = (argServer*)arg;
    socklen_t csize = sizeof(argt2->sd->addrClient);
    socketDatas * newSd = argt2->sd;
    SOCKET socketClient;
    argt2->running = TRUE;
    
    printf("Server running\n");
    
    while(argt2->running)
    {
        socketClient = accept(argt2->sd->socketServer, (struct sockaddr *)&argt2->sd[argt2->size].addrClient, &csize);
        if(socketClient != INVALID_SOCKET)
        {
            struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&argt2->sd[argt2->size].addrClient;
            struct in_addr ipAddr = pV4Addr->sin_addr;
            argt2->sd[argt2->size].socketServer = socketServer;
            printf("1 new client connected with ip %s and port %d\n",inet_ntoa(argt2->sd[argt2->size-1].addrClient.sin_addr), (int)ntohs(argt2->sd[argt2->size-1].addrClient.sin_port));
            printf("Connected clients : %d\n",argt2->size-1);
            argt2->size++;
        }
        send2Client *argClient = malloc(sizeof(send2Client));
        argClient->socket = socketClient;
        argClient->port = (int)ntohs(argt2->sd[argt2->size-1].addrClient.sin_port);
        argClient->argt = argt2;
        Sleep(500);
        pthread_create(&receive_from_client,NULL,receiveFromClient,(void *)argClient);
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
    socketDatas * sd = malloc(sizeof(socketDatas)*5);

    SOCKET clientSocket;
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    //socket du serveur
    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr(IP_LOCALE);    
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    socketServer = socket(AF_INET,SOCK_STREAM,0);

    bind(socketServer, (SOCKADDR *)&addrServer, sizeof(addrServer));
    printf("bind : %d\n", socketServer);

    listen(socketServer, 5);
    printf("Listening\n");

    argServer *argt = malloc(sizeof(argServer));
    argt->sd = sd;
    argt->running = running;
    argt->size = 1;
    //socket des clients
    SOCKADDR_IN addrClient;
    socklen_t csize = sizeof(addrClient);

    argt->sd->socketServer = socketServer;
    argt->sd->clientSocket = clientSocket;
    argt->sd->addrClient = addrClient;

    //on lance et attend l'arrêt du serveur
    searchClients((void*)argt);
    printf("Fin du serveur\n");
}

//-lwsock32 -lpthread