#include "sound.h"

void initAudio()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
        printf("%s", Mix_GetError());
}

void creation_canaux_musique()
{
    if(Mix_AllocateChannels(2) == -1) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());;
    /*Cannaux:
     *0:Musique fond
     *1:Effets joueur 1
     *2:Effets Joueur 2
     *3:Effets Joueur 3
     *4:Effets Joueur 4
     *5:Dialogues
     *6:Buffer
     */
    boop = Mix_LoadWAV("resources/boop.wav");
}

void *play_boop(void *key)
{
    int *temp = key;
    Mix_PlayChannel(0, boop, 0);
    while(*temp);
    pthread_exit(NULL);
}

void init_boop(SDL_bool *key)
{
    if(pthread_kill(boop_Thread,0) != 0)
    {
        pthread_create(&boop_Thread,NULL,play_boop,key);
    }
}

void detruire_musique(Mix_Chunk* chunk)
{
    Mix_FreeChunk(chunk);
}