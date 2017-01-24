#include "mpi.h"
#include <math.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    
    int    count;        /* count */
    int    global_count; /* Global prime count */
    int    i,j,k,l,m,n;
    int    sum;          /* summation */
    int    id;           /* Process ID number */
    int    high_value;   /* Highest value on this proc */
    int    low_value;    /* Lowest value on this proc */
    int    p;            /* Number of processes */
    double elapsed_time; /* Parallel execution time */
    MPI_Status status;   /* Result of receive */
    
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    
    n = 10;
    low_value = 1 + id * (n - 1) / p;
    high_value = (id + 1) * (n - 1) / p;
    
    count = 0;
    for (i = low_value; i <=high_value; i++) {
        for (j = 0; j <= 9; j++) {
            for (k = 0; k <= 9; k++) {
                for (l = 0; l <= 9; l++) {
                    for (m = 0; m <= 9; m++) {
                        for (n = 0; n <= 9; n++) {
                            sum = i + j + k + l + m + n;
                            if (sum == 7 || sum == 11 || sum == 13) {
                                count++;
                            }
                        }
                    }
                }
            }
        }
        
    }
    
    MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM,
                0, MPI_COMM_WORLD);
    
    /* Stop the timer */
    
    elapsed_time += MPI_Wtime();

    if (!id) {
        printf ("the number of different six-digit combinations %d\n",
                ((int) pow(9,5)) - global_count);
        printf ("Reduction (%d) %10.6f\n", p, elapsed_time);
    }
    MPI_Finalize ();
    return 0;
}
