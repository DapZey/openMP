/* arraySum.c uses an array to sum the values in an input file,
 *  whose name is specified on the command-line.
 */

#include <stdio.h>      /* I/O stuff */
#include <stdlib.h>     /* calloc, etc. */

void readArray(char * fileName, double ** a, int * n);
long double sumArray(double * a, int numValues) ;

int main(int argc, char * argv[])
{
    int  howMany;
    long double sum;
    double * a;


    readArray("100k.txt", &a, &howMany);
    sum = sumArray(a, howMany);
    printf("The sum of the values in the input file is %Lg\n",
            sum);

    free(a);

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

long double sumArray(double * a, int numValues) {
    int i;
    long double result = 0.0;

    for (i = 0; i < numValues; ++i) {
        result += a[i];
    }

    return result;
}

