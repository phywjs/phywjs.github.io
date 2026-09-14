/* This program illustrates character strings.  It uses three
   arrays of characters to hold the names of ther Three Stooges,
   entering the names in three different ways; it then prints
   all three names using printf and the string conversion
   specification %s. */

#include <stdio.h>

main()
{
   char stooge1[4];
   char stooge2[6];
   char stooge3[6] = "Larry";

   stooge1[0] = 'M';
   stooge1[1] = 'o';
   stooge1[2] = 'e';
   stooge1[3] = '\0';

   printf("\n\n\tPlease enter the second Stooge's name: ");
   scanf("%s", stooge2);

   printf("\n%s", stooge1);
   printf("\n%s", stooge2);
   printf("\n%s", stooge3);
}
