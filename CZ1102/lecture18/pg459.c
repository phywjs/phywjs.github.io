#include <stdio.h>
main()
{
   char big_A = 'A';
   char s[] = "look";
   int sevens = 7777777;
   double pi = 3.14159265;

   printf("\n1:%d", big_A);
   printf("\n2:%c", 61);
   printf("\n3:%x", sevens);
   printf("\n4:%c", big_A);
   printf("\n5:%5c", big_A);
   printf("\n6:%-5c", big_A);
   printf("\n7:%10s", s);
   printf("\n8:%-10s", s);
   printf("\n9:%f", pi);
   printf("\n10:%5.3f", pi);
   printf("\n11:%.15f", pi);
   printf("\n12:%-15.3f", pi);
   printf("\n12:%15.3f", pi);
}

