(define mult
  (lambda (c1 c2)
    (list (- (* (car c1) (car c2)) (* (cadr c1) (cadr c2))) 
	  (+ (* (car c1) (cadr c2)) (* (cadr c1) (car c2)))
     )
   )
)


(define power
  (lambda (c n)
    (if (= n 0) '()
	(power ((mult c c) (- n 1)))
    )
  )
)