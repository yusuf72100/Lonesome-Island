#include "socket.h"
#include "main.h"
#include "client.h"

SDL_Rect *rect = NULL;
playersRect *p_datas = NULL;

static argDessinerJoueurs * arg = NULL;
static int connected;

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
        /*case 'w':
            c = traitData(data);
            rect[i].w = atoi(data);
            break;   
        case 'h':
            c = traitData(data);
            rect[i].h = atoi(data);
            break;*/
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

void *sendPosition(SDL_Rect rect, int rotation)
{
    char bufferX[3] = "";
    char dataX[4] = "x";
    itoa(rect.x, bufferX, 10);
    strcat(dataX, bufferX);
    //printf("Sended %s\n",dataX);
    dataX[4] = '\0';
    send(*socket_Server,dataX,sizeof(sizeof(char)*4),0);

    char bufferY[3] = "";
    char dataY[4] = "y";
    itoa(rect.y, bufferY, 10);
    strcat(dataY, bufferY);
    //printf("Sended %s\n",dataY);
    dataY[4] = '\0';
    send(*socket_Server,dataY,sizeof(sizeof(char)*4),0);

    /*char bufferW[3] = "";
    char dataW[4] = "w";
    itoa(rect.w, bufferW, 10);
    strcat(dataW, bufferW);
    //printf("Sended %s\n",dataW);
    dataW[4] = '\0';
    send(*socket_Server,dataW,sizeof(sizeof(char)*4),0);

    char bufferH[3] = "";
    char dataH[4] = "h";
    itoa(rect.h, bufferH, 10);
    strcat(dataH, bufferH);
    //printf("Sended %s\n",dataH);
    dataH[4] = '\0';
    send(*socket_Server,dataH,sizeof(sizeof(char)*4),0);*/
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

//-lwsock32 -lpthread