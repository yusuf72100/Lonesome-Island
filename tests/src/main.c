#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define M 10

void initMatrice(int mat[M][M]){
    int i,j;
    int cpt = 0;
    for(i = 0; i < M; i++) {
        for(j = 0; j < M; j++) {
            mat[i][j] = cpt;
            cpt++;
        }
    }
}

void afficherMatrice(int mat[M][M]) {
    int i,j;
    for(i = 0; i < M; i++) {
        for(j = 0; j < M; j++) {
            printf("%3d", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int *argc, char *argv[]) {
    int mat[M][M];
    initMatrice(mat);
    afficherMatrice(mat);
    printf("succes");

    return 0;
}
