#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>

int main() {
    const int DIM = 1000;
    int i, j, k;
    double debut, fin, temps;
    double **a, **b, **cresu, **ctest;

    a = (double**) malloc(DIM * sizeof(double*));
    b = (double**) malloc(DIM * sizeof(double*));
    cresu = (double**) malloc(DIM * sizeof(double*));
    ctest = (double**) malloc(DIM * sizeof(double*));

    // Initialisation
    for (i = 0; i < DIM; i++) {
        a[i] = (double*) malloc(DIM * sizeof(double));
        b[i] = (double*) malloc(DIM * sizeof(double));
        cresu[i] = (double*) malloc(DIM * sizeof(double));
        ctest[i] = (double*) malloc(DIM * sizeof(double));

        for (j = 0; j < DIM; j++) {
            a[i][j] = (double)(i - j);
            b[i][j] = (double)(i + j);
            cresu[i][j] = 0.0;
            ctest[i][j] = 0.0;
        }
    }

    printf("Multiplication parallèle:\n");
    debut = omp_get_wtime();

    // Corrected OpenMP parallel region
    #pragma omp parallel for private(i, j, k) schedule(static) num_threads(4)
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            for (k = 0; k < DIM; k++) {
                ctest[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    fin = omp_get_wtime();
    temps = fin - debut;
    printf("Calcul parallèle %f secondes\n", temps);

    return 0;
}

