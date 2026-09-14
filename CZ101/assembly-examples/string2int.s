 # Lab 5.1, string to int conversion, with main program in spim
 # $4: 1st argu containing address of the string. $2: return value.
 #
       .text
       .globl string2int
string2int:

       addi $2, $0, 0       # intialize return value to 0
       addi $10, $0, 10     # constant 10

Loop:  lb $3, 0($4)         # load a byte
       beq $3, $0, Exit     # if null byte, exit
       addi $3, $3, -48     # convert ASCII code to number
       slt  $5, $3, $0      # $3 should be [0-9] 
       bne  $5, $0, Error   # lower bound
       slti $5, $3, 10
       beq  $5, $0, Error   # upper bound
       mul  $2, $2, $10     # old value times 10
       add  $2, $2, $3      # add current value
       addi $4, $4, 1       # advance pointer
       j  Loop

Error:
       addi $2, $0, -1
Exit:  
       j $31

      .data
str0: .asciiz "\n"
str1: .asciiz "10113"
str2: .asciiz "1234567890"
str3: .asciiz "12cw6"
      .text
      .globl main
main:
      subu $29, 24
      sw   $31, 20($29)
      la   $4, str1    # convert first string
      jal string2int
      move $4, $2
      li   $2, 1
      syscall
      la   $4, str0    # print a new line
      li   $2, 4
      syscall
      la   $4, str2    # second string
      jal string2int
      move $4, $2
      li   $2, 1
      syscall
      la   $4, str0    # print a new line
      li   $2, 4
      syscall
      la   $4, str3    # third string
      jal string2int
      move $4, $2
      li   $2, 1
      syscall
      la   $4, str0    # print a new line
      li   $2, 4
      syscall
      lw   $31, 20($29)
      addu $29, 24
      j    $31

