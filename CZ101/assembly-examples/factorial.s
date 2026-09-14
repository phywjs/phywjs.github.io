 # CC compiler's translation of the following C program.
 # #include <stdio.h>
 # 
 # main()
 # {
 #   printf("The factorial of 10 is %d\n", fact(10) );
 # }
 # 
 # int fact(int n)
 # {
 #    if ( n <= 1 )
 #      return 1;
 #    else
 #      return ( n* fact(n-1) );
 # }
 #
        .data        
Form:   .ascii        "The factorial of 10 is %d\n\0"

        .text        
        .globl  main
main:
        subu   $sp, 24             # reserve stack
        sw     $31, 20($sp)

        li     $4, 10
        jal    fact
        la     $4, Form
        move   $5, $2
        jal    printf

        lw     $31, 20($sp)       # restore stack
        addu   $sp, 24
        j      $31


 # The recursive function for factorial
        .text
        .globl fact
fact:
        subu   $sp, 24
        sw     $31, 20($sp)

        move   $3, $4
        bge    $3, 2, Else
        li     $2, 1
        j      Exit
Else:
        addu   $4, $3, -1
        sw     $3, 24($sp)
        jal    fact
        lw     $3, 24($sp)
        mul    $2, $3, $2
Exit:
        lw     $31, 20($sp)
        addu   $sp, 24
        j      $31

