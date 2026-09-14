 # Unsigned integer division algorithm (final version) of the textbook.
 # Implemented in software.
 # Argument passing,
 #   $4: dividend,
 #   $5: divisor,
 #   $6: address for quotient,
 #   $7: address for remainder.
 # $7 and $6 is used also as a 64-bit remainder register, $7 is the left
 # half and $6 the right half.

        .text

divide:
        move $8, $6        # $8 and $9 store the old value of
        move $9, $7        # of $6 and $7, which is the address of the results
        addi $3, $0, 0     # counter for the number of operations
        move $6, $4        # right half of the remainder reg contains dividend
        move $7, $0        # zero the left half

Loop:  addi $3, $3, 1      # one more operation 

 # (1) Shift the remainder registers ($7,$6) left 1 bit as a whole 64-bit
        slt $2, $6, $0     # left most bit in $6 is now in $2
        sll $6, $6, 1      # shift the 2 words remainder left 1 bit
        sll $7, $7, 1      # ajust the bit lost in $6
        or  $7, $7, $2     # left most bit in $6 gets into right most in $7 

 # (2) Subtract divisor from left half of remainder
        subu $7, $7, $5    # overflow ignored

 # (3) Test the remainder
        slt $2, $7, $0     # $2 = 1 if $7 is negative
        beq $2, $0, Positive   # implement if-else statment

 # (3b) Remainder < 0        quotient gets zero
        addu $7, $7, $5    # reverse the subtraction in step (2)
        j Exit             # jump over the code for remainder positive

Positive:
 # (3a) Remainder >= 0
        ori $6, $6, 1      # quotient gets a one
Exit:

 # (4) count == 32? 
        slti $2, $3, 32    #  $2 = 1 if count < 32
        bne  $2, $0, Loop  #  loop until count == 32

        sw $6, 0($8)       # save results in proper addresses
        sw $7, 0($9)

        j $31

