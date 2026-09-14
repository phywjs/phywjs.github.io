/* this program test tabbility of computing power of golden mean  
   phi^n, phi = (sqrt(5)-1)/2
*/
#include <stdio.h>
#include <math.h>

int main()
{
   float p, p0, f0, f1, f2;
   int i;

   p0 = 0.5*(sqrt(5.0)-1.0);   /* approx equal to 0.61803 */
   
   p = 1.0;
   f0 = 1.0;
   f1 = p0;
   for(i = 0; i < 30; ++i) {
      printf("n=%2d, phi^n by mul= %10.8g, by pow=%10.8g, by sub= %10.8g\n",
              i, p, pow(p0,i), f0);
      p = p * p0;
      f2 = f0 - f1;
      f0 = f1; 
      f1 = f2;
   }

   return 0;
}
