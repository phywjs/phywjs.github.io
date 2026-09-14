#include <stdio.h>
#include <stdlib.h>
main()
{
   char c = 'O', d= 'H';
   char *p1, *p2, *temp;

   p1 = &c;
   p2 = &d;
   printf("%c%c", *p1, *p2);
   temp = p1;
   p1 = p2;
   p2 = temp;
   printf("%c%c", *p1, *p2);
}
