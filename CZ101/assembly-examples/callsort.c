/* Main program to test the sort assembly procedures. */
/* Compile with   cc callsort.c sort.s swap.s         */
/* The assembly programs can be compiled only on real MIPS machine. */

#include <stdio.h>

main()
{
   int n, i, v[10];
 
   v[0] = 0; v[1] = 5; v[2] = 7; v[3] = 2;
   v[4] = 1; v[5] = 3; v[6] = 4; v[7] = 9;
   v[8] = 8; v[9] = 6;
   n = 10;
   
   printf("Before sort:\n");
   for(i = 0; i < n; ++i)
      printf("  v[%d] = %d\n", i, v[i]);
    
   sort(v, n);

   printf("After sort:\n");
   for(i = 0; i < n; ++i)
      printf("  v[%d] = %d\n", i, v[i]);
}
