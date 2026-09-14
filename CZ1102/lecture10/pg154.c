#include <stdio.h>

int sum(int i, int j);

main()
{
   int a, b, res;

   a = 1; b = 2;
   res = sum(a, b);
   printf("%d + %d = %d\n", a, b, res);
}

int sum(int i, int j)
{
   int k;

   k = i + j;
   return k;
}
