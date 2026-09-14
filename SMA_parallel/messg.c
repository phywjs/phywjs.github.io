#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char** argv)
{
   char msg[20];
   int myrank, np, tag = 99;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  
   if(np < 2) goto End;

   if(myrank == 0) {
      strcpy(msg, "Hello there");
      MPI_Send(msg, strlen(msg)+1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
      printf("from node %d, message: %s sent.\n", myrank, msg);
   } else if (myrank == 1) {
      MPI_Recv(msg, 20, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
      printf("from node %d, message: %s received.\n", myrank, msg);
   }

End:
   MPI_Finalize();
   return 0;
}
