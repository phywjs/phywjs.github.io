;; Common Lisp program for symbolic algebra. The function simplify knows
;; how to simplify expressions involving +, some *, and limited 
;; exponential ^.  It can do polynomial multiplications.
;;
;; Jian-Sheng Wang, October 1995.                     

;; EXPAND takes an expression and expands out in the form
;; (+ b (* c d) (f g) (^ a b) )
;;
;; expanded-p test if an expression is already in expanded format

;; expanded ::= term | (term ...) | (+ term ...)
;; eg a, (f (+ x y)), (* a b), (+ a (* b c)) 
(defun expanded-p (expr)
   (cond 
      ( (term-p expr) t)
      ( (term-list-p expr) t)
      ( (plus-opr-p expr) (term-list-p (oprands expr) ) )
      (t nil)
   )
)

;; term ::= factor | (factor ...) | (* factor ...)
(defun term-p  (expr)
   (cond 
      ( (is-opr expr) nil)   
      ( (factor-p expr) t)
      ( (factor-list-p expr) t)
      ( (times-opr-p expr) (factor-list-p (cdr expr)) )
      (t nil)
   )
)
;; term-list ::= (term ...)
(defun term-list-p (expr)
   (cond
      ( (endp expr) t )
      ( (and (term-p (car expr)) (term-list-p (cdr expr)) ) t)
      ( t nil)
   )
)

;; factor ::= expanded-function |(^ atom expanded) | atom (exclude '+ '* '^)
(defun factor-p (expr)
   (cond
      ( (is-opr expr) nil)
      ( (atom expr) t)
      ( (expanded-function-p expr) t)
      ( (power-opr-p expr)
         (and (atom (second expr) ) (expanded-p (third expr))) )
      (t nil)
   )
)
;; factor-list ::= (factor ...)
(defun factor-list-p (expr)
   (cond
      ( (endp expr) t )
      ( (and (factor-p (car expr)) (factor-list-p (cdr expr)) ) t)
      ( t nil)
   )
)
 
;; expanded-function ::= (atom expanded)
(defun expanded-function-p (expr)
   (cond 
      ( (is-opr (car expr)) nil )
      ( (and (atom (car expr)) (expanded-p (second expr)) ) t)
   )
)

;; What type of operator it is?
(defun plus-opr-p (expr)
   (and (listp expr) (eq (car expr) '+) )
)
(defun times-opr-p (expr)
   (and (listp expr) (eq (car expr) '*) )
)
(defun power-opr-p (expr)
   (and (listp expr) (eq (car expr) '^) )
)
;
; an expression of the form (opr ...)?
(defun operator-expr-p (opr expr)   
   (and (listp expr) (eq (car expr) opr))
)
(defun opr (expr) 
   (car expr)
)
(defun oprands (expr)
   (cdr expr)
)

;;
(defun is-opr (x)
 (or (eq x '+) (eq x '*) (eq x '^) )
)

;; Check for input validity
;; expr ::= (+ expr ...) | (* expr ...) | (^ expr expr) | (f expr)
;;
;; The expand function
(defun expand (expr)
   (cond 
     ; - (0) -  terminating condition
      ( (expanded-p expr) expr
      )
     ; - (1) - case for (+ a (+ b ...) ...), associative law
      ( (plus-opr-p expr) 
         (expand-associative '+ expr)
      )
     ; - (2) - case for (* a (* b ...) or (* a (+ b ...) ...)
      ( (times-opr-p expr) 
         (expand-distribution (expand-associative '* expr) )
      )
     ; - (3) - case for (^ a n)
      ( (power-opr-p expr)
         (expand-power (second expr) (third expr))
      )
     ; - (4) - case of a general function (f x ...)
      (t (expand-function-arguments expr)
      )
   ) ; end cond
)

;; expand the cases (+ a (+ b c)) or (* a (* b c))
(defun expand-associative (opr expr)
   (let ((result (list opr)))
      (dolist (x (cdr expr) result)
         (setf x (expand x))
         (if (operator-expr-p opr x) (setf x (cdr x) ) (setf x (list x)))
         (setf result (append result x))
      )
   )
)

;; expand the case (* (+ a b) c) -> (+ (* a c) (b c))
;; The input is alread in the form (* expanded-term ...)
(defun expand-distribution (expr)
   (do* (
      (x (cdr expr) (cdr x) )  ; rest
      (y (car x) (car x) )  ; car of x, current
      (z () )  ; head part
      (return-ok nil)   ; return flag
      (result '(+)) )
      ( (or return-ok (endp x) ) (if return-ok (expand result) expr ) )
      ( when (plus-opr-p y)
            (setf z (append z (cdr x)) ) ; expr with (+ ...) term removed
               (dolist (v (cdr y) result)
                  (setf result (append result (list (cons '* (cons v z)) ) ) )
               )
            (setf return-ok t)
      )
      ( setf z (append z (list y)) )
   )
)

;;
;; multiply out power or expand base and exponent
(defun expand-power (a n)
   (let
      ((result '()))
      (if (and (listp a) (integerp n) (>= n 1) )
         (progn
            (dotimes (i n)
                (push a result)
            )
            (push '* result)
            (expand result)
         )
         ; else
         (append '(^) (list (expand a)) (list (expand n)))
      )  ; end if
   )  ; end let
)

;; expand each term of the function argument
(defun expand-function-arguments (expr)
   (cons (car expr) (mapcar #'expand (cdr expr)) )
)

;; Simplify the constant expression of the form (+ 1 2 3 ...) or (* 1 2 3 ...)
(defun simplify-const (expr)
   (let ((num 0) (l 0) (sym-list '()) )
   (if (atom expr) 
      expr  ; then
      (progn  ; else
         (setf expr (mapcar #'simplify-const expr))  ; simplify each term first
         (cond 
            ( (plus-opr-p expr)
               (setf num (apply #'+ (remove-if-not #'numberp expr) ) )
               (setf sym-list (remove-if #'numberp expr) )
               (pop sym-list)  ; '+ sign removed
               (setf sym-list (sort sym-list #'expr-less-than-p))
               (push '+ sym-list)
               (setf l (length sym-list) )
               (if (= num 0) 
                  (cond 
                     ((= l 1) 0)
                     ((= l 2) (second sym-list) ) 
                     ((> l 2) sym-list)
                  )
                  (if (= l 1) num (append sym-list (list num) ) )
               )
            )
            ( (times-opr-p expr)
               (setf num (apply #'* (remove-if-not #'numberp expr) ) )
               (setf sym-list (remove-if #'numberp expr) )
               (pop sym-list)  ; '+ sign removed
               (setf sym-list (sort sym-list #'expr-less-than-p))
               (push '* sym-list)
               (setf l (length sym-list) )
               (cond 
                  ( (= num 1)
                     (cond 
                        ((= l 1) 1)
                        ((= l 2) (second sym-list) ) 
                        ((> l 2) sym-list)
                     )
                  )
                  ( (= num 0) 0
                  )
                  ( t 
                     (if (= l 1) num (append sym-list (list num) ) )
                  )
               )
            )
            (t expr)
         ) ; end cond
      ) ; end progn
   ) ; end if
   ) ; let
)

;; compare to expressions in lexical order
(defun expr-less-than-p (x y)
   (string< (format nil "~A" x) (format nil "~A" y))
)

;; combine terms
(defun simplify-expr (expr)
(let ((res nil)) ; res is result, initially nil
   (cond
      ( (plus-opr-p expr)
         ; go over the terms and build a new expression
         (dolist (x (cdr expr) (cons '+ res))
             (setf res (combine-a-term x res) )
         )
      )
      ( t expr)
   )  ; end cond
) ; end let
)

;; Add x to the list expr, combined terms if possible
(defun combine-a-term (x expr)
(let (xc symb-xc coeff-xc yc coeff new-expr)
      (setf xc (canonical-term x) )
      (setf symb-xc (symbol-term xc) )
      (setf coeff-xc (coefficient xc) )
   (do                                     ; run over each term in expr
      ( (y (car expr) (car u) )            ; the current term
        (p '() (append p (list y)) )       ; Processed term
        (u (cdr expr) (cdr u))             ; Uprocessed term
        (c x)                              ; Combined term, if combined
        (done nil)                         ; t if a combination is find
      )
      ( (or done (null y))                 ; stop if done or not more term
         (if done new-expr (append expr (list x)) ) )  ; return expression
       (setf yc (canonical-term y) )
       (when
         (equal symb-xc (symbol-term yc) )    ; term of same form
             (setf coeff (+ coeff-xc (coefficient yc) ) )
             (cond
               ( (eq symb-xc '()) (setf c coeff) )  ; a term is a number
               ( (= coeff 0) (setf c 0) )
               ( (= coeff 1) (setf c
                 (if (eq (length symb-xc) 1) (car symb-xc) (cons '* symb-xc) )))
               (t (setf c (append '(*) symb-xc (list coeff)) ) )
            )
             (setf new-expr (append p (list c) u) )
            (setf done t)
      )
   )  ; end do
)  ; end let
)

; A canonical term is define as (* number) or (* factor ... number).
; If it is a number return (* number);
; if it is already in the form (* a b c 3) return it; if it is
; (* a b) return (* a b 1), anything else return (* x 1).
(defun canonical-term (x)
   (cond
      ( (numberp x) (list '* x) )
      ( (times-opr-p x) (if (numberp (car (last x))) x (append x '(1))) )
      (t (list '* x 1))
   )
)

; take a canonical term and return the numerical coefficient of the term
(defun coefficient (x)
   (car (last x) )
)

; take a canical term and return the symbols, with * and number removed.
(defun symbol-term (x)
   (cdr (butlast x))
)

(defun simplify (expr)
   (simplify-expr (simplify-const (expand expr) ) )
)
