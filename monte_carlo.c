# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
#include "omp.h"

int main ();
void monte_carlo (int n, int *seed);
double random_value (int *seed);
void timestamp ( );

int main(void)
{
    int n;
    int seed;
    timestamp ( );
    printf ( "\n" );
    printf ( "RANDOM\n" );
    printf ( "  C version\n" );
    printf ( "  The random numbers depend on a seed.\n");
    printf ( "  You need to use this C code for OpenMP program that using random numbers.\n" );
    printf ( "  You need to insure that each OpenMP thread\n");
    printf ( "  starts with a different seed.\n" );
    printf ( "\n" );
    printf ( "  Number of processors available = %d\n", 1);
    printf ( "  Number of threads =              %d\n", 1 );

    n = 100;
    seed = 123456789;
    monte_carlo (n, &seed);
/*
  Terminate.
*/
    printf ( "\n" );
    printf ( "RANDOM\n");
    printf ( "  Normal end of execution.\n");
    printf ( "\n" );
    timestamp ( );
    return 0;
}

void monte_carlo (int n, int *seed)
{
    int i;
    double *x;
    x = (double *) malloc (n * sizeof (double));

    printf("\n");
    printf("  Thread   Seed       I       X(I)\n");
    printf("\n");
    omp_set_num_threads(4);
#pragma omp parallel
    {
        int my_id = omp_get_thread_num();
        int my_seed = *seed + my_id * 100; 

#pragma omp for
        for (i = 0; i < n; i++)
        {
            x[i] = random_value(&my_seed);
            printf("  %6d  %12d  %6d  %14.6g\n", my_id, my_seed, i, x[i]);
        }
    }

    free(x);
}

double random_value (int *seed)
{
    double r;
    *seed = ( *seed % 65536 );
    *seed = ( ( 3125 * *seed ) % 65536 );
    r = ( double ) ( *seed ) / 65536.0;
    return r;
}

void timestamp ()
{
# define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    const struct tm *tm;
    time_t now;

    now = time ( NULL );
    tm = localtime ( &now );

    strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

    printf ( "%s\n", time_buffer );

    return;
# undef TIME_SIZE
}
