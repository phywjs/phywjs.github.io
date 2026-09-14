 # The sum assembly procedure for Lab 2, Question 1
 #
 # The sum procedure takes one argument from $4 as the number n,
 # and return value sum 1 + 2 + 3 + ... + n, in $2.
 # It returns 0 if n <= 0.  I add backwards from n to 1.
 #
        .text
        .globl sum
sum:

        addi $2, $0, 0     # $2 for the sum, initialized to 0
                           # $4 is i (= n initially)
Loop:   slt  $3, $0, $4    # if (i <= 0)  Exit
        beq  $3, $0, Exit  # else do the sum
        add  $2, $2, $4    # sum = sum + i
        addi $4, $4, -1    # --i, decrease i
        j    Loop          # loop until i == 0
Exit:
        j    $31           # go back to calling program
