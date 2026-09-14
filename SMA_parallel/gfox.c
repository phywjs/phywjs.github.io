/* a simple implementation of Fox algorithm,
   one element per process.  In a full implementation A, B, C, T
   will be submatrices.
*/

#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
   double A, B, C, T;
   double *Ain, *Bin, *Cout; 
   int i, k, n, p, myid;
   int periodic, source, dest, kb;
   MPI_Comm row, col, ring;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &p);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);

   if(myid == 0) {
      printf("enter matrix dimension n\n");
      scanf("%d", &n);
      Ain = (double *) malloc(n*n*sizeof(double));
      Bin = (double *) malloc(n*n*sizeof(double));
      Cout = (double *) malloc(n*n*sizeof(double));
      printf("enter n^2 element of Ain (row-wise)\n");
      for(i = 0; i < n*n; ++i) {
         scanf("%lf", &Ain[i]);
      }
      printf("enter n^2 element of Bin (row-wise)\n");
      for(i = 0; i < n*n; ++i) {
         scanf("%lf", &Bin[i]);
      }
   }
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   if(p != n*n) {
     if(myid == 0) {
        printf("Must use %d processes\n", n*n);
     }
     goto End;
   }
   /* distribute the matrix a_{ij} to node i*n+j */
   MPI_Scatter(Ain, 1, MPI_DOUBLE, &A, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Scatter(Bin, 1, MPI_DOUBLE, &B, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
 
   /* create row and column communicators */
   MPI_Comm_split(MPI_COMM_WORLD, myid/n, myid, &row);
   MPI_Comm_split(MPI_COMM_WORLD, myid%n, myid, &col);
   /* define 1D topology (for the column roll operation only) */
   periodic = 1;
   MPI_Cart_create(col, 1, &n, &periodic, 0, &ring);
   MPI_Cart_shift(ring, 0, -1, &source, &dest);

   /* do multiplication in three steps */
   C = 0.0;
   for(k = 0; k < n; ++k) {
      /* (1) broadcast to the whole row */
      i = myid/n;       /* row number of matrix */
      kb = (i+k)%n;     /* this process broadcast */
      if(myid == kb + i*n) {
         T = A;
      }
      MPI_Bcast(&T, 1, MPI_DOUBLE, kb, row);
      /* (2) multiply */
      C += T * B;
      /* (3) shift or roll the columns */
      MPI_Sendrecv_replace(&B, 1, MPI_DOUBLE, dest, 0, source, 0, col, &status);
   }

   /* send back to root 0 */
   MPI_Gather(&C, 1, MPI_DOUBLE, Cout, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   if(myid == 0) {
      printf("C = A*B = \n");
      for(i = 0; i < n*n; ++i) {
         printf("%g ", Cout[i]);
         if(i%n == (n-1)) printf("\n");
      } 
   }

End:
   MPI_Finalize();
   return 0;
}
