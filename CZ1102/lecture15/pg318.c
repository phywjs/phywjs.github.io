#include <stdio.h>
main()
{
   char c='A';
   char *p;
   char **p2;
   char ***p3;

   p = &c;
   p2 = &p;
   p3 = &p2;

   printf("%c%c%c%c\n", c, *p, **p2, ***p3);
   printf("%d\t%p\n", c, &c);
   printf("%p\t%p\n", p, &p);
   printf("%p\t%p\n", p2, &p2);
   printf("%p\t%p\n", p3, &p3);
}
