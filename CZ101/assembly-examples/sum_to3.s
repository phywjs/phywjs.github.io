 # verify that   1 + 2 + 3 + .. + n = (1 + n) * n / 2 is true for n = 3.
 # Later on, we'll write a program for arbitrary n.
 #

main:                               # beginning of the program

 # initialize $8, $9, and $10 as 1, 2, 3.               
   li $8, 1                         # $8 now contains 1
   li $9, 2                         # $9 now contains 2
   li $10, 3                        # $10 now contains 3

 # compute 1+2+3, result in $11
   add $11, $8, $9                  # $11 = $8+$9 = 3
   add $11, $11, $10                # $11 = $11+$10 = 3+3 = 6
 
 # compute (1+3)*3/2, result in $12
   add $12, $8, $10                 # $12 = $8+$10 = 1+3 = 4
   mul $12, $12, $10                # $12 = $12*$10 = 4*3 = 12
   div $12, $12, $9                 # $12 = $12/$9 = 12/2 = 6

 # subtract, result in $13
   sub $13, $11, $12                # $13 = $11-$12 = 6-6 = 0
                                    # verified
j  $31                              # end the program
