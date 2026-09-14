;; The factorial function by recursion in LISP
(defun factorial (n) 
  (if (and (integerp n) (>= n 0))        ; if argument is integer >= 0,
      (if (= n 0)                        ; compute with 
          1                              ; 0! = 1,
          (* n (factorial (- n 1)) )     ; n! = n * (n-1)!
      )
      `(factorial ,n)                    ; else unevaluate
  )
)
