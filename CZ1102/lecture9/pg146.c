#include <stdio.h>
main()
{
   int x = 'A';
   int y = 2;
   short i, j;
  
   printf("%d\n", x >> y);
 
   i = 0x7A4E;
   j = ((i>>8) & 0x00FF) | (i<<8);

   printf("%X\n", j);
}
