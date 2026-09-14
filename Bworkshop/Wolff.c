/* Wolff single-cluster algorithm for the two dimensional nearest neighbor 
   Ising model                          By Jian-Sheng Wang, February 1995.
   Compile with  

       cc -O Wolff.c -lm 
 
   -O for optimization, and -lm to link the math library.                
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
                              /* macro definitions */
#define  L  16                /* lattice linear size */
#define  N  (L*L)             /* total number of spins */
#define  Z  4                 /* coordination number = 2*d */
#define  MCTOT 500            /* total Monte Carlo steps */
#define  MCDIS 200            /* steps discarded in the beginning */
                              /* global variables */
int s[N];                     /* spin +1 or -1 */
double  p = 1.0;              /* percolation probability */

			      /* funcition prototypes */
void neighbor(int i, int nn[ ]);
void flip(int i, int s0);
void monte_carlo_steps(int n);
void energy(double *);

/*  The main program, running the Monte Carlo loop, collecting data  */

void main()
{
   int i, mc;
   double e = 0;

   p = 1 - exp( - 2/2.269);
 
   for (i = 0; i < N; ++i)     /* initialize, all spin up */
      s[i] = 1;

   for(mc = 0; mc < MCTOT; ++ mc) {
      monte_carlo_steps(5);
      if( mc >= MCDIS) 
         energy(&e);
   }
   printf("<e> =  %f\n", e/(MCTOT-MCDIS)/N);
}

/* This function monte_carlo_steps performs n cluster flips, eqivalent to
few Monte Carlo steps in standard single-spin-flip algorithms.
It picks a seed site at random and calls the flip function to generate
one cluster.  N is total number of spin.  They are macro definitions. */

void monte_carlo_steps(int n)
{
   int i, k;

   for(k = 0; k < n; ++k) {
      i = drand48() * (double) N;
      flip(i, s[i]);
   }
}

/*  Perform a Wolff single cluster flip. s[], p, and Z are passed globally.
The first argument i of flip function is the site to be flipped, the
second argument is the spin of the cluster before flipping. */

void flip(int i, int s0)
{
   int j, nn[Z];

   s[i] = - s0;                    /* flip the spin immediately */
   neighbor(i, nn);                /* find nearest neighbor of i */
   for(j = 0; j < Z; ++j)          /* flip the neighbor if ...  */
      if(s0 == s[nn[j]] && drand48() < p)
         flip(nn[j], s0);
}

/* Neighbor returns in the array nn[ ] the neighbor sites of i.  The sites
are labelled sequentially, starting from 0.  It works for any hypercubic
lattice.  Z (=2*D) is the coordination number, passed as a macro defintion.
L is linear size, also passed as a macro definition. */

void neighbor(int i, int nn[ ])
{
   int j, r, p, q;

   r = i;
   p = 1 - L;
   q = 1;

   for(j = 0; j < Z; j += 2) {
      nn[j] = (r + 1) % L == 0 ? i + p : i + q;
      nn[j+1]     = r % L == 0 ? i - p : i - q;
      r = r/L;
      p *= L;
      q *= L;
   }
}

/* This function calculate the energy of the configuration s[], 
   in fact, it is the negative of the energy.  */

void energy(double *e)
{
   int i, j, ie = 0; 
   int nn[Z];
   
   for(i = 0; i < N; ++i) {
      neighbor(i, nn);             /* find the neighbor of center site */
      for(j = 0; j < Z; j += 2)    /* look at positive direction only */
         ie += s[i]*s[nn[j]];      /* add nearest neighbor interaction */
   }
   *e += ie;                       /* accumulant energy */
}

