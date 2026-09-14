;;
;; Chen Xuefeng's Tower of Hanoi code
;;
(defun hanoi(n source destn tmp)  ;; Recursive procedure solving Hanoi Tower
  ;; arguments: no. of disks, disks moved from 'source' to 'destn' using 'tmp'
  ;;            as a temporary pole
                                  ; notice that a special method is used for
                                  ; showing intermediate disks movements
  (cond
	((= n 1)
	  (print (list 'Disc n source '-> destn)))  ; show movement
    ((> n 1)
	  (hanoi (- n 1) source tmp destn)  ; move n-1 disks to tmp pole
      (print (list 'Disc n source '-> destn))   ; show movement
      (hanoi (- n 1) tmp destn source)  ; move those n-1 disks back
      t   ; returned but not used
    )
  )
)

(print "Enter a number for Hanoi Tower:")
(hanoi (read) 'a 'b 'c)
