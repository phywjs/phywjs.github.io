#include <stdio.h>
main()
{
   float x, y;
   double v, w;
   x = 42.4907;
   y = 3.8872e-12;
   v = -55.23289108;
   w = -84.3002669e17;
   printf("x=%e, y=%e\n", x, y);
   printf("x=%E, y=%E\n", x, y);
   printf("x=%f, y=%f\n", x, y);
   printf("x=%g, y=%g\n", x, y);
   printf("v=%e, w=%e\n", v, w);
   printf("v=%E, w=%E\n", v, w);
   printf("v=%f, w=%f\n", v, w);
   printf("v=%g, w=%g\n", v, w);
}

