#include <stdio.h>
main()
{
   int x, y, z, u, v;
   int j1, j2, j3, j4, j5;

   x = -1; y = 3; z = 0; u = 100; v = 7;

   j1 = v/u - x % y;
   j2 = y % v % y;
   j3 = x && y || u < v;
   j4 = x <= !y || z;
   j5 = !!x == x;

   printf("%d %d %d %d %d\n", j1, j2, j3, j4, j5);
}
