#include <stdio.h>
void print_names(char p[][11], int n);

main()
{
   char phys[4][11] = {"Newton", "Einstein",
                       "Fermi", "Heisenberg"};
   print_names(phys, 4);
}

void print_names(char p[][11], int n)
{
   int i;
   for(i = 0; i < n; i++)
      printf("%s\n", *p++);
}
