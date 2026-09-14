 #   This program shows how one program (main) calls another program
 #   (sum).  An example is given in the program ex3b.c.
 #   C function prototype for sum is  int sum(int a, int b).

  	.data	                # data declaration
format:                         # control string for printf
	.ascii "A=%d, B=%d, A + B = %d.\n\0" 
A:	.word 1			# three integer variables 
B:	.word 2
C:	.word 0

	.text	                # code starts below
	.globl	main
main:
                                # prologue
	sub 	$sp, 24         # reserve 6 words for the stack
	sw	$31, 20($sp)    # save the return address on stack

                                # body
	lw	$4, A           # load value of A into a0
	lw	$5, B		# load value of B into a1
	jal	sum	        # jump and link to sum function
				
 # calling a function has the effect of destroying the content of $31,
 # That's why we what to save it on stack.
        sw	$2, C		# save the return value into C
	la	$4, format	# first arg (address of string) into a0
	lw	$5, A		# 2nd arg for printf into a1
	lw	$6, B		# 3nd arg into a2
	lw	$7, C		# 4nd arg into a3
	jal	printf		# call printf
                                # epilogue
	lw	$31, 20($sp)    # restore return address in ra
	add 	$sp, 24         # pop stack 
	j	$31             # return to calling program

 # Here is a simplest example of a (leaf) function.
 # The sum function reads first two arguments from a0 and a1 and
 # returns the value in v0 (calling convention).
 # Since the function sum does not call other function,
 # we do not need to reserve space on stack. Thus there is
 # not need to change the value of $sp and save the register $31.

	.globl sum
sum:
	add $2, $4, $5		# do a $4 + $5 -> $2

	j	$31
