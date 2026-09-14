#include <stdio.h>
int p(void);

main()
{
   extern int cnt;
   int k;

   cnt = -10;
   k = p();
   printf("%d\n", k);
   ++cnt;
   k = p();
   printf("%d\n", k);
}

int cnt = 0;

int p(void)
{
   return ++cnt;
}
 
