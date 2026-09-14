 # Program for Lab 3, question 2
 # Signed integer multiplication Booth's algorithm
 # Argument passing,
 #   $4: multiplicand,  also known in the notes as M
 #   $5: multiplier,
 #   $6: address for result,
 # Register usage
 #   $2: PL, lower half of the product P,
 #   $3: PH, upper half of the product P,
 #   $7: previous bit C,
 #   $8: loop counter.
 #   $9: temporary use.

        .text
        .globl booth

booth:
        move $2, $5        # lower half of P contains multiplier 
        move $3, $0        # upper half of P is zero
        move $8, $0        # $8 counter contains 0
        move $7, $0        # C is 0
        

Loop:  addi $8, $8, 1      # one more operation 

 # (1) Add, subtract or do nothing, depending on (P0, C)
 #
        andi $9, $2, 1     # $9 = P0, 0-th bit in P
        xor  $9, $9, $7    # if (P0, C) = (00) or (11), then $9 = 0
        beq  $9, $0, Shift # do nothing if (00) or (11)

        beq  $7, $0, L10   #  if C = 0, jump to code for case (10) 
        addu $3, $3, $4    # (1b) case of (01), add M to PH      
        j Shift            # jump over the code for case (10) 
L10:                       # case for (10)
        subu $3, $3, $4    # (1c) case of (10), subtract M from PH

 # (2) Arithmetic shift register P ($2,$3) right 1 bit as a whole 64-bit
 #
Shift:  sll  $9, $3, 31    # 0-th bit in upper half, shift up
        andi $7, $2, 1     # 0-th bit in lower half
        sra  $3, $3, 1     # Upper reg shift down 1 bit
        srl  $2, $2, 1     # Lower reg shift down 1 bit
        or   $2, $2, $9    # the 31st bit in lower half
        
 # (3) count == 32? 
        slti $9, $8, 32    #  $9 = 1 if count < 32
        bne  $9, $0, Loop  #  loop until count == 32

        sw $2, 0($6)       # save results in proper addresses
        sw $3, 4($6)

        j $31

