/* Print any number of files given on the command line */
#include <stdio.h>
#include <stdlib.h>
main(int argc, char *argv[])
{
   char record[80];
   int i;
   FILE *fp;

   printf("There are %d files\n", argc-1);
   for(i = 1; i < argc; ++i) {
      printf("File: %s\n", argv[i]);
      fp = fopen(argv[i], "r");
      while(fgets(record, 80, fp) != NULL)
         printf("%s", record);
      fclose(fp);
   }
   return EXIT_SUCCESS;
}
