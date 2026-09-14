/* This program computes the first max_coeff + 1 terms of the
   discrete Fourier transform (h1, h2) of the function g, which
   is represented as the array
      g[0], g[1], ..., g[r-1]
   h1 is the cosine component and h2 is the sine component
   of the Fourier transform; h1 and h2 are represented as the
   arrays:
      h1[0], h1[1], ..., h1[max_coeff]
      h2[0], h2[1], ..., h2[max_coeff]
   The program prints the amplitude spectrum
      sqrt( h1[i]^2 + h2[i]^2)
   for i = 1, ..., max_coeff.

   The input must be of the form

   r
   g[0] g[1]  ... g[r-1]
   max_coeff
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
     

#define MAX 300

main()
{
   int r, i, s, t, max_coeff;
   float scale;
   float h1[MAX], h2[MAX], g[MAX];
   
   scanf("%d", &r);
   if (r<= 0 || r > MAX) {
      printf("Invalid resolution!\n");
      exit(1);
   }
   scale = 2 * M_PI/r;
   
   for(i = 0; i < r; ++i)
      scanf("%f", &g[i]);
   scanf("%d", &max_coeff);

   for(s = 0; s <= max_coeff; ++s) {
      h1[s] = 0;
      h2[s] = 0;
      for(t = 0; t < r; ++t) {
         h1[s] += g[t] * cos(scale*s*t);
         h2[s] += g[t] * sin(scale*s*t);
      }
   }
   printf("\n\nSpectrum:\n");
   for(i = 1; i <= max_coeff; ++i)
      printf("%f\n",
         sqrt(h1[i]*h1[i] + h2[i]*h2[i]) );
}
