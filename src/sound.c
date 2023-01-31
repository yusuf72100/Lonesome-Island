/**
 * @file sound.c
 * @author Yusuf Ulas
 * @brief Fichier de gestion de l'audio
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "sound.h"

void initAudio()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
        printf("%s", Mix_GetError());
}

void creation_canaux_musique()
{
    if(Mix_AllocateChannels(2) == -1) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
    /*Cannaux:
     *0:Musique fond
     *1:Effets joueur 1
     *2:Effets Joueur 2
     *3:Effets Joueur 3
     *4:Effets Joueur 4
     *5:Dialogues
     *6:Buffer
     */
    boop = Mix_LoadWAV("resources/sounds/boop.wav");
    hover = Mix_LoadWAV("resources/sounds/hover.wav");
}

void *play_hover_audio(void *key)
{
    SDL_bool *temp = key;
    Mix_PlayChannel(1, hover, 0);
    while(*temp);
    pthread_exit(NULL);
}

void init_hover(SDL_bool *key)
{
    if(pthread_kill(hover_Thread,0) != 0)
    {
        pthread_create(&hover_Thread,NULL,play_hover_audio,key);
    }
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