#include "socket.h"
#include "main.h"
#include "client.h"

#define dataLen 5

SDL_Rect *rect = NULL;
playersRect *p_datas = NULL;
static int connected;

void *receiveFromServer()
{
    if(p_datas != NULL) free(p_datas);
    if(rect != NULL) free(rect);
    int trash;
    char data[4] = "";
    int size = 0;
    int i = 1;
    trash = recv(*socketServer,data,sizeof(sizeof(char)*2),0);

    if(data[0]>=0 && atoi(data) <= 10) 
    {
        size = atoi(data);
        rect = malloc(sizeof(rect)*size);
        p_datas = malloc(sizeof(playersRect));
    }

    while(TRUE)
    {

        trash = recv(*socketServer,data,sizeof(sizeof(char)*4+1),0);
        //printf("Pure data : %s\n",data);

        char c = traitData(data);
        switch (c)
        {
        case 'x':
            rect[i].x = atoi(data);
            printf("received rect.x : %s\n",data);
            break;
        case 'y':
            rect[i].y = atoi(data);
            //printf("received rect.y : %d\n",rect[size].y);
            break;     
        case 'w':
            rect[i].w = atoi(data);
            //printf("received rect.w : %d\n",rect.w);
            break;   
        case 'h':
            rect[i].h = atoi(data);
            //printf("received rect.h : %d\n",rect.h);
            break;
        case 'o':
            i++;
            break;
        case 'e':
            //printf("datas: %d %d %d %d\n",rect.x,rect.y,rect.w,rect.h);
            i = 1;
            p_datas->rectangles = rect;
            p_datas->size = size;
            //printf("%d %d et %d\n",p_datas->rectangles[1].x,p_datas->rectangles[1].y,p_datas->size);
            //pthread_exit(p_datas);
            dessinerJoueur(p_datas,0);
            break;
        default:
            //printf("Incorrect data\n");
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
    send(*socketServer,dataX,sizeof(sizeof(char)*4),0);

    char bufferY[3] = "";
    char dataY[4] = "y";
    itoa(rect.y, bufferY, 10);
    strcat(dataY, bufferY);
    //printf("Sended %s\n",dataY);
    dataY[4] = '\0';
    send(*socketServer,dataY,sizeof(sizeof(char)*4),0);

    char bufferW[3] = "";
    char dataW[4] = "w";
    itoa(rect.w, bufferW, 10);
    strcat(dataW, bufferW);
    //printf("Sended %s\n",dataW);
    dataW[4] = '\0';
    send(*socketServer,dataW,sizeof(sizeof(char)*4),0);

    char bufferH[3] = "";
    char dataH[4] = "h";
    itoa(rect.h, bufferH, 10);
    strcat(dataH, bufferH);
    //printf("Sended %s\n",dataH);
    dataH[4] = '\0';
    send(*socketServer,dataH,sizeof(sizeof(char)*4),0);
}

void *stopConnection()
{
    closesocket(*socketServer);
    WSACleanup();
    printf("connection closed\n");
}

void *startConnection()
{   
    socketServer = malloc(sizeof(SOCKET));
    char *msg = malloc(sizeof(char)*2+1);
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr("90.93.91.79");      //ip publique
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    *socketServer = socket(AF_INET,SOCK_STREAM,0);

    //on test la connexion
    connected = connect(*socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    if(connected) printf("Error: connection lost\n");
    else printf("Connected\n");

}

//-lwsock32 -lpthread