/* read positive number, skip nonpositive number */
#include <stdio.h>
main()
{
   float x, sum;
   int count;

   sum = 0.0;
   count = 0;

   while(scanf("%f", &x) != EOF) {
      if (x <= 0.0)
         continue;  /* skip nonpositive */
      sum += x;
      count++;
   }
   if (count > 0)
      printf("\naverage=%f\n", sum/count);
   else
      printf("\nno positive number read\n");
}
