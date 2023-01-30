#include "client.h"
#include "main.h"

int connectedError= FALSE;

static void traitData()
{
    int position = 0;
    int j, k;
    char buffer[20] = "\0";
    //printf("tien : %s",tramClient_receive);
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
            joueurs[position].playerRect.x = atoi(buffer);
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
            joueurs[position].playerRect.y = atoi(buffer);
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
            size = atoi(buffer);
            k=0;
            buffer[0] = '\0';
        }
    }
}

void *receiveFromServer()
{
    while(TRUE)
    {
        recv(*socket_Server,tramClient_receive,(sizeof(char)*30),0);
        //printf("Received from server: %s\n",tramClient_receive);
        traitData();
        synchDatas(joueurs,size);
    }
}

static void buildTram(player joueur)
{
    tramClient_send[0] = '\0';
    strcat(tramClient_send, "fff ");
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
    strcat(tramClient_send, " fff");
}

void *sendDatas(player joueur)
{
    buildTram(joueur);
    if(send(*socket_Server,tramClient_send,(sizeof(char)*30),0) == SOCKET_ERROR)
    {
        stopConnection();
        connectedError= TRUE;
    }
}

void *stopConnection()
{
    closesocket(*socket_Server);
    WSACleanup();
    printf("Connection closed\n");
}

int startConnection()
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
