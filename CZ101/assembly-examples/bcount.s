 # The bcount assembly program (3.35) (Lab 2, Question 2)
 #
 # bcount counts the number of b characters in a string.  It 
 # takes one argument from $4 which contains the starting address
 # of the string, and search for b characters and count them. 
 # The count is returned in $2.  bcount uses bfind.
 #
 # Registers $2, $3, $4, $16, $29 and $31 are used.  If I
 # don't know the code inside bfind (assuming I don't) and 
 # assuming that bfind follows MIPS register usage convention,
 # then I should not use $2 to $15 for the count, since bfind
 # may change the value. I used $16, which has to be preserved
 # across procedure calls.  Old values of $16 as well as $31
 # will be saved and restored.

        .text
        .globl bcount
bcount:
   
        addi $29, $29, -8   # ask 2 words for $31 and count
        sw   $31, 0($29)    # save old values on stack
        sw   $16, 4($29)

        add  $16, $0, $0    # initialize count to 0
Loop:   jal bfind           # call bfind, $4 is used and changed
        lb   $3, 0($2)      # bfind returns address for character in $2
        beq  $3, $0, Exit   # if the character is null, end is reached, exit
        addi $16,$16, 1     # otherwise, there must be a 'b', count it
        addi $4, $2, 1      # address after 'b', new arg for bfind
        j Loop              # looking for next 'b' 
Exit:
        add  $2, $16, $0    # copy count into $2 for return value

        lw   $31, 0($29)    # restore old values from stack
        lw   $16, 4($29)
        addi $29, $29, 8    # restore stack pointer

        j $31              
