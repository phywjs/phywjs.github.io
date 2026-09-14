#include <stdio.h>
#include <string.h>

void rev(char *s);

main()
{
   char str[101];

   scanf("%s", str);
   rev(str);
   printf("%s", str);
}

void rev(char *s)
{
   char t, *e;
   e = s + strlen(s) -1;

   while(s < e) {
      t = *s;
      *s++ = *e;
      *e-- = t;
   }
}
