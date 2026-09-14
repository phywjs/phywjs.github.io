#include <stdio.h>
main()
{
   union char_float {
      float x;
      char c[4];
   };

   union char_float buff;
   int i;
   float y = -824.718023;

   buff.x = y;
   for(i = 0; i < 4; i++)
      printf("%x ", buff.c[i]);
}
/*
fffffff4 2d 4e ffffffc4 
*/
