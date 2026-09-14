 # The first multiplication algorithm for unsigned integer.
 # Simple-minded implementation, produce only 32-bit result.
 #
 # Calculating the product $4 * $5, and returnning result in $2.
 # $4 contains the multiplicand and $5 contains the multiplier,

main:
         li   $4, 37
         li   $5, 31
         add  $2, $0, $0     # initialize product to zero

Loop:    beq  $5, $0, Exit   # if the multiplier is 0, terminate looping
         andi $3, $5, 1      # mask out the 0th bit in multiplier
         beq  $3, $0, Shift  # if the bit is 0, skip add
         addu $2, $2, $4     # add (shifted) multiplicand to product
Shift:   sll  $4, $4, 1      # shift up the multiplicand 1 bit
         srl  $5, $5, 1      # shift down the multiplier 1 bit
         j    Loop           # go for next bit in multiplier
Exit:                        # finished.

         j $31               # back to calling routine

