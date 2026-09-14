 # Assembly program bcopy, Lab 2, Question 2
 # bcopy copies a sequence of bytes from source address in $4
 # to destination address in $5 until a null byte is found.
 # It returns the count in $2.  The last null byte is copied 
 # but not counted.  Prototype declaration in C:
 # int bcopy(char s[], char t[]);
 #
 # Registers $2, $3, $4, and $5 are used. None of them are 
 # saved (MIPS convention).
 #
      .text
      .globl bcopy

bcopy:

      addi $2, $0, -1    # Initialize the count -1
loop: lb   $3, 0($4)     # Read next byte from source
      sb   $3, 0($5)     # Write to destination
      addi $2, $2, 1     # Increment count (number of bytes copied)
      addi $4, $4, 1     # Advance to next source
      addi $5, $5, 1     # Advance to next dest
      bne  $3, $0, loop  # Loop if byte copied is not zero

      j $31              # back to calling program
 
