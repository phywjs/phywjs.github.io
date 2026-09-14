 # Evaluate the value of a polynomial 
 # f = a[0] + a[1] * x + a[2] * x^2 + ... + a[n] * x^n
 # Using Horner's algorithm.
 # $4 contains address for the array of float coefficient a[]
 # $5 contains int n, order of polynomial
 # $6 contains float x, the variable of polynomial

        .text
        .globl poly
poly:

        li.s  $f0, 0.0          # y = 0, running & return result
        mtc1  $6  $f12          # x, move to float register
Loop:
        mul.s $f14, $f12, $f0   # compute (x * y)
        mul   $2, $5, 4         # $5 = i, compute address of a[i]
        addu  $3, $2, $4        # a + (i*4)
        l.s   $f16, 0($3)       # a[i], load coefficient
        add.s $f0, $f16, $f14   # y = a[i] + (x*y)
        addi  $5, $5, -1        # decrease i

        slt   $2, $5, $0        # $2 = 1 if i < 0 
        beq   $2, $0, Loop      # goto Loop if i >= 0 

Exit:
        j     $31
