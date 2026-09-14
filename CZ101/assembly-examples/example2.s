 # This assembly program print an int (word, 32 bits) and double (64 bits)
 # The printings are done by calling the C function printf with
 # two arguments, equivalent to C code printf("%d\n", A) and
 # printf("%f\n", F).
 #
  	.data	                # data declaration
A:	.word 1001
F:	.double 3.1415926
formi:                       
	.ascii "A=%d\n\0"       #  format string for integer
formf: 
	.ascii "F=%f\n\0"       # format string for floating point number

	.text
	.globl	main
main:
                                # prologue
	sub	$sp, 24         # reserve stack and
	sw	$31, 20($sp)    # save return address

                                # body, arguments to printf
                                # print an integer
	la	$4, formi       # load first argument (address) to a0
        lw      $5, A           # load second argument to a1
	jal	printf          # jump and link to printf function
                                # print a double
        la      $4, formf       # format for first argument
        ld      $6, F           # F occupies a2 and a3 (F is 8 bytes)
        jal     printf          # call the printf

                                # epilogue
	lw	$31, 20($sp)    # restore return address in ra
	add 	$sp, 24         # pop stack 
	j	$31             # return to calling program
