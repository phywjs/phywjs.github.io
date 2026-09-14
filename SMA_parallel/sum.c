/* A simple numerical integration MPI program 

   Define a function f
*/

#include <math.h>
double f(double x) 
{
   double y;
   
   y = sin(x); 
   return y;
}

#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv)
{
   int myid, np, n, nterms;
   int ia, i, i_begin, i_end;
   double a, b, x, dx, sum, total;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);

   if(myid == 0) {
      printf("enter integration limit [a, b]\n");
      scanf("%lf%lf", &a, &b);
      printf("enter the number of sampling points n\n");
      scanf("%d", &n);
   }
   MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

   /* Given the interval [a,b], and divided by n points,
      each process will be responsible for an interval of (b-a)/np
      make sure n is a multiple of num of processes */
   n = (n/np) * np;
   nterms = n/np;
   dx = (b-a)/ (double) n;
   ia = a/dx;
   
   i_begin = ia + myid*nterms;
   i_end = ia + (myid+1)*nterms;

   /* for debugging
    printf("myid=%d, i_begin=%d, i_end=%d\n", myid, i_begin, i_end);
    */

   sum = 0.0;
   for(i = i_begin; i < i_end; ++i) {
      x = i * dx;
      sum += f(x);
   }

   MPI_Reduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   if(myid == 0) {
      total = total * dx;
      printf("Integral = %g\n", total);
   }
 
   MPI_Finalize();
   return 0;
}
