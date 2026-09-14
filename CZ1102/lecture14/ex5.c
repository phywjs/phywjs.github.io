#include <stdio.h>
#include <string.h>

main()
{
   char s[] = "We study";
   char t[] = "uoga uoga";
   char v[15] = "did";

   printf("%d\n", strlen(s));
   printf("%d\n", strcmp(s,t));
   printf("%s\n", strcpy(s,v));
   printf("%s\t%s\n", s, v);
   printf("%s\n", strcat(v, t));
   printf("%s\n", strstr(t, "og"));
}
