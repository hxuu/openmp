#include <omp.h>
#include <stdio.h>
int main () {
    #pragma omp parallel
    {// Création de threads (processus légers)
        int ID = omp_get_thread_num();
        printf("Hello(%d)", ID);
        printf ("world(%d)\n", ID);
    }//Destruction de threads
}

