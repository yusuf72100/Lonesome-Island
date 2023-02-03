/**
 * @file socket.c
 * @author Yusuf Ulas
 * @brief Fichier qui s'occupera uniquement du serveur
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "socket.h"

/**
 * @brief Construit une trame dédiée uniquement au client i.
 * 
 * @param joueur 
 * @param i 
 */
static void buildtramClient_send(player joueur, int i)
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

    char bufferC[3] = "";
    char dataC[4] = "c";
    itoa(joueur.connected, bufferC, 10);
    strcat(dataC, bufferC);
    strcat(tramClient_send, dataC);

    char bufferS[3] = "";
    char dataS[4] = "s";
    itoa(max_player, bufferS, 10);
    strcat(dataS, bufferS);
    strcat(tramClient_send, dataS);

}

/**
 * @brief Envoie la donnée d'un client à tous les autres y compris au client lui-même.
 * 
 * @param argClient 
 * @param position 
 * @return void* 
 */
static void *sendToClient(send2Client *argClient, int position)
{
    int i = 1, j = 1;
    SOCKADDR_IN addr_Client;
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&addr_Client;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    //à tout le monde
    buildtramClient_send(argClient->argt->sd[position].joueur, position);
    do
    {
        //on envoi les données du joueur en question
        if(argClient->argt->sd[i].clientSocket != INVALID_SOCKET && argClient->argt->sd[i].joueur.connected == TRUE)
        {
            if(send(argClient->argt->sd[i].clientSocket,tramClient_send,(sizeof(char)*30),0) == SOCKET_ERROR) printf("Server: Packet lost for %d\n",i);
            //printf("Sended to client: %s\n",tramClient_send);
        }
        i++;
    } while (i <= max_player);
    i=1;
}

/**
 * @brief Décrypte la trame reçut par le client indice.
 * 
 * @param argClient 
 * @param indice 
 */
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

/**
 * @brief Déconnecte le client en fonction de sa position dans le tableau.
 * 
 * @param argClient 
 * @param position 
 */
void disconnectPlayer(send2Client *argClient, int position)
{
    argClient->argt->sd[position].joueur.connected = FALSE;
    argClient->argt->size--;
    sendToClient(argClient, position);
    pthread_exit(&receive_from_client[position]);

    close(argt->sd[position].clientSocket);
}

/**
 * @brief Cherche la position du client dans le tableau des clients
 * 
 * @param argClient 
 * @return int 
 */
int findPosition(send2Client *argClient)
{
    int position = 0;

    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&argClient->socket;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    socklen_t len = sizeof(*pV4Addr);

    do
    {
        position++;
    } while (strcmp(inet_ntoa(argClient->argt->sd[position].addrClient.sin_addr),inet_ntoa(ipAddr)) || ((int)ntohs(argClient->argt->sd[position].addrClient.sin_port) != argClient->port));

    return position;
}

void initConnection(void *arg, int position)
{
    send2Client *argClient = arg;

    for(int i = 1; i <= max_player; i++)
    {
        buildtramClient_send(argClient->argt->sd[i].joueur, i);

        if(argClient->argt->sd[i].clientSocket != INVALID_SOCKET && argClient->argt->sd[i].joueur.connected == TRUE)
        {
            if(send(argClient->argt->sd[position].clientSocket,tramClient_send,(sizeof(char)*30),0) == SOCKET_ERROR) printf("Server: Packet lost for %d\n",i);
        }
    }
}

/**
 * @brief Écoute un client tant que la connexion est établie. (se lance dans un thread)
 * 
 * @param arg 
 * @return void* 
 */
void *receiveFromClient(void *arg)
{
    int i = 0, position=0;
    send2Client *argClient = (send2Client *)arg;

    //on récupère les données de positions des joueurs
    while(argClient->argt->running == TRUE)
    {
        tramClient_receive[0] = '\0';

        if(recv(argClient->socket,tramClient_receive,(sizeof(char)*30),0) == INVALID_SOCKET)
        {
            printf("Server: Client with id %d has been disconnected\n",position);
            disconnectPlayer(argClient, position);
        }
        else
        {
           
            i = findPosition(argClient);

            traitData(argClient, i);
            argClient->argt->sd[i].joueur.connected = TRUE;
            argClient->argt->sd[i].joueur.playerRect.w = 50;
            argClient->argt->sd[i].joueur.playerRect.h = 81;
            sendToClient(argClient, i);
            position = i;
            i=0;
        }

    }
    pthread_exit(&receive_from_client[i]);
}

/**
 * @brief Gère la place des clients dans la table de client et reçoit les nouveaux clients. (se lance dans un sous-thread)
 * 
 * @param arg 
 * @return void* 
 */
void *searchClients(void *arg)
{
    int place;
    argServer *argt2 = (argServer*)arg;
    socklen_t csize = sizeof(argt2->sd->addrClient);
    SOCKET socketClient;
    argt2->running = TRUE;
    argClient = malloc(sizeof(send2Client)*max_player+1);

    printf("Server running\n");
    
    while(argt2->running)
    {
        //on place le client dans une file d'attente
        socketClient = accept(argt2->sd->socketServer, (struct sockaddr *)&argt2->sd[max_player+1].addrClient, &csize);
        
        if(argt2->size < max_player+1)  
        {
            //on cherche une place pour le client
            place=1;
            while(argt2->sd[place].joueur.connected == TRUE || place < argt2->size) place++;
            argt2->sd[place] = argt2->sd[max_player+1];
            
            struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&argt2->sd[place].addrClient;
            struct in_addr ipAddr = pV4Addr->sin_addr;
            argt2->sd[place].socketServer = socketServer;
            argt2->sd[place].clientSocket = socketClient;
            printf("1 new client connected with ip %s and port %d and id %d\n",inet_ntoa(argt2->sd[place].addrClient.sin_addr), (int)ntohs(argt2->sd[place].addrClient.sin_port), place);
            printf("Connected clients : %d\n",argt2->size);
            argt2->size++;
            argClient[place].socket = socketClient;
            argClient[place].port = (int)ntohs(argt2->sd[place].addrClient.sin_port);
            argClient[place].argt = argt2;
            argt->sd[place].joueur.connected = TRUE;
            pthread_create(&receive_from_client[place],NULL,receiveFromClient,(void *)&argClient[place]);
        }
    }

    close(argt2->sd->socketServer);
    WSACleanup();
}

/**
 * @brief Arrête le serveur en déconnectant tous les clients d'abord.
 * 
 */
void stopServer()
{
    if(argt->running == TRUE)
    {
        for(int i = 2; i<=max_player+1; i++)
            if(argt->sd[i].joueur.connected == TRUE)
                disconnectPlayer(argClient, i);

        free(argt->sd);
        free(argt);
        free(argClient);
        free(receive_from_client);

        receive_from_client = NULL;
        argt->sd = NULL;
        argt = NULL;
        argClient = NULL;
        
        printf("Server: server cleared!\n");
        WSACleanup();
    }
}

/**
 * @brief Démarre le serveur et initie la table de clients. (se lance dans un thread)
 * 
 * @return void* 
 */
void *startServer()
{
    item bois = {
        .stack = 64,
        .eatable = FALSE,
        .isTool = FALSE,
        .isWeapon = FALSE,  
        .damage = 0
    };

    receive_from_client = malloc(sizeof(pthread_t)*max_player+1);
    socketDatas * sd = malloc(sizeof(socketDatas)*max_player+1);

    SOCKET clientSocket;
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    //socket du serveur
    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr(IP_LOCALE);    
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    socketServer = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    bind(socketServer, (SOCKADDR *)&addrServer, sizeof(addrServer));
    printf("bind : %d\n", socketServer);

    listen(socketServer, max_player+1);
    printf("Listening\n");

    argt = malloc(sizeof(argServer));
    argt->sd = sd;
    argt->running = TRUE;
    argt->size = 1;
    
    //socket des clients
    SOCKADDR_IN addrClient;
    socklen_t csize = sizeof(addrClient);

    for(int i = 1; i <= max_player +1; i++)
        argt->sd[i].joueur.connected = FALSE;

    argt->sd->socketServer = socketServer;
    argt->sd->clientSocket = clientSocket;
    argt->sd->addrClient = addrClient;
    argt->sd->joueur.playerRect.w = 50;
    argt->sd->joueur.playerRect.h = 81;

    //on lance et attend l'arrêt du serveur
    searchClients((void*)argt);
    printf("Fin du serveur\n");
}