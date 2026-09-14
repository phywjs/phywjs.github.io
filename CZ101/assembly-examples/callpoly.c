/* Testing the polynomial function */

#include <stdio.h>

float poly(float a[], int n, float x);      /* prototype necessary */

main()
{
   float a[10], x, f;
   int  n;

   a[0] = 1.0;
   a[1] = 0.0;
   a[2] = 3.0;
   a[3] = 2.0;
   n = 3;
   x = 0.25; 
   f = poly(a, n, x);
   printf("f = %f\n", f);
}
