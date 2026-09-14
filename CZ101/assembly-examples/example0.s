 # This program does nothing except to show the format of a MIPS assembly
 # program.  The comment character # should NOT start from the first column.

	.text	                 # The .text assembler directive indicates
	.globl	main             # the following is code (as oppose to data).
main:                            # .globl makes main known to the 
                                 # outside of the program.
                                 # Here should be code but nothing now.
	j	$31              # Jump back to operating system.
