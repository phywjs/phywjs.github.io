 # The assembly code for the C swap function, values in the array at
 # k and k+1 are interchanged.
 #
 # swap(int v[], int k)
 # {  int temp;
 #    temp = v[k];
 #    v[k] = v[k+1];
 #    v[k+1] = temp;
 # }
 #
 # Using simplified linkage convention of the textbook.  
 # Note that this is different from the MIPS convention.
 #
        .text
        .globl swap
swap:
                                  # Save registers
        addi    $29, $29, -12     # make room on stack for 3 reg
        sw      $2,  0($29)       # save $2 on stack
        sw      $15, 4($29)       # save $15 on stack
        sw      $16, 8($29)       # save $16 on stack
                                  # Precedure body
        mul     $2, $5, 4         # reg $2 = k * 4
        add     $2, $4, $2        # reg $2 = v + (k*4)
                                  # reg $2 has the address of v[k]
        lw      $15, 0($2)        # reg $15 (temp) = v[k]
        lw      $16, 4($2)        # reg $16 = v[k+1]
        sw      $16, 0($2)        # v[k] = reg $16
        sw      $15, 4($2)        # v[k+1] = reg $15 (temp)
                                  # Restoring registers
        lw      $2, 0($29)        # restore $2 from stack
        lw      $15, 4($29)       # restore $15 from stack
        lw      $16, 8($29)       # restore $16 from stack
        addi    $29, $29, 12      # restore stack pointer
                                  # Procedure return
        j       $31               # return to calling routine

