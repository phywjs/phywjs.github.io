#include <stdio.h>

int g = 0;
extern int k;
void fun(char c);

main()
{
  char c ='A';
  auto int i; 

  ++g;
  fun(c);
  ++g;
  fun(c+1);
  fun(c+2);
}

void fun(char c)
{
   int y = 3;
   static int x=0;

   putchar(c);
   printf(" x=%d, y=%d, g=%d\n", x++, y--, ++g);
}
