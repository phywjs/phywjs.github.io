/*  The tower of Hanoi is a puzzle consisting of three pegs (poles)
mounted on a board and n disks of various sizes with holes in their
centers.  If a disk is on a peg, only a diks of smaller diameter
can be placed on top of it.  Large disk can not be put on small disks.
Given all disks properly stacked on one peg, say peg A, the problem
is to transter the disks to another peg, say peg B, by moving one
disk at a time, using peg C as a spare.  This program prints the steps 
of moves.
*/
#include <stdio.h>

void hanoi(char A, char B, char C, int n);
main()
{
   char A='A', B='B', C='C';
   int n;

   printf("\n\tHow many disks initially on peg A? ");
   scanf("%d", &n);

   if(n < 1) 
      printf("\n\tThere's nothing to move!\n");
   else
      hanoi(A, B, C, n);
}

/*  hanoi function moves n disks on the peg A
    to peg B, using C as a spare */

#define MOVE(x,y) printf("Move disk from %c to %c.\n", (x), (y))

void hanoi(char A, char B, char C, int n)
{
   if (n == 1) {
      MOVE(A, B);
      return;
   }
   hanoi(A, C, B, n-1);
   MOVE(A, B);
   hanoi(C, B, A, n-1);
}  

