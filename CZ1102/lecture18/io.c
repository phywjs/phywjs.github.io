#include "defines.h"

void update_lhs(void);

extern char buffer[MaxExp+2];
extern int lhs;

void prompt(void)
{
   printf("\n%d ", lhs);
}

void input(void)
{
   char * nl;
   fgets(buffer, MaxExp, stdin);
   if ( (nl = strchr(buffer, '\n') ) != NULL)
      *nl = '\0';     /* eliminate new line */
}

void output(void)
{
   printf("%d %s == ", lhs, buffer);
   update_lhs();
   printf("%d\n", lhs);
}
