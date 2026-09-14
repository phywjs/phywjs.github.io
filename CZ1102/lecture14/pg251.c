#include <stdio.h>

int sum(int b[], int n)
{
   int i, res;

   res = 0;
   for(i = 0; i < n; ++i)
      res += b[i];
   return res;
}

main()
{
   int a[] = {0, 1, 2, 3, 4, 5, 6};

   printf("%d\n", sum(a, 2));
   printf("%d\n", sum(a, 4));
}
