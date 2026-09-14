#include <stdio.h>
main()
{
   int i, j;
   float x, y, z;

   i = 1;
   j = 3;
   x = i/j;
   y = (float) i / j;
   z = (float) i / (float) j;

   printf("x=%f,\ny=%f,\nz=%f.\n", x, y, z);
}
   
