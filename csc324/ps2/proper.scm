
(define deep-properlist?
  (lambda (a)
    (if (or (null? a) (not (pair? a))) #t (and (list? a) (if (pair? (car a)) (deep-properlist? (car a)) (deep-properlist? (cdr a)))))))