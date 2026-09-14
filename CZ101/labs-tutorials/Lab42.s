 # Main procedure for SPIM simulator, for problem 4.2
 #
        .text
        .globl main
main:
        addi $29, $29, -4      # 1 word for return address on stack
        sw   $31, 0($29)

        li.s $f12, 1.0         # load mediate value x into the arugment
        jal expt               # call the exponential function
        li $2, 2               # print out the result by a system call 
        mov.s $f12, $f0        # for float $2=2 and $f12 contain value
        syscall                # The system calls works only on SPIM

        lw  $31, 0($29)
        addi $29, $29, 4       # restore stack

        j $31

        .text
        .globl expt
expt:

 # Your code goes here.
 #
 
       j $31

