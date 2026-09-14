/* Test the multiplication assembly program */
 
#include <stdio.h>

int multiply(int, int);

main()
{
  int a, b;

   a = 4; 
   b = 7;
   printf("%d * %d = %d, multiply gives %d\n",
           a, b, a*b, multiply(a,b));
 
   a = 10904; 
   b = 41313;
   printf("%d * %d = %d, multiply gives %d\n",
           a, b, a*b, multiply(a,b));

   a = -2; 
   b = 8;
   printf("%d * %d = %d, multiply gives %d\n",
           a, b, a*b, multiply(a,b));

   a = 2; 
   b = -1;
   printf("%d * %d = %d, multiply gives %d\n",
           a, b, a*b, multiply(a,b));

   a = -1; 
   b = -1;
   printf("%d * %d = %d, multiply gives %d\n",
           a, b, a*b, multiply(a,b));

   a = 2081718345; 
   b = 2109182721;
   printf("%d * %d = %d, multiply gives %d\n",
           a, b, a*b, multiply(a,b));
}
