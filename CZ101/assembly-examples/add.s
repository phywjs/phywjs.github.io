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

main:                  # start of the program

   li   $8, 1          # set $8 to 1
   li   $9, 2          # set $9 to 2
   add  $10, $8, $9    # $10 = $8 + $9

   j $31               # return to system
