#include<stdio.h>
#include<mpi.h>

int main (int argc, char *argv[]) 
{ 
  int id, p;
  int m, n;

  m = 5;
  n = 5;

  int arry[m][n] = {{0,1,1,0,0},{1,1,1,0,0},
{1,1,1,1,0},{0,0,0,1,0},{1,1,1,1,0}};

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);
  MPI_Comm_size (MPI_COMM_WORLD, &p);  

   

  MPI_Finalize();

  return 0;
}
