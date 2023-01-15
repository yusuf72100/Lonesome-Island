#include "socket.h"
#include "main.h"
#include "client.h"

#define THREAD_NULL 0
#define dataLen 5

SDL_Rect *rect = NULL;
playersRect *p_datas = NULL;

static argDessinerJoueurs * arg = NULL;
static pthread_t * displayThreads = NULL;
static int connected;

void initThread()
{
    displayThreads = malloc(sizeof(pthread_t)*10);
    arg = malloc(sizeof(argDessinerJoueurs)*10);
    for(int i = 0; i<10; i++){
        displayThreads[i] = THREAD_NULL;
    }
}

//cette fonction mettra à jour le tableau des positions 
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
            //printf("received rect.x : %s\n",data);
            break;
        case 'y':
            rect[i].y = atoi(data);
            break;     
        case 'w':
            rect[i].w = atoi(data);
            break;   
        case 'h':
            rect[i].h = atoi(data);
            break;
        case 'o':
            if(displayThreads[i] != THREAD_NULL)
            {
                arg[i].rect = rect[i];
                printf("rect %d\n",arg[i].rect.x);
                arg[i].rotation = 0;

            }
            else {
                pthread_create((displayThreads+i),NULL,dessinerJoueur,(void*)&arg[i]);  
                arg[i].rect = rect[i];
                printf("rect %d\n",arg[i].rect.x);
                arg[i].rotation = 0;
                printf("Thread launched\n");
            }
            i++;
            break;
        case 'e':
            i = 1;
            p_datas->rectangles = rect;
            p_datas->size = size;
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
    Sleep(5);

    char bufferY[3] = "";
    char dataY[4] = "y";
    itoa(rect.y, bufferY, 10);
    strcat(dataY, bufferY);
    //printf("Sended %s\n",dataY);
    dataY[4] = '\0';
    send(*socketServer,dataY,sizeof(sizeof(char)*4),0);
    Sleep(5);

    char bufferW[3] = "";
    char dataW[4] = "w";
    itoa(rect.w, bufferW, 10);
    strcat(dataW, bufferW);
    //printf("Sended %s\n",dataW);
    dataW[4] = '\0';
    send(*socketServer,dataW,sizeof(sizeof(char)*4),0);
    Sleep(5);

    char bufferH[3] = "";
    char dataH[4] = "h";
    itoa(rect.h, bufferH, 10);
    strcat(dataH, bufferH);
    //printf("Sended %s\n",dataH);
    dataH[4] = '\0';
    send(*socketServer,dataH,sizeof(sizeof(char)*4),0);
    Sleep(5);
}

void *stopConnection()
{
    closesocket(*socketServer);
    WSACleanup();
    printf("connection closed\n");
}

void *startConnection()
{   
    initThread();
    socketServer = malloc(sizeof(SOCKET));
    char *msg = malloc(sizeof(char)*2+1);
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
    SOCKADDR_IN addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");      //ip publique 90.93.91.79
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4148);
    *socketServer = socket(AF_INET,SOCK_STREAM,0);

    //on test la connexion
    connected = connect(*socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
    if(connected) printf("Error: connection lost\n");
    else printf("Connected\n");

}

//-lwsock32 -lpthread