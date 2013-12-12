(define replace
  (lambda (a b lst)
    (cond ((null? lst) '())
	  ((eq? a (car lst)) (cons b (replace a b (cdr lst))))
	  (else (cons (car lst) (replace a b (cdr lst))))
    )
  )
)

(define deep-replace
  (lambda (a b c)
    (cond ((null? c) '())
	  ((list? (car c)) (deep-replace a b (car c)))
	  ((eq? a (car c)) (cons b (deep-replace a b (cdr c))))
	  (else (append (car c) (deep-replace a b (cdr c))))
    )
  )
)