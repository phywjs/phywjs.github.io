#include <stdio.h>
main()
{
   char alpha[]={'A', 'B', 'C', 'D', 'E'};
   char x, *p1, *p2;

   p1 = alpha;
   p2 = p1 + 2;
   x = *p2;
   printf("%c%c%c", x, *p1, *(p2-1));
} 
