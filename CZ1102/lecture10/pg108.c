/* This program prints the names of lake and a bar graph to
   show the volume of the lakes.  */

#include <stdio.h>

main()
{
   char c;
   int i, vol;

   while( scanf(" %c", &c) != EOF) {
      do {
         printf("%c", c);
         scanf("%c", &c);
      } while( c != '\n');
      printf("\n");
      scanf("%d", &vol);
      for(i = 1; i <= vol; i++)
         printf("*");
      printf("\n\n");
   }
}
