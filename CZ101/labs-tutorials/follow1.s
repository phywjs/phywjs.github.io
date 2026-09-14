 # Assembly program for tutorial 1, question 3.
 # You can run it on xspim.
 #
main:
   li  $3, 1
   li  $2, 0x10000000
   li  $4, 1
   sw  $4, 0($2)
   li  $4, 2
   sw  $4, 4($2)
   li  $4, 3
   sw  $4, 8($2)
   lw  $5, 0($2)
   add $5, $5, $3
   sw  $5, 12($2)
   lw  $5, 4($2)
   add $5, $5, $3
   sw  $5, 16($2)
   lw  $5, 8($2)
   add $5, $5, $3
   sw  $5, 20($2)
   j $31
