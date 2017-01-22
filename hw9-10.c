#include "stdio.h"
#include "stdlib.h"
#include <math.h>
#include <mpi.h>

void manager (int argc, char *argv[], int p) {
  int i, src, tag, m, terminated;
  int prime;
  int count = 0;
  MPI_Status status;
  MPI_Request request;

  m = 0;
  terminated = 0;
  do {
    MPI_Probe (MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv (&count, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    src = status.MPI_SOURCE;
    tag = status.MPI_TAG;
    m += count;
    if (m <= 8) {
      MPI_Send (&m, 1, MPI_INT, src, 1, MPI_COMM_WORLD);
    }
    else {
      MPI_Send (NULL, 0, MPI_CHAR, src, 1, MPI_COMM_WORLD);
      terminated++;
    }
  } while(terminated < p - 1);

}

void worker (int argc, char *argv[], int id, int p, long long int perfect[]) {
  int i, n, m;
  int prime, discriminant, check;
  int count = 0;
  int countpro = 0;
  MPI_Status status;
  prime =  id + 1;

  n = 1;
  m = 0;
  while (1) { 
    if (countpro) prime = prime + (p - 1);
    countpro = 1;
    check = (int) (pow(2, prime) - 1);
    discriminant = (int) sqrt(check);
    i = 2;
    m = 1;
    while (i <= discriminant) {
      if ( check % i == 0 ) {
        count = 0;
        MPI_Send (&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Probe (0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv (&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        m = 0;
        break;
      }
      i++;
    }
    if (m) {
      count = 1;
      MPI_Send (&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
      MPI_Probe (0, 1, MPI_COMM_WORLD, &status);
      MPI_Recv (&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
      perfect[count] = (pow(2, prime) - 1) * pow(2, prime - 1);
if (perfect[count] > 0)
printf("***worker find prime before send id, count, n = %d %d %lld\n", id, count, perfect[count]);
    }
    MPI_Get_count (&status, MPI_INT, &n);
    if (!n) break;
  }

}

int main (int argc, char *argv[])
{
  int id, p, n;  
  long long int perfect[11];
  MPI_Comm worker_comm; 
 
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);
  MPI_Comm_size (MPI_COMM_WORLD, &p);

  if (!id) {  
    manager (argc, argv, p);
  }

  else if (id){  
    worker (argc, argv, id, p, perfect);
  }

  MPI_Finalize();

  return 0;
}

