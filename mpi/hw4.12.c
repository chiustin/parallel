/* This program uses Simpson's Rule t o compute p i .*/
#include "mpi.h"
#include <math.h>
#include <stdio.h>

#define n 50

double f (int i)
{
    double x;
    
    x = (double) i / (double) n;
    
    return 4.0 / (1.0 + x * x ) ;
}

int main (int argc, char *argv[])
{
    double global_area; /* Global area */
    double area;
    int    high_value;   /* Highest value on this proc */
    int    low_value;    /* Lowest value on this proc */
    int    i;
    int    id;           /* Process ID number */
    int    p;            /* Number of processes */
    double elapsed_time; /* Parallel execution time */
    
    MPI_Status status;   /* Result of receive */
    
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    
    if (!id) low_value = 1 + id * (n / 2 - 1) / p;
    else low_value = 2 + id * (n / 2 - 1) / p;
    high_value = 1 + (id + 1) * (n / 2 - 1) / p;
    
    area = 0;
    for (i = low_value; i <= high_value; i++)
        area += 4.0 * f (2 * i - 1) + 2 * f (2 * i);
    
    MPI_Reduce (&area, &global_area, 1, MPI_DOUBLE, MPI_SUM,
                0, MPI_COMM_WORLD);
    
    /* Stop the timer */
    
    elapsed_time += MPI_Wtime();
    
    if (!id) {
        global_area = (f(0) - f(n) + global_area) / (3.0 * n);
        printf ("Approximation of pi: %13.11f\n", global_area);
        printf ("Reduction (%d) %10.6f\n", p, elapsed_time);
    }
    MPI_Finalize ();
    return 0;
 }
