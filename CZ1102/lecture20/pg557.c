#include <stdio.h>
#include <stdlib.h>
main()
{
   int *ptr;

   ptr = malloc(sizeof(int));
   *ptr = 999;

   
   ptr = malloc(sizeof(int));
   *ptr = -777;
   printf("\n%d\n", *ptr);

   free(ptr);
}
