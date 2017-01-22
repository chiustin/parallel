#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
  int id, npros;
  int src   = 0;
  int count = 0;
  int *data;
  double elapsed_time;
  int d = 1;
  MPI_Status status;

  data = (int *) malloc (d * sizeof (int));

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);
  MPI_Comm_size (MPI_COMM_WORLD, &npros);

  if (!id) {
    elapsed_time = - MPI_Wtime();

    do {
      src += 1;
      MPI_Send (data, d, MPI_INT, src, 0, MPI_COMM_WORLD);
//      MPI_Probe (src, 1, MPI_COMM_WORLD, &status);
      MPI_Recv (data, d, MPI_INT, src, 1, MPI_COMM_WORLD, &status);
      count += 1;
    } while (count <= npros - 2);
    
    elapsed_time += MPI_Wtime();

    printf ("Data Length %d\n", d);
    printf ("Execution time %8.6f\n", elapsed_time);
    printf("Average time %8.6f\n",elapsed_time/2);
  }

  if (id) {
//    MPI_Probe (0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(data,d,MPI_INT,0,0,MPI_COMM_WORLD,&status);
    MPI_Send(data,d,MPI_INT,0,1,MPI_COMM_WORLD);
  }
  
  MPI_Finalize();
  return 0;
}
