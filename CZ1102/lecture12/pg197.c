#include <stdio.h>

int fact(int num);

main()
{
   int num;

   printf("\nPlease enter a number: ");
   scanf("%d", &num);
   
   if (num < 0) 
      printf("\nError, must be >= 0.\n");
   else
      printf("\n%d! = %d.\n", num, fact(num));
   
}

int fact(int n)
{
   if (n <= 1)
      return 1;
   else
      return n * fact(n-1);
}
