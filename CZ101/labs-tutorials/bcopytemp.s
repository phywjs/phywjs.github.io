# Template for the bcopy.s (string copy) program
 
      .data
str:    .asciiz "a string--end\n"  # the source string
str2:   .asciiz "test, space, enough space?\n"    # destination string

      .text
main:

   la $4, str                      # load in source string address
   la $5, str2                     # load destination address

# Your code here


# Your code end
   move $4, $2
   li $2, 1
   syscall                         # print out count
   li $2, 4
   la $4, str                      # print source string
   syscall
   li $2, 4
   la $4, str2                     # print destination string
   syscall

   j $31 
