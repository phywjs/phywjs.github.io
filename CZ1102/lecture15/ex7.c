#include <stdio.h>
void func(int i, int *p);

main()
{
   int i = 1, j = 2;
   
   printf("%d %d\n", i, j);
   func(i,&j);
   printf("%d %d\n", i, j);
}

void func(int i, int *p)
{
  ++i;
  *p = *p + 1;
}

/*
1 2
1 3
*/
