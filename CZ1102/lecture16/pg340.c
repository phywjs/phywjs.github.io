#include <stdio.h>
void f(int x, int *p);

main()
{
   int x = 998;
   int y = 998;

   f(x,&y);
   printf("%d  %d\n", x, y);
}

void f(int x, int *p)
{
   x += 2;
   *p += 2;
}
/*
998  1000
*/
