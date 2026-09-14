int length(char s[])
{
   int cnt;
   for(cnt = 0; s[cnt] != '\0'; ++cnt)
      ;
   return cnt;
}

#include <stdio.h>
main()
{
   printf("\"otter\"\t%d\n", length("otter"));
   printf("\"\"\t%d\n", length(""));
   printf("\"a\"\t%d\n", length("a"));
}
