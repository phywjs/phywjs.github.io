#include <stdio.h>
main()
{
   printf("%d\n", length("This"));
}

int length(char *s)
{
   char *p = s;
   
   while(*s) 
      ++s;
   return (s-p);
}
