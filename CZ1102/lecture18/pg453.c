#include <stdio.h>
main()
{
   char s[3], t[30], u[30];
   int i;
   float f;

   scanf("%c", s);
   scanf(" %2d%4s%4f", &i, t, &f);
   scanf(" \"%[^\"]\"", u);


   printf("%d, %f, %c, %s, %s\n", i, f, s[0], t, u);
}
