/* This program reads molar concentrations until end-of-file.  For
   each molar concentration mc, it computes the corresponding pH using
   the formula
         pH = - log10( mc )
   It then reports whether the solution is acidic (pH < 7) or nonacidic.
*/

#include <stdio.h>
#include <math.h>

main()
{
   float mc, ph;

   while( scanf("%f", &mc) != EOF ) {
      ph = - log10(mc);
      printf("\nMolar concentration = %e\n", mc);
      printf("pH = %f\n", ph);
      if (ph < 7.0) 
         printf("Acidic\n");
      else
         printf("Nonacidic\n");
   }
}
