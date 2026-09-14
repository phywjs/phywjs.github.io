#include <stdio.h>
main()
{
   short int w1, w2;

   w1 = 12;
   w2 = -35;
   printf("w1 = %d\n", w1);
   printf("w2 = %d\n", w2);
   printf("~w1 = %d\n", ~w1);
   printf("~w2 = %d\n", ~w2);
   printf("w1 & w2 = %d\n", w1 & w2);
   printf("w1 & ~w2 = %d\n", w1 & ~w2);
   printf("~w1 & ~w2 = %d\n", ~w1 & ~w2);
   printf("w1 | w2 = %d\n", w1 | w2);
   printf("~(w1 | w2) = %d\n", ~(w1 | w2));
   printf("w1 ^ w2 = %d\n", w1 ^ w2);
}
