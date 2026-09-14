;; Common LISP program for symbolic differentiation, by Wang Jian-Sheng
;;
;; Refs.:  (1) S. Hekmatpour, Introduction to LISP and Symbol Manipulation,
;;         Prentice Hall, 1988, Chapter 13.
;;         (2) R. D. Cameron, Symbolic Computing with LISP,
;;         Prentice Hall, 1992, sec. 2.6.  
;;
;; The main function is (derivative expr 'x).
;;
;; Ask if an expression is a constant, it is a constant
;; if the expression does not contain explicitly x
(defun is-const (f x)
   (cond 
         ( (atom f) (if (eq f x) nil t) ) ;; anything except x is const
         ( (null f) t )   ;; check 3 cases for list, () is surely a const
         ( (atom (car f) )     ;; if first element is an atom
            (and ( not (eq (car f) x) )     ;; it is a const if head not
                 (is-const (cdr f) x) ) )   ;; equal x and tail is const
         ( t ( and (is-const (car f) x)     ;; else it is constant if 
                   (is-const (cdr f) x) ) ) ;; head and tail are constants
   )
)
;;
;; function definitions to exact certain part of an expression
(defun opr (f) (first f))     ;; extract the operator of f
(defun opd1 (f) (second f))   ;; extract the first operand of f
(defun opd2 (f) (third f))    ;; extract the second operand of f

;; find the derivative of an expression
;; the use-level call sequence is (derivative expr 'x)
(defun derivative (f x)
   (cond 
;; -- Check for validity of the input data.  Valid expression for derivative
;; to work correctly must be unary or binary expressions.
      ( (and (listp f) (> (length f) 3) )  
         (format t "Not a valid expression!") )

;; -- Rule (1) --  the derivative of a constant is zero.
      ( (is-const f x) 0 )        ;; d const/dx = 0

;; -- Rule (2) -- the derivative of the variable itself is one.
      ( (atom f)  ( if (eq f x) 1 0 ) )  ;; d x/dx = 1, 0 otherwise

;; -- Rule (3) -- the derivative of sum or different is the sum or 
;;    different of the derivatives.
      ( (or (eq (opr f) '+) (eq (opr f) '-))     ;; f' = g'+h', or g'-h'
         (list (opr f) (derivative (opd1 f) x) (derivative (opd2 f) x))
      )

;; -- Rule (4) -- derivative of product
      ( (eq (opr f) '*)                         ;; (g h)' = g h' + g' h
         (list '+ (list '* (derivative (opd1 f) x) (opd2 f) )
                  (list '* (opd1 f) (derivative (opd2 f) x) ) ) )

;; -- Rule (5) -- derivative of quotient
      ( (eq (opr f) '/)                         ;; (g/h)' = g'/h - g h'/h^2
         (list '/ 
            (list '- (list '* (derivative (opd1 f) x) (opd2 f) )
                     (list '* (opd1 f) (derivative (opd2 f) x) ) ) 
            (list '^ (opd2 f) 2 ) ) )

;; -- Rule (6) -- derivative of constant power
      ( (and (eq (opr f) '^) (is-const (opd2 f) x) )  ;; (g^c)' = c g^(c-1) g'
         (list '* (opd2 f)                          ;; c = const
                  (list '* (derivative (opd1 f) x)
                           (list '^ (opd1 f) (list '- (opd2 f) 1 ) )
                  )
      ) )

;; -- Rule (7) -- derivative of exponential function
      ( (eq (opr f) 'exp)                    ;; (exp g)' = exp g * g'
         (list '* (derivative (opd1 f) x) f) )

;; -- Rule (8) -- derivative of logarithm
      ( (eq (opr f) 'log)                    ;; (log g)' = g'/g
         (list '/ (derivative (opd1 f) x) (opd1 f)) )

;; -- Rule (9) -- derivative of sine function
      ( (eq (opr f) 'sin)                    ;; (sin g)' = g' cos g
         (list '* (derivative (opd1 f) x) (list 'cos (opd1 f) ) ) )

;; -- Rule (10) -- derivative of cosine function
      ( (eq (opr f) 'cos)
         (list '- (list '* (derivative (opd1 f) x) (list 'sin (opd1 f) ) ) ) )
      ( t  `(derivative ,f ,x) ) ;; don't know how to calculate.
   )
)
;;
