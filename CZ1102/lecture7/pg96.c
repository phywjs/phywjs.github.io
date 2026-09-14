/*  This program reads a series of floating-point numbers from
    the standard input and prints a statistical summary of the data
    that includes the largest value input, the smallest value input,
    the sum of all values read, the mean (average) of all values
    input, the population variance, and the standard deviations. */

#include <stdio.h>
#include <math.h>

main()
{
   float x, max, min, sum, sum2, mean, variance;
   int count;

   if( scanf("%f", &x) == EOF )
      printf(" 0 data items read\n");
   else {
      max = min = sum = x;
      count = 1;
      sum2 = x * x;
      while ( scanf("%f", &x) != EOF ) {
         ++count;
         if (x > max) max = x;
         if (x < min) min = x;
         sum += x;
         sum2 += x * x;
      }
      printf("%d data items read\n", count);
      printf("maximum value read = %f\n", max);
      printf("minimum value read = %f\n", min);
      printf("sum of all values read = %f\n", sum);
      mean = sum / count;
      printf("mean = %f\n", mean);
      variance = sum2/count - mean * mean;
      printf("variance = %f\n", variance);
      printf("standard deviation = %f\n", sqrt(variance));
   }
} 
