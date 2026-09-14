/*  This program prints all positive prime integers 
    less than or equal to the user-supplied value n.  */

#include <stdio.h>
main()
{
   int p, n, div;

   printf("\tThis program lists all primes <= n\n\n");
   printf("Input n: ");
   scanf("%d", &n);
   printf("\n\n\tPrimes <= %d:\n\n", n);
   for( p = 2; p <= n; p++) {
                     /* try to find a divisor of p */
      for(div = 2; div < p; div++) {
         if( p % div == 0)      /* found a divisor */
            break;          /* so p is not a prime */
      }
      if (div == p) /* exhausted possible divisors */
         printf("%d, ", p);       /* so p is prime */
   }
   printf("\n");
}
