/**
 * @file liste.c
 * @author Yusuf Ulas
 * @brief Fichier d'utilisation de listes
 * @version 0.1
 * @date 2023-01-31
 * @copyright Copyright (c) 2023
 */

#include "liste.h"

void supprimerListe(Liste *l)
{
    if(l->suivant != NULL)
    {
        supprimerListe(l->suivant);
    }
    free(l);
}

Liste *creerListe()
{
    Liste *nouveau = malloc(sizeof(Liste*) + 100);
    nouveau->b = NULL; 
    nouveau->suivant = NULL; 

    return nouveau;
}

Liste* suivant(Liste *l)
{
    return l->suivant;
}

void ajouterElement(Liste *l, Bullet *b)
{
    if(l->suivant != NULL)
        ajouterElement(l->suivant,b);

    else
    {
        Liste *nouveau = malloc(sizeof(Liste*) + 100);
        nouveau->b = b; 
        nouveau->suivant = NULL; 
        l->suivant = nouveau;
    }
        
}

// renvoi la longueur de la liste
int lenght(Liste *l)
{
    if(l->suivant == NULL)
        return 0;
    else
        return 1 + lenght(l->suivant);
}

int elementCompare(Bullet *b1, Bullet *b2)
{
    return(b1->DirX == b2->DirX && b1->DirY == b2->DirY && b1->Vitesse == b2->Vitesse && b1->rotation == b2->rotation && b1->rectangle.x == b1->rectangle.y && b1->rectangle.y == b2->rectangle.y && b1->rectangle.h == b2->rectangle.h && b1->rectangle.w == b2->rectangle.w);
}

int supprimerElement(Liste *l, Bullet *b)
{
    if(elementCompare(l->suivant->b, b))
    {
        Liste* elementSup = l->suivant;
        l->suivant = elementSup->suivant;
        free(elementSup);
    }

    else
        supprimerElement(l->suivant,b);
}

// renvoi 0 si la liste n'a plus de place
int hasNext(Liste *l)
{
    if(l == NULL)
        return 0;

    return lenght(l);
}