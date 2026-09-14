 # Sort function
 #
 # sort(int v[], int n)
 # { int i, j;
 #    for(i = 0; i < n; i = i+1) {
 #       for(j = i-1; j >= 0 && v[j] > v[j+1]; j = j-1) {
 #          swap(v, j);
 #       }
 #    }
 # }
 #
 # Using simplified linkage convention of the textbook.
 #
        .text
        .globl sort
sort:
 # SAVING REGISTERS
        addi    $29, $29, -36   # make room on stack for 9 regs
        sw      $15, 0($29)     # save $15 on stack
        sw      $16, 4($29)     # save $16 on stack
        sw      $17, 8($29)     # save $17 on stack
        sw      $18,12($29)     # save $18 on stack
        sw      $19,16($29)     # save $19 on stack
        sw      $20,20($29)     # save $20 on stack
        sw      $24,24($29)     # save $24 on stack
        sw      $25,28($29)     # save $25 on stack
        sw      $31,32($29)     # save $31 on stack
 # PROCEDURE BODY
                                # Move Parameters
        move    $18, $4         # copy v into $18
        move    $20, $5         # copy n into $20
                                # Outer Loop
        add     $19, $0, $0     # i = 0
for1:   slt     $8, $19, $20    # reg $8 = 0 if $19 >= $20 (i>=n)
        beq     $8, $0, exit1   # goto exit1 if $19 >= $20 (i>=n)
                                # Inner Loop
        addi    $17, $19, -1    # j =i - 1
for2:   slti    $8, $17, 0      # reg $8 = 1 if $17 < 0 (j<0)
        bne     $8, $0, exit2   # goto exit2 if $17 < 0 (j<0
        mul     $15, $17, 4     # reg $15 = j * 4
        add     $16, $18, $15   # reg $16 = v + (j*4)
        lw      $24, 0($16)     # reg $24 = v[j]
        lw      $25  4($16)     # reg $25 = v[j+1]
        slt     $8, $25, $24    # reg $8 = 0 if v[j+1] >= v[j]
        beq     $8, $0, exit2   # goto exit2 if v[j+1] >= v[j]
                                # Passing Parameters and Call
        move    $4, $18         # 1st parameter of swap is v
        move    $5, $17         # 2nd parameter of swap is j
        jal     swap            # call swap
                                # Inner  Loop
        addi    $17, $17, -1    # j = j - 1
        j       for2            # jump to test of inner loop
                                # Outer Loop
exit2:  addi    $19, $19, 1     # i = i + 1
        j       for1            # jump to test of outer loop
 # RESTORING REGISTERS
exit1:
        lw      $15, 0($29)     # restore $15 from stack
        lw      $16, 4($29)     # restore $16 from stack
        lw      $17, 8($29)     # restore $17 from stack
        lw      $18,12($29)     # restore $18 from stack
        lw      $19,16($29)     # restore $19 from stack
        lw      $20,20($29)     # restore $20 from stack
        lw      $24,24($29)     # restore $24 from stack
        lw      $25,28($29)     # restore $25 from stack
        lw      $31,32($29)     # restore $31 from stack
        addi    $29, $29, 36    # restore stack pointer
 # PROCEDURE RETURN
        j       $31             # return to calling routine
