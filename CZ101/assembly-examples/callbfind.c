/* You may use this C program to test your assembly program in lab 1, (3.34).
   You need not understand the detail of the program, but your output 
   of running this program should be  
 
   Find b in imbibe.

*/ 

#include <stdio.h>

char *bfind(char s[]);

main()
{
   char s[7], *t;                        /* Here t is declared as a pointer */

   s[0] = 'i';
   s[1] = 'm';
   s[2] = 'b';
   s[3] = 'i';
   s[4] = 'b';
   s[5] = 'e';
   s[6] = 0;
   t = bfind(s);

   printf("Find %c in %s.\n", *t, s);     /* value at address t is printed */
}
