 # This assembly program print a string twice on screen.
 # The printing is done by calling the C function printf with
 # one argument, equivalent to C code printf("Hello!\n"). 
 #
  	.data	                # data declaration
mesg:                           # label mesg indicates the address
	.ascii "Hello!\n\0"     # of this string, \0 is the null

	.text	                # code starts below
	.globl	main
main:
                                # prologue, needed if this function
                                # call other functions
	sub	$sp, 24         # reserve 24 bytes on the stack
	sw	$31, 20($sp)    # save the return address on stack

                                # body
	la	$4, mesg        # load address of the string into a0
	jal	printf          # jump and link to printf function
  	la	$4, mesg        # call it second time
	jal	printf

                                # epilogue
	lw	$31, 20($sp)    # restore return address in ra
	add 	$sp, 24         # pop stack 
	j	$31             # return to calling program
