(define f
	(lambda (item ls)
		(cond
			((null? ls) '())
			((equal? (car ls) item) (cdr ls))
			(else (cons (car ls) (f item (cdr ls)))))))
			
(define (g L)
	(cond
		((number? L) '())
		((number? (car L)) (cdr L))
		(else (cons (g (car L)) (cdr L)))))
		
(define r
	(lambda (n)
		(letrec 
			((helper
				(lambda (k t)
					(cond
						((zero? k) t)
						(else (helper (- k 1) (* t k)))))))
		(helper n 1))))

(define (root tree)
	(car tree))
(define (left tree)
	(cadr tree))
(define (right tree)
	(caddr tree))
		
(define swap-branches
	(lambda (tree)
		(if 
			(null? tree) 
			'()
			(list (root tree) (swap-branches (right tree)) (swap-branches (left
			tree))))))
							
		
(define sum
	(lambda (list partial)
		(cond 
			((null? list) partial)
			((number? list) (+ list partial))
			((list? (car list)) (sum (cdr list) (sum (car list) partial)))
			(else (sum (cdr list) (+ (car list) partial))))))

(define sum-all
	(lambda (list)
		(sum list 0)))		
		
		
(define odd-members
	(lambda (list)
		(cond 
			((null? list) '())
			((< (length list) 2) list)
			(else	(cons (car list) (odd-members (cddr list)))))))

(define (what x y z)
	(cond 
		((null? x) 0)
		((equal? (car x) z) 0)
		((equal? (car x) y)
			(+ 1 (what (cdr x) y z)))
		(else (what (cdr x) y z))))
		
(define diff-list
	(lambda (l1 l2)
		(cond 
			((null? l1) '())
			((equal? (car l1) (car l2)) (diff (cdr l1) (cdr l2)))
			(else (cons* (car l1) (car l2) (diff-list (cdr l1) (cdr l2)))))))
			
(define sum-over-ten
	(lambda (l1 l2)
		(apply + (map (lambda (e1 e2) (if (> (+ e1 e2) 10) 1 0)) l1 l2))))
			
			
(define sum-over-ten2
	(lambda (l1 l2)
		(eval (cons '+ (map (lambda (e1 e2) (if (> (+ e1 e2) 10) 1 0)) l1 l2)) '()))
)				
			
			
(define sum
	(lambda (c1 c2)
		(cons* (+ (car c1) (car c2)) (+ (cadr c1) (cadr c2)) '())))
		
		
		
(define replace
	(lambda (a b lst)
		(cond 
			((null? lst) '())
		 	((eq? (car lst) a) (cons b (replace a b (cdr lst))))
			(else (cons (car lst) (replace a b (cdr lst)))))))		
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			 
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		 
