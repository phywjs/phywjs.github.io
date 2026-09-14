 # Lab 5.2.  Compute Legendre polynomials recursively.
 # Answers are P(0.0,2) = -0.5, P(0.5,3)=-0.4375, P(0.25,5)=0.339722
 # C prototype: double P(double x, int n);
 # argument passing: 
 # x -> $f12
 # n -> $6
 # Intermediate results are in saved regs $f20 and $f22.
 #
       .text
       .globl P
P:
       addiu $29, $29, -40    # 4 words for args,  4 words for saved regs
       sw   $31,  32($29)     # 1 word for $31
       s.d  $f20, 16($29)     # for (1-1/n)
       s.d  $f22, 24($29)     # for partial result

       bne  $6, $0, N1 
       li.d   $f0,  1.0       # n = 0
       j  Exit                # return 1.0

N1:    addi $2, $0, 1
       bne  $6, $2, Recur
       mov.d $f0, $f12        # n = 1
       j  Exit                # return x 

Recur:                        # n > 1, recursive calls
       mtc1 $6, $f10          # compute (1-1/n)
       cvt.d.w  $f10, $f10    # $f10 = float(n)
       li.d $f8, 1.0          # $f8 = 1.0
       div.d $f6, $f8, $f10   # $f6 = 1/n
       sub.d $f20, $f8, $f6   # $f20 = 1-1/n (save reg)

       s.d  $f12, 40($29)     # save arg1 = x (in reserved area by caller)
       sw   $6,   48($29)     # save arg2 = n
       add  $6, $6, -1        # n - 1
       jal P                  # $f0 = P(x,n-1)

       li.d  $f8, 1.0
       add.d $f6, $f20, $f8   # $f6 = (2-1/n)
       l.d   $f12, 40($29)    # x
       mul.d $f10, $f12, $f6  # x * (2-1/n)
       mul.d $f22, $f10, $f0  # $f22 = x * (2-/1n) P(x, n-1) (save reg)

       lw   $6,   48($29)     # saved arg2 = n
       addi $6, $6, -2        # n - 2
       jal P                  # P(x, n-2)
       
       mul.d $f6, $f20, $f0   # (1-1/n) P(x, n-2)
       sub.d $f0, $f22, $f6  
Exit:

       lw   $31,  32($29)     # restore registers
       l.d  $f20, 16($29)
       l.d  $f22, 24($29)
       addiu $29, $29, 40     # restore stack

       j $31
