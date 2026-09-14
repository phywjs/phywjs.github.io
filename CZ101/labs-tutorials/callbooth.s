      .data
prod: .word 0, 0
str1: .asciiz "\nEnter multiplicand "
str2: .asciiz "Enter multiplier "
str3: .asciiz "Product low word = "
str4: .asciiz "\nProduct high word = "
      .text

main:
   addiu $29, $29, -16
   sw $31, 12($29)

   li $2, 4
   la $4, str1
   syscall
   li $2, 5
   syscall 
   sw $2, 0($29)   # $4 is the multiplicand 
   li $2, 4
   la $4, str2
   syscall
   li $2, 5
   syscall       # $5 is the multiplier
   move $5, $2
   lw $4, 0($29)
   la $6, prod   # $6 contain address
   jal booth     # call the function
   li $2, 4
   la $4, str3
   syscall
   la $6, prod
   li $2, 1
   lw $4, 0($6)
   syscall
   li $2, 4
   la $4, str4
   syscall
   la $6, prod
   li $2, 1
   lw $4, 4($6)
   syscall
   
   lw $31, 12($29)
   addiu $29, $29, 16
   j $31

booth:
# Your code begin here
  

# Your code end 
