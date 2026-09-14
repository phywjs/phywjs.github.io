/* The C program to test your assembly code bcopy.s for lab question 1 (3.31).
   If this program is named callbcopy.c and the assembly program is called 
   bcopy.s, then compile with 

       cc callbcopy.c bcopy.s

   Run with a.out
*/

#include <stdio.h>

main()
{
   int count;                          /* count of the number of characters */
   char s[4], t[4];         /* declare two arrays of chararacters of size 4 */

   s[0] ='A';                  /* initialize array s[] as 'A', 'B', 'C', 0. */
   s[1] ='B'; 
   s[2] ='C'; 
   s[3] = 0;

   count = bcopy(s, t);                 /* copy every element in s[] to t[] */
                                               
   printf("count = %d, s = %s, t = %s\n",       /* print out count, s and t */
           count, s, t);

   printf("\nIf your assembly code is correct, your output above should be\ncount = 3, s = ABC, t = ABC\n");

}
