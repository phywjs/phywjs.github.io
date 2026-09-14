#include <stdio.h>
#include <stdlib.h>

int echo_line(void);
void print_stars(int vol);

main()
{
   int value;
   while( echo_line() != EOF) {
      scanf("%d", &value);
      print_stars(value);
   }
   return EXIT_SUCCESS;
}

int echo_line(void)
{
   char c;
   if (scanf(" %c", &c) == EOF)
      return EOF;
   for( ; ; ) {
      putchar(c);
      if(c == '\n')
         return c;
      c = getchar();
   }
}
 

void print_stars(int vol)
{
   while(vol-- > 0)
      putchar('*');
   printf("\n\n");
}
 
