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

 # Compute the exponetial function according to Taylor expansion
 #   exp(x) = 1 + x + x^2/2! + x^3/3! + ...
        .text
        .globl expt
expt:
                          # x is in $f12
        li.s $f16 0.0     # n = 0
        li.s $f14 1.0     # y = 1
        li.s $f0  0.0     # f = 0
        li.s $f2, 1.0     # constant 1.0
        li.s $f4, 0.00001 # constant
Loop:
        add.s $f0, $f0, $f14     # f = f + y
        add.s $f16, $f16, $f2    # n = n + 1
        mul.s $f14, $f14, $f12   # y = y * x
        div.s $f14, $f14, $f16   # y = y / n
        abs.s $f18, $f14         # |y|
        c.lt.s $f4, $f18         # 0.00001 < |y|?
        bc1t Loop

       j $31

