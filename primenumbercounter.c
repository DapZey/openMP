#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "omp.h"

int main (int argc, char *argv[]);
void prime_number_sweep (int n_lo, int n_hi, int n_factor);
int prime_number (int n);

int main ( int argc, char *argv[] )
{
    omp_set_num_threads(4); // Set number of threads to 4

    int n_factor;
    int n_hi;
    int n_lo;

    printf ( "\n" );
    printf ( "PRIME\n" );
    printf ( "  Parallelized with OpenMP\n" );

    printf ( "\n" );
    printf ( "  Number of processors available = %d\n", omp_get_num_procs() );
    printf ( "  Number of threads =              %d\n", omp_get_max_threads() );

    n_lo = 2;
    n_hi = 10;
    n_factor = 1;

    prime_number_sweep (n_lo, n_hi, n_factor);

    /*
    Terminate.
    */
    printf ( "\n" );
    printf ( "PRIME\n" );
    printf ( "  Normal end of execution.\n" );

    return 0;
}


void prime_number_sweep ( int n_lo, int n_hi, int n_factor )
{
    printf ( "\n" );
    printf ( "TEST01\n" );
    printf ( "  Call PRIME_NUMBER to count the primes from 1 to N.\n" );
    printf ( "\n" );
    printf ( "         N        Pi          Time\n" );
    printf ( "\n" );

#pragma omp parallel for
    for (int n = n_lo; n <= n_hi; n += n_factor)
    {
        time_t now;
        struct tm *tm;
        char time_buffer[40];

        now = time(NULL);
        tm = localtime(&now);

        double wtime = omp_get_wtime();
        int primes = prime_number(n);
        wtime = omp_get_wtime() - wtime;

        strftime(time_buffer, 40, "%d %B %Y %I:%M:%S %p", tm);

#pragma omp critical
        printf("  %8d  %8d  %s (Time: %f sec)\n", n, primes, time_buffer, wtime);
    }
}


int prime_number ( int n )
{
    int total = 0;

#pragma omp parallel for reduction(+:total)
    for (int i = 2; i <= n; i++)
    {
        int prime = 1;

        for (int j = 2; j * j <= i; j++) 
        {
            if (i % j == 0)
            {
                prime = 0;
                break;
            }
        }
        total += prime;
    }
    return total;
}
