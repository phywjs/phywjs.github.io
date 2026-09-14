#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef double real;
#define SIZE_OF_REAL 8

void eigensystem(const real *h, real *epsilon, real *psi, int dim, int nov);

#define DIM  3
int main() 
{
   real h[DIM*DIM] = {1,2,-1, 2,5,-3, -1,-3,4};
   real epsilon[DIM];
   real psi[DIM*DIM];

   eigensystem(h, epsilon, psi, DIM, DIM);

   printf("eps[i] = %f, %f, %f\n", epsilon[0], epsilon[1], epsilon[2]);
   printf("eigen vector[1] = (%f, %f, %f)\n", psi[0], psi[1], psi[2]);
   printf("eigen vector[2] = (%f, %f, %f)\n", psi[3], psi[4], psi[5]);
   printf("eigen vector[3] = (%f, %f, %f)\n", psi[6], psi[7], psi[8]);
   
   return 0;
}
