 # The main program for Lab 4.1, compute gcd(A,B), result is put back in
 # C.  See Appendix A, page A45,A46 on how to use syscall instruction.
 # The syscall works only on xspim or spim, but not with cc.
 # With SPIM, the main program and the function must be in the same file.
        .data
str1:   .asciiz "gcd("
str2:   .asciiz ", "
str3:   .asciiz ") = "
str4:   .asciiz "\n"
A:      .word 12     # change the value for A, B for other input
B:      .word 4
C:      .word 0
        .text
        .globl main
main:
        addi $29, $29, -4
        sw $31, 0($29)
        lw $4, A        # load in values to registers
        lw $5, B
        jal gcd         # call the function
        sw $2, C        # printing out in a nice form is tedious with SPIM 
        la $4, str1
        li $2, 4
        syscall 
        lw $4, A
        li $2, 1
        syscall
        la $4, str2
        li $2, 4
        syscall 
        lw $4, B
        li $2, 1
        syscall
        la $4, str3
        li $2, 4
        syscall
        lw $4, C
        li $2, 1
        syscall 
        la $4, str4
        li $2, 4
        syscall
        lw $31, 0($29)
        addi $29, $29, 4
        j $31

 # The MIPS assembly language procedure for the greatest common
 # divisor.    $4 and $5 are the two arguments.
 # $4 contains the dividend,
 # $5 contains the divisor,

        .text
        .globl gcd
gcd:
          
 # Your code goes here
 #


        j $31
