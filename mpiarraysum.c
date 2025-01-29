/* arraySum.c uses an array to sum the values in an input file,
 *  whose name is specified on the command-line.
 */

#include <stdio.h>      /* I/O stuff */
#include <stdlib.h>     /* calloc, etc. */
#include <mpi.h>

void readArray(char * fileName, double ** a, int * n);
long double sumArray(double * a, int numValues, int rank, int size) ;

int main(int argc, char * argv[])
{
    int  howMany;
    long double sum;
    double * a;
    long double globalSum;
    double startTime, endTime;
    readArray("100k.txt",&a,&howMany);
    MPI_Init(&argc,&argv);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank)
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    startTime = MPI_Wtime();   
    sum = sumArray(a, howMany);
    printf("The sum of the values in the input file is %Lg\n",
            sum);

    free(a);
    MPI_Reduce(&sum.&globalSum,1,MPI_LONG_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    printf("final sum: %Lg\n", globalSum);
    endTime = MPI_Wtime();
    if (world_rank == 0)
    printf("time is: %f", endTime-startTime);
    MPI_Finalize();
    return 0;
}

/* readArray fills an array with values from a file.
 * Receive: fileName, a char*,
 *          a, the address of a pointer to an array,
 *          n, the address of an int.
 * PRE: fileName contains N, followed by N double values.
 * POST: a points to a dynamically allocated array
 *        containing the N values from fileName
 *        and n == N.
 */

void readArray(char * fileName, double ** a, int * n) {
    int count, howMany;
    double * tempA;
    FILE * fin;

    fin = fopen(fileName, "r");
    if (fin == NULL) {
        fprintf(stderr, "\n*** Unable to open input file '%s'\n\n",
                fileName);
        exit(1);
    }

    fscanf(fin, "%d", &howMany);
    tempA = (double *)(calloc(howMany, sizeof(double)));
    if (tempA == NULL) {
        fprintf(stderr, "\n*** Unable to allocate %d-length array",
                howMany);
        exit(1);
    }

    for (count = 0; count < howMany; count++)
        fscanf(fin, "%lf", &tempA[count]);

    fclose(fin);

    *n = howMany;
    *a = tempA;
}


/* sumArray sums the values in an array of doubles.
 * Receive: a, a pointer to the head of an array;
 *          numValues, the number of values in the array.
 * Return: the sum of the values in the array.
 */

long double sumArray(double * a, int numValues, int rank, int size) {
    int i;
    long double result = 0.0;

    for (i = rank; i < numValues; i+=size) {
        result += a[i];
    }

    return result;
}

