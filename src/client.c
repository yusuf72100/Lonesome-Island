#include "socket.h"
#include "main.h"
#include "client.h"

static void traitData()
{
    int position = 0;
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
            joueurs[position].playerRect.x = atoi(buffer);
        }
        else if(tramClient_receive[j] == 'y')
        {
            k=j+1;
            while(tramClient_receive[k] >= '0' && tramClient_receive[k] <= '9')
            {
                buffer[k] = tramClient_receive[k];
            }
            joueurs[position].playerRect.y = atoi(buffer);
        }
        else if(tramClient_receive[j] == 'a')
        {
            k=j+1;
            while(tramClient_receive[k] >= '0' && tramClient_receive[k] <= '9')
            {
                buffer[k] = tramClient_receive[k];
            }
            joueurs[position].animation_state = atoi(buffer);
        }
        else if(tramClient_receive[j] == 'i')
        {
            k=j+1;
            while(tramClient_receive[k] >= '0' && tramClient_receive[k] <= '9')
            {
                buffer[k] = tramClient_receive[k];
            }
            position = atoi(buffer);
        }
        else if(tramClient_receive[j] == 's')
        {
            k=j+1;
            while(tramClient_receive[k] >= '0' && tramClient_receive[k] <= '9')
            {
                buffer[k] = tramClient_receive[k];
            }
            size = atoi(buffer);
        }
    }
}

void *receiveFromServer()
{
    recv(*socket_Server,tramClient_receive,sizeof(sizeof(char)*100+1),0);
    traitData();
}

static void buildTram()
{
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
}

void *sendDatas(player joueur)
{
    buildTram(joueur);
    send(*socket_Server,tramClient_send,sizeof(sizeof(char)*strlen(tramClient_send)),0);
}

void *stopConnection()
{
    closesocket(*socket_Server);
    WSACleanup();
    printf("connection closed\n");
}

void *startConnection()
{   
    joueurs = malloc(sizeof(player) * 10);
    socket_Server = malloc(sizeof(SOCKET));
    char *msg = malloc(sizeof(char)*2+1);
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr(IP_LOCALE);      
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    *socket_Server = socket(AF_INET,SOCK_STREAM,0);

    //on test la connexion
    connected = connect(*socket_Server, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    if(connected) printf("Error: connection lost\n");
    else printf("Connected\n");

}
