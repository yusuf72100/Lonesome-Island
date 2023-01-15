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
    int err;
    int i = 1;
    send2Client *argClient = (send2Client *)arg;
    SDL_Rect rect;
    SOCKADDR_IN addr_Client;
    SDL_Rect tab[10];
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    while(argClient->argt->running == TRUE)
    {
        do
        {

            if(strcmp(inet_ntoa(argClient->argt->sd[i].addrClient.sin_addr),inet_ntoa(addr_Client.sin_addr)) && ((int)ntohs(argClient->argt->sd[i].addrClient.sin_port) != (int)ntohs(addr_Client.sin_port)))
            {
                Sleep(5);
                char taille[2];
                itoa(argClient->argt->size, taille, 10);
                send(argClient->socket,taille,sizeof(sizeof(char)*2),0);
                //printf("size %d sended\n",argClient->argt->size);

                Sleep(5);
                char bufferX[3] = "";
                char dataX[4] = "x";
                itoa(argClient->argt->sd[i].rectangle.x, bufferX, 10);
                strcat(dataX, bufferX);
                //printf("Sended %s\n",dataX);
                dataX[4] = '\0';
                if(argClient->socket == INVALID_SOCKET) printf("Error: INVALID SOCKET\n");
                if(send(argClient->socket,dataX,sizeof(sizeof(char)*4+1),0) == SOCKET_ERROR ) {
                    printf("Error: SOCKET_ERROR\n");
                    err = WSAGetLastError();
                    printf("%d\n",err);
                }
                char bufferY[3] = "";
                char dataY[4] = "y";
                itoa(argClient->argt->sd[i].rectangle.y, bufferY, 10);
                strcat(dataY, bufferY);
                //printf("Sended %s\n",dataY);
                dataY[4] = '\0';
                send(argClient->socket,dataY,sizeof(sizeof(char)*4+1),0);
                Sleep(5);

                char bufferW[3] = "";
                char dataW[4] = "w";
                itoa(argClient->argt->sd[i].rectangle.w, bufferW, 10);
                strcat(dataW, bufferW);
                //printf("Sended %s\n",dataW);
                dataW[4] = '\0';
                send(argClient->socket,dataW,sizeof(sizeof(char)*4+1),0);
                Sleep(5);

                char bufferH[3] = "";
                char dataH[4] = "h";
                itoa(argClient->argt->sd[i].rectangle.h, bufferH, 10);
                strcat(dataH, bufferH);
                //printf("Sended %s\n",dataH);
                dataH[4] = '\0';
                send(argClient->socket,dataH,sizeof(sizeof(char)*4+1),0);
                Sleep(5);
                send(argClient->socket,"over",sizeof(sizeof(char)*4+1),0);
                Sleep(5);
            }
            i++;
        } while (i < argClient->argt->size);
        send(argClient->socket,"end",sizeof(sizeof(char)*4+1),0);
        i=1;
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
    char buffer[6] = "";
    send2Client *argClient = (send2Client *)arg;
    SDL_Rect rect;
    SOCKADDR_IN addr_Client;

    if(argClient->socket != INVALID_SOCKET) printf("Ready to receive\n");
    
    //on récupère les données de positions des joueurs
    while(argClient->argt->running == TRUE)
    {
        Sleep(5);
        recv(argClient->socket,buffer,sizeof(sizeof(char)*4),0);
        //printf("pure data : %s\n",buffer);
        char c = traitData(buffer);
        //printf("pure tranformed : %s\n",buffer);
        switch (c)
        {
        case 'x':
            rect.x = atoi(buffer);
            //printf("received rect.x : %s\n",recvBuffer);
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

        //on clear l'input
        clearInput(argClient->socket);

        //on stock les données
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
        struct in_addr ipAddr = pV4Addr->sin_addr;

        do
        {
            i++;
        } while (strcmp(inet_ntoa(argClient->argt->sd[i].addrClient.sin_addr),inet_ntoa(addr_Client.sin_addr)) && ((int)ntohs(argClient->argt->sd[i].addrClient.sin_port) != (int)ntohs(addr_Client.sin_port)));

        //printf("find at position %d\n",i);
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
            argt2->sd->socketServer = argt2->sd->socketServer;
            printf("1 new client connected with ip %s and port %d\n",inet_ntoa(argt2->sd->addrClient.sin_addr), (int)ntohs(argt2->sd->addrClient.sin_port));
            printf("Connected clients : %d\n",argt2->size);
        }

        send2Client *argClient = malloc(sizeof(send2Client));
        argClient->socket = socketClient;
        argClient->argt = argt2;

        Sleep(500);
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
    addrServer.sin_addr.s_addr = inet_addr("192.168.1.16");     //ip locale
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