# TP parallélisme

## References

- https://en.wikipedia.org/wiki/OpenMP

## Answers

To complete this basic lab on OpenMP with the C language, you will need the following tools and software on your machine:

1. An OpenMP-Compatible Compiler: gcc (GNU Compiler Collection).
2. Installation on Linux (Arch btw~).

### Important note

You have to add this option to compile your code:

```bash
-fopenmp
    Enable  handling of OpenMP directives #pragma omp, [[omp::directive(...)]], [[omp::sequence(...)]] and [[omp::decl(...)]] in
    C/C++ and !$omp in Fortran.  It additionally enables the conditional compilation sentinel !$ in Fortran.   In  fixed  source
    form  Fortran,  the  sentinels can also start with c or *.  When -fopenmp is specified, the compiler generates parallel code
    according to the OpenMP Application Program Interface v4.5 <https://www.openmp.org>.  This option implies -pthread, and thus
    is only supported on targets that have support for -pthread. -fopenmp implies -fopenmp-simd.
```

### Example 01

Code:

```c
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
```

Result:

```bash
tp2 on  main [!?] via C v14.2.1-gcc
➜ gcc example1.c -fopenmp

tp2 on  main [!?] via C v14.2.1-gcc
➜ ls
a.out  example1.c  poc.c  README.md

tp2 on  main [!?] via C v14.2.1-gcc
➜ ./a.out
Hello(0)world(0)
Hello(6)world(6)
Hello(2)world(2)
Hello(7)world(7)
Hello(4)world(4)
Hello(5)world(5)
Hello(3)world(3)
Hello(1)world(1)
```

Explanation: OpenMP creates 8 threads matching CPU cores; each thread prints its ID independently,
causing unordered "Hello(ID)world(ID)" output.

### Example 02

Code:

```c
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
```

Result:

```bash
tp2 on  main [!?] via C v14.2.1-gcc
➜ gcc example2.c -fopenmp

tp2 on  main [!?] via C v14.2.1-gcc
➜ ./a.out
Hello(9)world(9)
Hello(4)world(4)
Hello(6)world(6)
Hello(1)world(1)
Hello(8)world(8)
Hello(2)world(2)
Hello(3)world(3)
Hello(7)world(7)
Hello(0)world(0)
Hello(5)world(5)
```

Explanation: The same thing as before, but we specified the number of threads spawned using `num_threads (10)`

### Example 03

Code:

```c
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
void main() {
    omp_set_num_threads(2) ;
    int a = 10, b = 20, c = 30 ;
    #pragma omp parallel private (a)
    {
        printf(" %d \n", a + 1) ;
    }
    #pragma omp parallel firstprivate (b)
    {
        b=b + 1;
        printf(" %d \n", b ) ;
    }
    #pragma omp parallel shared (c)
    {

        printf(" %d \n", c ) ;
    }
}
```

Result:

```bash
tp2 on  main [!?] via C v14.2.1-gcc
➜ gcc example3.c -fopenmp

tp2 on  main [!?] via C v14.2.1-gcc
➜ ./a.out
 23225
 32480
 21
 21
 30
 30
```

Explanation:

- `omp_set_num_threads`: sets 2 threads for each `omp parallel` directive
- `private`: Each thread gets its own uninitialized copy of a.
- `firstprivate`: Each thread gets a copy of b initialized with the original value (20).
- `shared`: c is shared across threads.

### Example 04

#### Sequential

Code:

```c
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
int main() {
    const int DIM = 1000;
    int i,j,k;
    double debut, fin, temps;
    double **a, **b, **cresu, **ctest;
    a= (double**) malloc(DIM*sizeof(double*));
    b= (double**) malloc(DIM*sizeof(double*));
    cresu= (double**) malloc(DIM*sizeof(double*));
    ctest= (double**) malloc(DIM*sizeof(double*));
    // initialisations etc...
    for (i=0; i<DIM; i++)
    {
        a[i]=(double*) malloc(DIM*sizeof(double));
        b[i]=(double*) malloc(DIM*sizeof(double));
        cresu[i]=(double*) malloc(DIM*sizeof(double));
        ctest[i]=(double*) malloc(DIM*sizeof(double));
        for (j = 0; j < DIM; j++)
        {
            a[i][j] = (double)(i-j);
            b[i][j] = (double)(i+j);
            cresu[i][j] = 0.0;
            ctest[i][j] = 0.0;
        }
    } // Multiplication C = A x B (séquentiel)
    printf("Multiplication sequentielle:\n");
    debut= omp_get_wtime();
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++)
            for (k = 0; k < DIM; k++)
                ctest[i][j] += a[i][k] * b[k][j];
    fin= omp_get_wtime();
    temps=fin-debut;
    printf ("Calcul séquentiel %f secondes\n", temps);
    // Multiplication C = A x B (parallèle)
    return (0);
}
```

Result:

```bash
tp2 on  main [!?] via C v14.2.1-gcc
➜ ./a.out
Multiplication sequentielle:
Calcul séquentiel 9.219313 secondes
```

#### Parallel

```c
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
```

Result:

```bash
tp2 on  main [!?] via C v14.2.1-gcc
➜ ./a.out
Multiplication parallèle:
Calcul parallèle 2.333815 secondes
```

La directive:

```c
#pragma omp parallel for private(i, j, k) schedule(static) num_threads(4)
```

sert a:

- Paralléliser l'exécution de la boucle for sur 4 threads (num_threads(4)).
- Répartir statiquement les itérations entre les threads (schedule(static)), c’est-à-dire que les itérations sont divisées également au début.
- Définir i, j, et k comme privés, donc chaque thread possède ses propres copies de ces variables (private(i, j, k)), évitant ainsi les conflits d'accès.

---

END.
