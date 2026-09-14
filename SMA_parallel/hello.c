#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char** argv) {
   int myid;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   
   printf("hello from %d\n", myid);
   system("uname -a");

   MPI_Finalize();

   return 0;
}

