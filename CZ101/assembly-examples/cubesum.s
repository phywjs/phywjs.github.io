 # This program shows the use of loop. It computes 
 #    1^3 + 2^3 + 3^3 + 4^3 + ... + N^3.
 # N is a value loaded in $6

main:
                   # How registers are used:
   li, $6, 10      # $6, the limit N
   li, $8, 0       # $8, the sum
   li, $4, 0       # $4=n, runs from 1 to N
   li, $3, 1       # $3, constant 1

L: add $4, $4, $3  # increment n by 1
   mul $5, $4, $4  # $5 = n^2
   mul $5, $5, $4  # $5 = n^3
   add $8, $8, $5  # add n^3 to sum
   bne $4, $6, L   # go back if n!=N

   j $31
