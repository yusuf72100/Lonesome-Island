#include "socket.h"

static void buildtramClient_send(player joueur, int i, int size)
{
    tramClient_send[0] = '\0';
    char bufferI[3] = "";
    char dataI[4] = "i";
    itoa(i, bufferI, 10);
    strcat(dataI, bufferI);
    strcat(tramClient_send, dataI);

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
static void *sendToClient(send2Client *argClient)
{
    int err;
    int i = 1, j = 1;
    SOCKADDR_IN addr_Client;
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    //à tout le monde
    do
    {
        //on envoi les coordonnées de tout le monde
        do
        {
            if(argClient->argt->sd[i].clientSocket != SOCKET_ERROR)
            {
                buildtramClient_send(argClient->argt->sd[j].joueur, j, argClient->argt->size);
                send(argClient->argt->sd[i].clientSocket,tramClient_send,(sizeof(char)*30),0);
            }
            j++;
        } while(j < argClient->argt->size);
        j=1;
        i++;
    } while (i < argClient->argt->size);
    i=1;
}

static void traitData(send2Client *argClient, int indice)
{
    int j, k;
    char buffer[20] = "\0";
    for(j = 0; tramClient_receive[j]!='\0'; j++){
        k = 0;

        if(tramClient_receive[j] == 'x')
        {
            j++;
            while(tramClient_receive[j] >= '0' && tramClient_receive[j] <= '9')
            {
                buffer[k] = tramClient_receive[j];
                k++;
                j++;
            }
            buffer[k] = '\0';
            argClient->argt->sd[indice].joueur.playerRect.x = atoi(buffer);
            k=0;
            buffer[0] = '\0';
        }
        if(tramClient_receive[j] == 'y')
        {
            j++;
            while(tramClient_receive[j] >= '0' && tramClient_receive[j] <= '9')
            {
                buffer[k] = tramClient_receive[j];
                k++;
                j++;
            }
            buffer[k] = '\0';
            argClient->argt->sd[indice].joueur.playerRect.y = atoi(buffer);
            k=0;
            buffer[0] = '\0';
        }
        if(tramClient_receive[j] == 'a')
        {
            j++;
            while(tramClient_receive[j] >= '0' && tramClient_receive[j] <= '9')
            {
                buffer[k] = tramClient_receive[j];
                k++;
                j++;
            }
            buffer[k] = '\0';
            argClient->argt->sd[indice].joueur.animation_state = atoi(buffer);
            k=0;
            buffer[0] = '\0';
        }
    }
}

//fonction qui se lance dans un thread
void *receiveFromClient(void *arg)
{
    int i = 0;
    send2Client *argClient = (send2Client *)arg;
    
    //on récupère les données de positions des joueurs
    while(argClient->argt->running == TRUE)
    {

        tramClient_receive[0] = '\0';
        recv(argClient->socket,tramClient_receive,(sizeof(char)*30),0);

        //on stock les données du client
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&argClient->socket;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        socklen_t len = sizeof(*pV4Addr);

        //on cherche la position du client dans le tableau de données
        do
        {
            i++;
        } while (strcmp(inet_ntoa(argClient->argt->sd[i].addrClient.sin_addr),inet_ntoa(ipAddr)) || ((int)ntohs(argClient->argt->sd[i].addrClient.sin_port) != argClient->port));

        traitData(argClient, i);

        argClient->argt->sd[i].joueur.playerRect.w = 50;
        argClient->argt->sd[i].joueur.playerRect.h = 81;
        sendToClient(argClient);
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