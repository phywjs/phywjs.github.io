#include <stdio.h>
main()
{
   int x = 1;

   printf("%d\n", x);

   {
      int x=2, y=3;

      printf("%d %d\n", ++x, y);
   }
   printf("%d\n", x);
}
