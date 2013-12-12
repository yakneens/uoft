(define swap
  (lambda (a)
    (if (and (list? a) (>= (length a) 3)) (cons* (caddr a) (cadr a) (car a) (cdddr a)) a))) 


(define deep-swap
  (lambda (a not-swapped)
     (if not-swapped (let ((not-swapped #f)(a (swap a))) (if (pair? (car a)) (cons (deep-swap (car a) #t) (deep-swap (cdr a) #f))))) a))

