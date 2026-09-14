#include <stdio.h>
main()
{
   int i, j, k, x, y, z;
   k = -3;
   x = 2; y  = 100; z = 7;

   for(i = 0; i > k; --i) {
      j = !i;
      if (j) 
         continue;
      else 
         break; 
      printf("%d\n", x/y);
   }
   if (y > z) 
      printf("%d, %d, %c\n",
         x%y, z--, y);
}      
