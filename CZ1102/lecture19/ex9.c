#include <stdio.h>
main()
{
   struct element {
      char name[10];
      char symbol[5];
      float aWgt;
      float mass;
   };

   struct element es[3];
   struct element e1 = {"Hydrogen", "H", 1.0, 3.0};

   es[0] = e1;

   printf("%s, %s, %f, %f\n", es[0].name, e1.symbol, es[0].aWgt, e1.mass);
}


