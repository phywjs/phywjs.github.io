 # This MIPS assembly program calculates the sum of an array (of
 # integers) of first n elements,  
 #   B[0] = A[0] + A[1] + A[2] + ... + A[n-1].
 # The result is sent back to the 0th element of array B[].
 #
 # Arguments passing:
 #   $4 contains the starting address of the array A[],
 #   $5 contains the number of terms (value) n to be summed,
 #   $6 contains the starting address of the result array B[].
 #
 # Note that $4, and $6 contain addresses, while $5 contains the 
 # value n, e.g, $5 = 10 if n = 10.
 # According to MIPS convention, the calling program (which calls
 # this program) is expected to put the addresses A and B and 
 # value n into the proper registers discribed above.
 #
 # In C the summation can simply be written as
 #
 #  sum = 0;
 #  for(i=0; i < n; ++i) {
 #    sum = sum + A[i];
 #  }
 #  B[0] = sum;
 # 
 # Here is the assembly code:

       .text
       .globl arraysum

arraysum:                    # starting address of this procedure

       addi $29, $29, -20    # 20 bytes for 5 registers
       sw   $2, 0($29)       # $2, $3, $4, $8, and $9 are not
       sw   $3, 4($29)       # really necessary to save and restore
       sw   $4, 8($29)       # by MIPS convention.
       sw   $8,12($29)       # But we following callee save 
       sw   $9,16($29)       # convention of the textbook.

       add  $2, $0, $0       # $2 contains the partial sum, sum = 0
       add  $3, $0, $0       # array index i = 0
Loop:  slt  $9, $3, $5       # testing, $9 = 1 if $3 < n
       beq  $9, $0, Exit     # if $9==0, (that is, if $3>=n) goto Exit
       lw $8, 0($4)          # $8 = A[i]
       add  $2, $2, $8       # sum <- sum + A[i]
       addi $4, $4, 4        # address of next element, 4 bytes away
       addi $3, $3, 1        # i <- i + 1, increment array index
       j    Loop             # goto Loop
Exit:  sw   $2, 0($6)        # store result in B[0]

       lw   $2, 0($29)       # restore $2, $3, $4, $8, and $9
       lw   $3, 4($29)
       lw   $4, 8($29)
       lw   $8,12($29)
       lw   $9,16($29)
       addi $29, $29, 20     # restore stack

       j    $31              # jump back to calling program
 
