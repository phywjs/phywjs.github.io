#include <stdio.h>
main()
{
   int i, j, k, x;

Label:
   i = 1;
   j = 5;
   switch (j) {
      case 1:
         k = 0; 
         break;
      case 2: 
         k = 5; 
         goto Label;
      default: 
         k = 10;
   } 
   x = (i>j) ? k : i;
  
   printf("%d %d\n", k, x);

}
