 # Here is a simplest example of a (leaf) function.
 # This function can be called by another assembly program or C.
 # The sum function reads first two arguments from a0 and a1 and
 # returns the value in v0 (calling convention).
 # Since the function sum does not call other function,
 # we do not need to reserve space on stack. Thus there is
 # not need to change the value of $sp and to save the register $31.

	.text
	.globl sum
sum:
	add $2, $4, $5		# do a $4 + $5 -> $2

	j	$31
