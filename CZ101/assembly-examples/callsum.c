/* C program to test the sum assembly procedure sum */

#include <stdio.h>
int sum(int n);

main()
{
   int a, b, c, d, e;

   a = sum(-1);
   b = sum(0);
   c = sum(1);
   d = sum(3);
   e = sum(10);
   printf("%d %d %d %d %d\n", a, b, c, d, e);
}
