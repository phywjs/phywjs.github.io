/* This C program calls the sum function written in assembly
   language.   The sum function is defined in example3b.s.
    Compile this program together with assembly program with
         cc ex3b.c example3b.s
*/

int sum(int, int);         /* prototype declaration */

main()
{
   int a, b, c;

   a = 2;
   b = 3;
   c = sum(a, b);
   printf("%d + %d = %d.\n", a, b, c);
}
