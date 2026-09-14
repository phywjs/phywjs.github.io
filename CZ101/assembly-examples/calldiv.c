/* This program test the integer division procedure */

#include <stdio.h>

void divide(int a, int b, int *c, int *d);

main()
{
   int a, b, c, d;

   a = 10; b = 3;

   printf("\n%d / %d = %d, %d %% %d = %d\n", a, b, a/b, a, b, a%b);
   divide(a, b, &c, &d);
   printf("assembly code gives %d divide %d, quotient = %d, remainder = %d\n",
      a, b, c, d);

   a = 19009091; b = 39048;

   printf("\n%d / %d = %d, %d %% %d = %d\n", a, b, a/b, a, b, a%b);
   divide(a, b, &c, &d);
   printf("assembly code gives %d divide %d, quotient = %d, remainder = %d\n",
      a, b, c, d);
}

