#include "socket.h"

static void buildtramClient_send(player joueur, int i, int size)
{
    char bufferI[3] = "";
    char dataI[4] = "i";
    itoa(i, bufferI, 10);
    strcat(dataI, bufferI);
    strcat(tramClient_send, dataI);

    tramClient_send[0] = '\0';
    strcat(tramClient_send, "-");
    char bufferX[3] = "";
    char dataX[4] = "x";
    itoa(joueur.playerRect.x, bufferX, 10);
    strcat(dataX, bufferX);
    strcat(tramClient_send, dataX);

    char bufferY[3] = "";
    char dataY[4] = "y";
    itoa(joueur.playerRect.y, bufferY, 10);
    strcat(dataY, bufferY);
    strcat(tramClient_send, dataY);
    
    char bufferA[3] = "";
    char dataA[4] = "a";
    itoa(joueur.animation_state, bufferA, 10);
    strcat(dataA, bufferA);
    strcat(tramClient_send, dataA);

    char bufferS[3] = "";
    char dataS[4] = "s";
    itoa(size, bufferS, 10);
    strcat(dataS, bufferS);
    strcat(tramClient_send, dataS);
}

//envoi les données aux clients
static void *sendToClient(void *arg)
{
    int err;
    int i = 1, j = 1;
    send2Client *argClient = (send2Client *)arg;
    SOCKADDR_IN addr_Client;
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    //à tout le monde
    do
    {
        //on envoi les coordonnées de tout le monde
        do
        {
            buildtramClient_send(argClient->argt->sd[i].joueur, i, argClient->argt->size);
            send(argClient->argt->sd[i].clientSocket,dataS,sizeof(sizeof(char)*2),0);
            j++;

        } while(j < argClient->argt->size);
        j=1;
        i++;
    } while (i < argClient->argt->size);
    i=1;
}

static void traitData(player joueur)
{
    int j, k;
    char buffer[10] = "\0";
    for(j = 0; tramClient_receiveClient_send[j]!='\0'; j++){
        if(tramClient_receive[j] == 'x')
        {
            k=j+1;
            while(tramClient_receive[k] >= '0' && tramClient_receive[k] <= '9')
            {
                buffer[k] = tramClient_receive[k];
            }
            joueur.playerRect.x = atoi(buffer);
        }
        else if(tramClient_receive[j] == 'y')
        {
            k=j+1;
            while(tramClient_receive[k] >= '0' && tramClient_receive[k] <= '9')
            {
                buffer[k] = tramClient_receive[k];
            }
            joueur.playerRect.y = atoi(buffer);
        }
        else if(tramClient_receive[j] == 'a')
        {
            k=j+1;
            while(tramClient_receive[k] >= '0' && tramClient_receive[k] <= '9')
            {
                buffer[k] = tramClient_receive[k];
            }
            joueur.animation_state = atoi(buffer);
        }
    }
}

//fonction qui se lance dans un thread
void *receiveFromClient(void *arg)
{
    pthread_t send_to_client;
    int i = 0;
    send2Client *argClient = (send2Client *)arg;

    if(argClient->socket != INVALID_SOCKET) printf("Ready to receive\n");
    
    //on récupère les données de positions des joueurs
    while(argClient->argt->running == TRUE)
    {
        recv(argClient->socket,tramClient_receive,sizeof(sizeof(char)*100+1),0);

        //on stock les données du client
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&argClient->socket;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        socklen_t len = sizeof(*pV4Addr);

        //on cherche la position du client dans le tableau de données
        do
        {
            i++;
        } while (strcmp(inet_ntoa(argClient->argt->sd[i].addrClient.sin_addr),inet_ntoa(ipAddr)) || ((int)ntohs(argClient->argt->sd[i].addrClient.sin_port) != argClient->port));

        Sleep(5);
        traitData(argClient->argt->sd[i].joueur);

        //printf("find at position %d\n",i);
        argClient->argt->sd[i].joueur.playerRect.w = 50;
        argClient->argt->sd[i].joueur.playerRect.h = 81;
        pthread_create(&send_to_client,NULL,sendToClient,(void *)argClient);
        //sendToClient((void *)argClient);
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
            argt2->sd[argt2->size].clientSocket = socketClient;
            printf("1 new client connected with ip %s and port %d\n",inet_ntoa(argt2->sd[argt2->size].addrClient.sin_addr), (int)ntohs(argt2->sd[argt2->size].addrClient.sin_port));
            printf("Connected clients : %d\n",argt2->size);
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
    argt->sd->joueur.playerRect.w = 50;
    argt->sd->joueur.playerRect.h = 81;

    //on lance et attend l'arrêt du serveur
    searchClients((void*)argt);
    printf("Fin du serveur\n");
}