#include <stdio.h>

main()
{
   int i, n, resp;
   float sum;

   do {
      printf("\nAgain (1 = yes, 0 = no)? ");
      scanf("%d", &resp);
      if (resp) {
         printf("n = ? ");
         scanf("%d", &n);
         sum = 0.0;
         for(i = n; i > 0; i--)
            sum += 
            1/((float) i * (float) i);
         printf("Sum = %f\n", sum);
      }
   } while (resp);
}
