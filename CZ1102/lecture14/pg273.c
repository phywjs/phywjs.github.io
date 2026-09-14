#include <stdio.h>

#define MAXSIZE 20

void store(int m[][MAXSIZE], int n),
     mult(int m1[][MAXSIZE], int m2[][MAXSIZE], 
          int m3[][MAXSIZE], int n),
     print(int m[][MAXSIZE], int n);

main()
{
   int n;
   int m1[MAXSIZE][MAXSIZE];
   int m2[MAXSIZE][MAXSIZE];
   int m3[MAXSIZE][MAXSIZE];

   printf("Input matrix size:");
   scanf("%d", &n);
   printf("Input first matrix by row\n");
   store(m1, n);
   printf("\nMaitrx m1:\n");
   print(m1, n);
   printf("Input second matrix by row\n");
   store(m2, n);
   printf("\nMaitrx m2:\n");
   print(m2, n);

   mult(m1, m2, m3, n);

   printf("\nProduct m3:\n");
   print(m3, n);
}

/* Multiply matrix m1 by m2, store the product in matrix m3 */
void mult(int m1[][MAXSIZE],
          int m2[][MAXSIZE],
          int m3[][MAXSIZE], 
          int n)
{
   int i, j, k;
   
   for(i = 0; i < n; ++i)
      for(j = 0; j < n; ++j) {
         m3[i][j] = 0;
         for(k = 0; k < n; ++k) 
            m3[i][j] += m1[i][k] * m2[k][j];
      }
}

/* Store/read data in matrix by row */
void store(int m[][MAXSIZE], int n)
{
   int i, j;
   for(i = 0; i < n; ++i) 
      for(j = 0; j < n; ++j) 
         scanf("%d", &m[i][j]);
}

/* Print the matrix */
void print(int m[][MAXSIZE], int n)
{
   int i, j;
   for(i = 0; i < n; ++i) {
      for(j = 0; j < n; ++j) 
         printf("%d ", m[i][j]);
      putchar('\n');
   }
}

