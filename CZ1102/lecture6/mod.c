#include <stdio.h>
main()
{
   int a, b, q, r;

   printf("   a     b    a/b    a%%b\n");
   while (scanf("%d%d", &a, &b) != EOF) {
      q = a / b;
      r = a % b;
      printf("%5d %5d %5d %5d\n", a, b, q, r);
   }
}
   

