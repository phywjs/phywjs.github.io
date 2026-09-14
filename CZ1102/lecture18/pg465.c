#include <stdio.h>
main()
{
   float v[5], r[5];
   FILE *fp;

   v[0] = 13.15;
   v[1] = 1.0;

   fp = fopen("test.dat", "wb");
   fwrite(v, sizeof(float), 2, fp);
   fclose(fp);

   fp = fopen("test.dat", "rb");
   fread(r, sizeof(float), 2, fp);
   fclose(fp);

   printf("%f %f\n", r[0], r[1]);
}


