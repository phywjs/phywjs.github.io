#include <stdio.h>
main()
{
   int x;     /* a int value */
   int *p;    /* pointer to int */
   int **pp;   /* pointer to pointer to int */

   x = 5;
   p = &x;
   pp = &p;

   printf("%d %d %d\n", x, *p, **pp);
} 


