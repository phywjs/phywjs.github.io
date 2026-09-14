 # This assembly program read and print an int, with scanf and printf.
 #
  	.data	                # data declaration
A:
	.word 0                 # to store readin data
formscanf:                       
	.ascii "%d\0"           #  format string for scanf
formprintf:                       
	.ascii "You have entered %d.\n\0"  #  format string for printf

	.text	                # code start below
	.globl	main
main:
                                # prologue
	sub 	$sp, 24         # reserve 24 bytes for the stack
	sw	$31, 20($sp)    # save the return address on stack

                                # read in an integer
                                # arguments to scanf("%d", &A)
	la	$4, formscanf   # load first argument (address) to a0
        la      $5, A           # load second argument, address of A, into a1
	jal	scanf           # jump and link to scanf function
                                # print the value just read in
                                # printf("...%d\n", A)
        la      $4, formprintf  # format for first argument
        lw      $5, A           # A now as the read-in value
        jal     printf          # call the printf

                                # epilogue
	lw	$31, 20($sp)    # restore return address in ra
	add 	$sp, 24         # pop stack 
	j	$31             # return to calling program
