#include <stdio.h>
main()
{
   float x;
   double y;
   scanf("%f%lf", &x, &y);
   printf("x=%f\ny=%f\n", x, y);
   printf("x=%e\ny=%e\n", x, y);
   printf("x=%g\ny=%g\n", x, y);
}
