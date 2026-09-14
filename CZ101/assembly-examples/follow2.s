 # If you are reading this file by netscape (or other internet browser),
 # you can save it into your own directory.  On netscape, put down the
 # "File" menu, Choose the "Save As" by mouse, then click "OK" button.
 #
 # You can run this program by first evoking the xspim (just type xspim
 # at Unix prompt).  Once in xspim, press "load" button, type in the file
 # name containing MIPS code.  You should be able to see part of this 
 # file in xspim code window.
 # Press "run" to run the program.  Better yet, to see what's going on,
 # step the program one instruction at a time,  by press "step".
 # this will pop up a small step window.  Inside it, press "step" again to
 # actually step the program.  watch carefully the value in registers.
 #

main:
   li  $2, 0x10000000    #  (1)
   li  $4, 1             #  (2)
   sw  $4, 0($2)         #  (3)
   li  $4, 3             #  (4)
   sw  $4, 4($2)         #  (5)
   li  $4, 5             #  (6)
   sw  $4, 8($2)         #  (7)
   lw  $3, 0($2)         #  (8)
   lw  $4, 4($2)         #  (9)
   add $3, $3, $4        # (10)
   lw  $4, 8($2)         # (11)
   add $3, $3, $4        # (12)
   sw  $3, 12($2)        # (13)
   lw  $3, 0($2)         # (14)
   lw  $4, 4($2)         # (15)
   mul $3, $3, $4        # (16)
   lw  $4, 8($2)         # (17)
   mul $3, $3, $4        # (18)
   sw  $3, 16($2)        # (19)
   j $31
