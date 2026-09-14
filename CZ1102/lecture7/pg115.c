#include <stdio.h>
main()
{
   int i,   /* an int in the standard input */
       after_i,       /* int that follows i */
       eof_flag;              /* signal EOF */
   
   scanf("%d", &i);
   while( (eof_flag = scanf("%d", &after_i) )
            != EOF ) {
      if (i > after_i)         /* order OK? */
         break;   /* if not, terminate loop */
      else               /* if OK, update i */
         i = after_i;
   }
   if (eof_flag != EOF)
      printf("\nfile is not sorted\n");
   else
      printf("\nfile is sorted\n");
}
      
