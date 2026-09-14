#include <stdio.h>
main()
{
   enum marital_status { single, married, divorced, widowed };
   typedef enum marital_status MARITAL;

   MARITAL s1, s2;
   s1 = divorced;
   s2 = single;
   printf("\n%d\n", s1);
   printf("\n%d\n", s2);
}
