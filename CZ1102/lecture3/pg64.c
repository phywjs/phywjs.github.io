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
   FILE *fin, *fout;

   fin = fopen("ph.in", "r");
   fout = fopen("ph.out", "w");

   while( fscanf(fin, "%f", &mc) != EOF ) {
      ph = - log10(mc);
      fprintf(fout, "\nMolar concentration = %e\n", mc);
      fprintf(fout, "pH = %f\n", ph);
      if (ph < 7.0) 
         fprintf(fout, "Acidic\n");
      else
         fprintf(fout, "Nonacidic\n");
   }
}
