/* weighted least square, from "Numerical Recipies in C", page 665,
   with some modification.  17 Aug 2002 by Wang Jian-Sheng 

   The program read from standard input or file three columns of numbers
   for x, y, and dy and print results to standard output.  If
   errors dy for y is not unknown, you have to modify the program
   slightly.   
*/

/* supporting functions, mainly gammq, used system gammq function.
*/

#include <stdio.h>
#include <math.h>

#define MAX 100
#define ITMAX 100
#define EPS 3.0e-12
#define FPMIN 1.0e-30

void gser(double *gamser, double a, double x, double *gln)
{
   int n;
   double sum, del, ap;

   *gln = lgamma(a);
   if(x <= 0.0) {
      if(x < 0.0) fprintf(stderr, "x less than 0 in routine gser\n");
      *gamser = 0.0;
      return;
   } else {
      ap = a;
      del = sum = 1.0/a;
      for(n=1; n <= ITMAX; ++n) {
         ++ap;
         del *= x/ap;
         sum += del;
         if(fabs(del) < fabs(sum)*EPS) { 
            *gamser=sum*exp(-x+a*log(x)-(*gln));
            return;
         }
      }
      fprintf(stderr, "a too large, ITMAX too small in routine gser\n");
   }
}


void gcf(double *gammcf, double a, double x, double *gln)
{
   int i;
   double an, b, c, d, del, h;

   *gln = lgamma(a);
   b = x+1.0-a;
   c = 1.0/FPMIN;
   d = 1.0/b;
   h = d;
   for(i=1; i <= ITMAX; ++i) {
      an = -i*(i-a);
      b += 2.0;
      d = an*d+b;
      if(fabs(d) < FPMIN) d = FPMIN;
      c = b+an/c;
      if(fabs(c) < FPMIN) c = FPMIN;
      d = 1.0/d;
      del = d*c;
      h *= del;
      if(fabs(del-1.0) < EPS) break;
   }
   if(i > ITMAX) fprintf(stderr, "a too large, ITMAX too small in gcf\n");
   *gammcf = exp(-x+a*log(x)-(*gln))*h;
}


double gammq(double a, double x)
{
   double gamser, gammcf, gln;

   if(x < 0.0 || a <= 0.0) fprintf(stderr, "invalid arugment in routine gammq\n");
   if( x < (a+1.0) ) {
      gser(&gamser,a,x,&gln);
      return 1.0-gamser;
   } else {
      gcf(&gammcf,a,x,&gln);
      return gammcf;
   }
}


void fit(double x[], double y[], int n, double sig[], int mwt, 
     double *a, double *b, double *siga, double *sigb, double *chi2, double *q)
{
   double gammq(double a, double x);
   int i;
   double wt, t, sxoss, sx=0.0, sy=0.0, st2=0.0, ss, chi, sigdat;

   *b = 0.0;
   if(!mwt) {
      for(i=1; i<=n; ++i) {
         sig[i] = 1.0;
      }
   } 
   ss = 0.0;
   for(i=1; i<=n; ++i) {
      wt = 1.0/(sig[i]*sig[i]);
      ss += wt;
      sx += x[i]*wt;
      sy += y[i]*wt;
   }
   sxoss = sx/ss;
   for(i=1; i<=n; ++i) {
      t = (x[i]-sxoss)/sig[i];
      st2 += t*t;
      *b += t*y[i]/sig[i];
   }  
   *b /= st2;
   *a = (sy-sx*(*b))/ss;
   *siga = sqrt((1.0+sx*sx/(ss*st2))/ss);
   *sigb = sqrt(1.0/st2);
   *chi2 = 0.0;
   for(i=1;i<=n;++i) {
      chi = (y[i]-(*a)-(*b)*x[i])/sig[i];
      *chi2 += chi*chi;
   }
   if(mwt) {
      *q = gammq(0.5*(n-2), 0.5*(*chi2));
   } else {
      *q = 1.0;
      sigdat = sqrt((*chi2)/(n-2));
      *siga *= sigdat;
      *sigb *= sigdat;
   }
}


main(int argc, char *argv[])
{
   double x[MAX], y[MAX], sig[MAX];
   int i, n;
   double a, b, siga, sigb, chi2, q;
   FILE *fp;

   if(argc == 1) {
      fp = stdin;
   } else if(argc == 2) {
      fp = fopen(argv[1], "r");
   } else {
      fprintf(stderr, "too many command line arguments!\n");
      exit(1);
   }

   i = 1;
   while(fscanf(fp, "%lf%lf%lf", &x[i], &y[i], &sig[i]) != EOF) { 
      ++i;
      if (i >= MAX) {
         fprintf(stderr, "array size to small in main()\n");
         exit(1);
      }
   }
   n = i-1;
      
   fit(x, y, n, sig, 1, 
       &a, &b, &siga, &sigb, &chi2, &q);

   printf("intercept = %g +/- %3g\n", a, siga);
   printf("slope = %g +/- %3g\n", b, sigb);
   printf("chi2 = %g, goodness-of-fit Q = %g\n", chi2, q);

}
