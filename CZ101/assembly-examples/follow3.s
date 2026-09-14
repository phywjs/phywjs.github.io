# The MIPS assembly program used for the quiz
# of 21 August 1997, 1:30pm-2:00pm.
# Follow the program, comment each execution 
# of the instructions.
#
main:
   li $4, 4
   li $2, 0x10000000
   sw $4, 0($2)
   add $2, $2, $4
   mul $4, $4, $4
   sw $4, 0($2)
   addi $3, $0, 2
L: beq $3, $0, Exit
   lw $5, 0($2)
   sub $4, $4, $5
   addi $3, $3, -1
   addi $2, $2, -4
   j L
Exit: slt $3, $5, $4
   sw $3, 12($2)
   bne $3, $0, Exit
   j $31
