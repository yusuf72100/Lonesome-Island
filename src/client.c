#include "socket.h"
#include "main.h"
#include "client.h"

SDL_Rect *rect = NULL;
playersRect *p_datas = NULL;

static argDessinerJoueurs * arg = NULL;
static int connected;

void *receiveFromServer()
{
    recv(*socket_Server,tramClient_receive,sizeof(sizeof(char)*100+1),0);
    int j, k;
    char buffer[10] = "\0";
    for(j = 0; tram[j]!='\0'; j++){
        if(tram[j] == 'x')
        {
            k=j+1;
            while(tram[k] >= '0' && tram[k] <= '9')
            {
                buffer[k] = tram[k];
            }
            joueur.playerRect.x = atoi(buffer);
        }
        else if(tram[j] == 'y')
        {
            k=j+1;
            while(tram[k] >= '0' && tram[k] <= '9')
            {
                buffer[k] = tram[k];
            }
            joueur.playerRect.y = atoi(buffer);
        }
        else if(tram[j] == 'a')
        {
            k=j+1;
            while(tram[k] >= '0' && tram[k] <= '9')
            {
                buffer[k] = tram[k];
            }
            joueur.animation_state = atoi(buffer);
        }
    }
}

/*
//cette fonction mettra à jour le tableau des positions 
void *receiveFromServer()
{
    if(p_datas != NULL) free(p_datas);
    if(rect != NULL) free(rect);
    int trash;
    char data[4] = "";
    int size = 0;
    int i = 1;
    char c;

    rect = malloc(sizeof(SDL_Rect)*10);
    p_datas = malloc(sizeof(playersRect));

    while(TRUE)
    {
        //printf("size : %d\n",size);
        trash = recv(*socket_Server,data,sizeof(sizeof(char)*4+1),0);
        //printf("Pure data : %s\n",data);

        switch (data[0])
        {
        case 'x':
            c = traitData(data);
            rect[i].x = atoi(data);
            //printf("received rect.x : %s pour i = %d\n",data,i);
            break;
        case 'y':
            c = traitData(data);
            rect[i].y = atoi(data);
            break;     
        case 'w':
            c = traitData(data);
            rect[i].w = atoi(data);
            break;   
        case 'h':
            c = traitData(data);
            rect[i].h = atoi(data);
            break;
        case 's':
            c = traitData(data);
            size = atoi(data);
            size--;
            break;    
        case 'o':
            if(strcmp(data,"over") == 0)
            {
                i++;
            }
            break;
        case 'e':
            if(strcmp(data,"end") == 0)
            {
                i--;
                p_datas->rectangles = rect;
                p_datas->size = size;
                synch_datas(p_datas);
                i = 1;
            }
            break;
        default:
            break;
        }
    }
}
*/

static void buildTram(player joueur)
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
    printf("Tram: %s\n",tramClient_send);
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
