#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
void main() {
    omp_set_num_threads(2) ;
    int a = 10, b = 20, c = 30 ;
    #pragma omp parallel private (a)
    {
        printf("inside private block: %d \n", a + 1) ;
    }
    #pragma omp parallel firstprivate (b)
    {
        b=b + 1;
        printf("inside firstprivate block: %d \n", b ) ;
    }
    #pragma omp parallel shared (c)
    {

        printf("inside shared block: %d \n", c ) ;
    }
}
