#include <omp.h>
#include <stdio.h>
void main () {
 #pragma omp parallel num_threads (10)
 {// Création de threads (processus légers)
 int ID = omp_get_thread_num ();
 printf("Hello(%d)", ID);
 printf ("world(%d)\n", ID);
 }//Destruction de threads
 }
