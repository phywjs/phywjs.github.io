#include <stdio.h>
main()
{
   int i, j, k;
   
   i = 1;  j = 0;  k = 3;

   printf("%d, ", k<<i);
   printf("%d, ", !j);
   printf("%d, ", ~j);
   printf("%d, ", i & k);
   printf("%d, ", i && k);
   printf("%d, ", i ^ j);
   printf("%d, ", i | k);
   printf("%d, ", i || k);
   printf("\n");
}
