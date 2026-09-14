 # The bfind assembly program for problem 3.34 (Lab 1, Question 2)
 #
 # bfind takes one argument $4 as the starting address of a string.
 # bfind tries to find the first 'b'.  It returns the address of first 'b'
 # if there is one.  Otherwise, the address of the null byte is
 # returned (in $2, of course).  C prototype:
 # char * bfind(char s[]);
 #
 # Registers $2, $3, and $4 are used.  None of them are saved
 # (MIPS convention).

      .text
      .globl bfind

bfind:
                         # nothing needs to be saved or restore
      addi $3, $0, 98    # $3 contains ASCII value for 'b'
Loop: lb   $2, 0($4)     # Load byte from source to $2
      beq  $2, $0, Exit  # Exit if it is a null byte
      beq  $2, $3, Exit  # Exit if it is a 'b'
      addi $4, $4, 1     # address of the following character
      j    Loop          # loop back for next character
Exit: add  $2, $4, $0    # return address of 'b' or null in $2

      j $31              # back to calling program

