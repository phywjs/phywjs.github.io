/* Test features of IEEE 754 floating point arithematic */
#include <stdio.h>
int main()
{
   float x, y, z;
   int *p;

   x = 1.0e-30;
   y = 1.0e+30;
   z = ((2*x + y) - y) - x;

   printf("((2*%g+%g)-%g)-%g = %g\n", 
            x, y, y, x, z);

   p = &x;
   x = 0.0;
   printf("x= %f, hex= %X\n", x, *p);
   x = -0.0;
   printf("x= %f, hex= %X\n", x, *p);
   x = 1.0;
   printf("x= %f, hex= %X\n", x, *p);
   x = 0.1;
   printf("x= %f, hex= %X\n", x, *p);
   x = 10.25;
   printf("x= %f, hex= %X\n", x, *p);
   x = 1.0/3.0;
   printf("x= %f, hex= %X\n", x, *p);
   x = 1.0/0.0;
   printf("x= %f, hex= %X\n", x, *p);
   x = 0.0/0.0;
   printf("x= %f, hex= %X\n", x, *p);

   return 0;
}
