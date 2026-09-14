/* a number of sorting routines */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* The straight insertion sort, NR page 330 */
void piksrt(int n, float arr[])
{
   int i, j;
   float a;

   for(j = 2; j <=n; ++j) {
      a = arr[j];
      i = j-1;
      while(i > 0 && arr[i] > a) {
         arr[i+1] = arr[i];
         i--;
      }
      arr[i+1] = a;
   }
}


/* compare function for qsort */
int compare(const void *a, const void *b)
{
   float d;

   d = *( (float *) a)  - *( (float *) b );
   if(d > 0.0) {
      return 1;
   } else if (d < 0.0) {
      return -1;
   } else {
      return 0;
   }
}

/* heap sort, NR page 337-338, ra[] index from 1 */
void hpsort(unsigned long n, float ra[])
{
   unsigned long i, ir, j, l;
   float rra;

   if(n < 2) return;
   l = (n >> 1) + 1;
   ir = n;
   for( ; ; ) {
      if(l > 1) {
         rra = ra[--l];
      } else {
         rra = ra[ir];
         ra[ir] = ra[1];
         if(--ir == 1) {
            ra[1] = rra;
            break;
         }
      }
      i = l;
      j = l + l;
      while(j <= ir) {
         if(j < ir && ra[j] < ra[j+1]) j++;
         if(rra < ra[j]) {
            ra[i] = ra[j];
            i = j;
            j <<= 1;
         } else j=ir+1;
      }
      ra[i] = rra;
   }
}


int main()
{
   float *a, *b, *c;
   int i, n;
   clock_t t0, t1, t2, t3;

   printf("enter n:\n");
   scanf("%d", &n);

   a = malloc(n*sizeof(float));
   b = malloc(n*sizeof(float));
   c = malloc(n*sizeof(float));
  
   for(i = 0; i < n; ++i) {
      c[i] = b[i] = a[i] = rand()/(float) RAND_MAX;
   }

   t0 = clock();
   piksrt(n, a-1);
   
   t1 = clock();
   qsort(b, n, sizeof(float), compare);

   t2 = clock();
   hpsort(n, c-1);
   t3 = clock();

   for(i = 0; i < n; ++i) {
      if(a[i] != b[i] || a[i] != c[i]) { 
         printf("result not equal!");
         printf("i=%d, a=%f, b=%f, c=%f\n", i, a[i], b[i], c[i]);
      }
   }
   printf("CPU time(sec) insert= %f,  qsort= %f, hpsort= %f\n",
      (double) (t1-t0)/CLOCKS_PER_SEC, (double) (t2-t1)/CLOCKS_PER_SEC, 
      (double) (t3-t2)/CLOCKS_PER_SEC); 

   return 0; 

}
