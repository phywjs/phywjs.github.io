/* This program is used to call the assembly program arraysum,
   which performs the summation, A[0] + A[1] + A[2] + ... + A[n-1]
   and put the result in B[0].
   Note that in C, B[0] refers to the value of the 0th element, while
   B (without square brackets) refers to the starting address of 
   the array B[].
*/
#include <stdio.h>

main()
{
   int A[10], B[4];    /* claim 10 elements for A[] and 4 for B[]. */
   int n;  

   A[0] = 1;
   A[1] = 4;
   A[2] = -2;
   A[3] = 7;
   n = 4;

   arraysum(A, n, B);

   printf("%d + %d + %d + %d = %d\n",
      A[0], A[1], A[2], A[3], B[0]);
}
