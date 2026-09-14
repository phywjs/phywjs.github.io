#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct elephant {
   char name[10];
   struct elephant *next;
} ELEPHANT;

void print_elephants(const ELEPHANT* ptr);

main()
{
   ELEPHANT e1, e2, e3, *start;

   strcpy(e1.name, "Edna");
   strcpy(e2.name, "Elmer");
   strcpy(e3.name, "Eloise");
   
   start = &e1;
   e1.next = &e2;
   e2.next = &e3;
   e3.next = NULL;

   print_elephants(start);
}

void print_elephants(const ELEPHANT* ptr)
{
   int count = 1;
   printf("\n");
   while(ptr != NULL) {
      printf("\nElephant numer %d is %s.",
              count++, ptr -> name);
      ptr = ptr -> next;
   }
}
   
