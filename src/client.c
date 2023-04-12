/**
 * @file client.c
 * @author Yusuf Ulas
 * @brief Fichier qui contient la configuration du socket client
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "client.h"
#include "main.h"

/**
 * @brief Traite les données reçus par le serveur
 * 
 */
static void traitData()
{
    int position = 0;
    int j, k;
    char buffer[20] = "\0";

    for(j = 0; tramClient_receive[j]!='\0'; j++){
        k = 0;
        if(tramClient_receive[j] == 'i')
        {
            j++;
            while(tramClient_receive[j] >= '0' && tramClient_receive[j] <= '9')
            {
                buffer[k] = tramClient_receive[j];
                k++;
                j++;
            }
            buffer[k] = '\0';
            position = atoi(buffer);
            k=0;
            buffer[0] = '\0';
        }
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
            joueurs[position].mapPosition.x = atoi(buffer);
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
            joueurs[position].mapPosition.y = atoi(buffer);
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
            joueurs[position].animation_state = atoi(buffer);
            k=0;
            buffer[0] = '\0';
        }
        if(tramClient_receive[j] == 'c')
        {

            j++;
            while(tramClient_receive[j] >= '0' && tramClient_receive[j] <= '9')
            {
                buffer[k] = tramClient_receive[j];
                k++;
                j++;
            }
            buffer[k] = '\0';
            joueurs[position].connected = atoi(buffer);
            k=0;
            buffer[0] = '\0';
        }
        if(tramClient_receive[j] == 's')
        {

            j++;
            while(tramClient_receive[j] >= '0' && tramClient_receive[j] <= '9')
            {
                buffer[k] = tramClient_receive[j];
                k++;
                j++;
            }
            buffer[k] = '\0';
            size = atoi(buffer) + 1;
            k=0;
            buffer[0] = '\0';
        }
    }
    synchDatas(joueurs,size);
}

/**
 * @brief Établie la connexion avec le serveur. (se lance dans un thread)
 * 
 * @return void* 
 */
void *receiveFromServer()
{
    while(recv(*socket_Server,tramClient_receive,(sizeof(char)*30),0) != INVALID_SOCKET)
    {
        //printf("Received from server: %s\n",tramClient_receive);
        traitData();
    }
    stopConnection();
}

/**
 * @brief Construit une trame réseau dédiée uniquement au serveur.
 * 
 * @param joueur 
 */
static void buildTram(player_t joueur)
{
    tramClient_send[0] = '\0';
    strcat(tramClient_send, "fff ");
    char bufferX[3] = "";
    char dataX[4] = "x";
    itoa(joueur.mapPosition.x, bufferX, 10);
    strcat(dataX, bufferX);
    strcat(tramClient_send, dataX);

    char bufferY[3] = "";
    char dataY[4] = "y";
    itoa(joueur.mapPosition.y, bufferY, 10);
    strcat(dataY, bufferY);

    strcat(tramClient_send, dataY);
    
    char bufferA[3] = "";
    char dataA[4] = "a";
    itoa(joueur.animation_state, bufferA, 10);
    strcat(dataA, bufferA);
    strcat(tramClient_send, dataA);
    strcat(tramClient_send, " fff");
}

/**
 * @brief Récupère les données du joueur et les envois au serveur via le socket.
 * 
 * @param joueur 
 * @return void* 
 */
void *sendDatas(player_t joueur)
{
    buildTram(joueur);
    if(send(*socket_Server,tramClient_send,(sizeof(char)*30),0) == SOCKET_ERROR)
    {
        connectedError = TRUE;
        stopConnection();
    }
}

/**
 * @brief Arrête la connexion avec le serveur.
 * 
 * @return void* 
 */
void stopConnection()
{
    printf("Connection closed\n");
    closesocket(*socket_Server);
    WSACleanup();
    CONNECTED = FALSE;
}

/**
 * @brief Démarre la connexion au serveur et renvoi TRUE en cas d'erreur sinon renvoi FALSE.
 * 
 * @return int 
 */
int startConnection()
{   
    socket_Server = malloc(sizeof(SOCKET));
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
    SOCKADDR_IN addrServer;

    if(HOST == FALSE && SOLO == FALSE)
    {
        addrServer.sin_addr.s_addr = inet_addr(IP_PUBLIC);
    }
    else {
        addrServer.sin_addr.s_addr = inet_addr(getLocalIp());
    }
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    *socket_Server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    //on test la connexion
    connectedError = connect(*socket_Server, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    if(connectedError == SOCKET_ERROR) 
    {
        printf("Error : ");
        stopConnection();
        return 1;
    }

    printf("Connected\n");
    return 0;
}
