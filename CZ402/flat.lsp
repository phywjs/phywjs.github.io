;; A flat list is defined as any form  (a b c ... ) where a, b, c, ... 
;; are atom except NIL. Anything else is not flat.
(defun flatp (x)
  (cond 
    ( (not (listp x)) nil) ; Non-list (atom but not NIL) is not flat. 
    ( (endp x) t )         ; Empty list a flat list.
    ((listp (car x)) nil)  ; If the first is a list, it is not flat
    (t (flatp (cdr x)) )   ; else first one must be an atom (but not 
  )                        ; NIL), the flatness is determined by rest.
)

;; flatten a list, remove all parentheses except the out one. 
;; flatten a non-NIL atom gives a list of atom.
(defun flatten (x)
  (cond 
      ( (null x) x)              ; empty list  is already flattened
      ( (atom x) (list x) )      ; atom x return (x)
      ( t (append (flatten (car x)) (flatten (cdr x) ) ) )
  )                              ; otherwise, flatten the first and rest
)
