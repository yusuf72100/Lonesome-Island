/**
 * @file liste.h
 * @author Yusuf Ulas
 * @brief CECI EST UN FICHIER NON UTILISÉ PAR LE PROGRAMME PRINCIPAL, IL S'AGIT D'UN FICHIER DE TEST!
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */
#ifndef LISTE_H
#define LISTE_H

#include <stdlib.h>
#include <SDL.h>

typedef struct
{
     double x;
     double y;
     
}Vecteur;

typedef struct
{
  float DirX,DirY;            // Vecteur normalisé pour la direction (DirX = cos(Angle), DirY = -sin(Angle))
  int Vitesse;
  int rotation;
  int age;

  SDL_Rect rectangle;

}Bullet;

typedef struct element
{
    Bullet *b;
    struct element *suivant;

}Liste;

Liste *creerListe();

Liste* suivant(Liste *l);

void supprimerListe(Liste *l);

void ajouterElement(Liste *l, Bullet *b);

int lenght(Liste *l);

int supprimerElement(Liste *l, Bullet *b);

int elementCompare(Bullet *b1, Bullet *b2);

int hasNext(Liste *l);

#endif