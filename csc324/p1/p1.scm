;Title: Scheme Project 1
;By: Sergei Iakhnin
;#: 981 843 420
;For: Alex Kress
;Course: CSC 324


; This is the main procedure of the expression evaluator
;Pre: bindings is a list of bindings
(define run
  (lambda (bindings)
    ; Save the bindings for testing purposes: after a crash, run can
    ; be called with (run last-bindings) to start where it left off.
    ; WARNING:  last-bindings should not appear anywhere else in your
    ;           code!!!!
    (set! last-bindings bindings)
    (display "\n>")         ; display a user prompt
    (let ((expr (read)))    ; Read a statement from the user
      (cond
        ((eq? expr 'quit)
         (display "Done!"))
        ((symbol? expr) (cond ((validate-bindings expr bindings) (display-all (do-eval (resolve-bindings expr bindings))) (run bindings))
                             (else (run bindings))
                      ))
        ((list? expr)
         (if (member ':= expr)
             (cond ((or (< (length expr) 3) (not (eq? (cadr expr) ':=)))
                 (display-error 'syntax) (run bindings))
                 (else (if (list? (car expr))
                           (run (bind-func-def (caar expr) (cdar expr) (caddr expr) bindings))
                           (run (bind-var (car expr) (caddr expr) bindings))
                       )
                 )
             )
             (cond ((validate-bindings expr bindings) (display-all (do-eval (resolve-bindings expr bindings))) (run bindings))
                   (else (run bindings))
             )
          ))
          (else
            (display "Unknown statement type")
            (run bindings))
            )
        )
    )
)


;;;; Define all helper procedures required by run here ;;;;

;Evaluates expr if it is a list
;Returns expr if it is not a list
;Pre: None
(define do-eval
  (lambda (expr)
    (if (pair? expr) (if (> (length expr) 1) (eval (infix-to-prefix expr) '()) (eval (if (procedure? (car expr)) expr (car expr)) '())) expr)
  )
)

;Binds name to value inside the bindings list
;All bindings in value are resolved and value is evaluated prior to binding
;Prints name
;Returns new bindings list
;Pre: bindings is a list of bindings
(define bind-var
  (lambda (name value bindings)
    (if (validate-bindings value bindings) (begin (display-all name) (set-binding name (do-eval (resolve-bindings value bindings)) bindings)) bindings)
  )
)

;Binds name to a procedure of (length args) number of arguments
;with procedure body described by body
;All bindings in body are resolved
;A lambda expression is constructed out of the body and then evaluated
;Prints name of procedure and its arguments
;If (validate-bindings body superbinds) does not hold the bindings are not altered
;Pre: args is a list, bindings is a list of bindings, body is a valid scheme expression
(define bind-func-def
  (lambda (name args body bindings)
    (let* ((superbinds (set-args args bindings)) 
           (valid (validate-bindings body superbinds)))
      (if valid 
          (begin (display-all (cons name args))
                 (set-binding name 
                              (eval (cons* 'lambda 
                                           args 
                                           (infix-to-prefix (resolve-bindings body superbinds)) 
                                           '()) 
                              '()) 
                              bindings))
          bindings)
    )
  )
)

;For every element in args adds a binding to bindings that binds that element to itself
;Returns a new bindings list
;Pre: args is a list, bindings is a list
(define set-args
  (lambda (args bindings)
    (if (null? args) bindings (set-args (cdr args) (set-binding (car args) (car args) bindings)))
  )
)

;----------- Supplied Utility Procedures -----------

; Returns #t iff symbol is an arithmetic operator that is allowed for use
; in infix expressions.
(define infix-operator?
    (lambda (symbol)
        (member symbol '(+ - * / ^ % < <= = > >=))
    )
)


; Returns #t iff symbol is bound a to builtin procedure
; Note: this code only works in MIT Scheme
(define builtin?
    (lambda (symbol)
        (if (not (symbol? symbol))
            (display-all symbol
             "is not a symbol---you are calling builtin?  incorrectly.\n"))
        (and (symbol? symbol)
             (environment-bound? () symbol)
             (procedure? (eval symbol ())))
    )
)

; Display an error message.
;  Allowed calls to this procedure:
;   (display-error 'unbound symbol)
;       displays error saying symbol is unbound
;   (display-error 'syntax)
;       displays an error saying the user entered an invalid expression
;   (display-error 'internal)
;       displays an internal error message--the use should never see it!
;   (display-error 'arity function)
;       displays an error saying a function was used with the wrong number
;       of arguments  (you will only need this for Bonus 2).
;   (display-error 'apply expr)
;       displays an error saying that expr is not a function
;       (you will only need this for Bonus 2).
(define display-error
    (lambda (type . args)
        (cond ((and (eq? type 'unbound) (pair? args)
                    (symbol? (car args)) (null? (cdr args)))
                  (display-all "Undefined symbol:" (car args) "\n"))
              ((and (eq? type 'syntax) (null? args))
                  (display-all "Invalid or unsupported expression.\n"))
              ((and (eq? type 'internal) (null? args))
                  (display-all "An internal error occurred--please contact"
                               "your system vendor for a patch.\n"))
              ((and (eq? type 'arity) (pair? args)
                    (or (symbol? (car args)) (procedure? (car args)))
                    (null? (cdr args)))
                  (display-all "Function" (car args)
                               "called with the wrong number of arguments.\n"))
              ((and (eq? type 'apply) (pair? args) (null? (cdr args)))
                  (display-all "The object" (car args) "is not a function."))
              (else
                  (display-all "Invalid call to display-error--fix"
                               "your code, the user should never see"
                               "this.\n"))
        )
    )
)

; Display a number of objects, with a space between each.
; You may use this instead of display, where convenient.
(define display-all
    (lambda lst
        (cond ((null? lst) ())
              ((null? (cdr lst)) (display (car lst)) ())
              (else (display (car lst)) (display " ")
                    (apply display-all (cdr lst))))
    )
)

; This bindings variable is defined for your convenience.
; Feel free to modify it for your own testing.
; Note: ` stands for quasiquote.  Like quote, quasiquote inhibits the
; evaluation of its argument, but it permits the use of , (unquote) to
; specify that some parts should still be evaluated, in this case the
; lambda expression.
(define bindings `((a . 1) (b . 2) (f . ,(lambda () (* 2 3)))))
; This variable is modified each time you call run.  It must be defined
; before set! can be called, so we define it to be the empty list initially.
(define last-bindings ())


;----------- Bindings -----------
; Returns the last inserted binding for symbol
; (as a pair ( symbol .  binding ) ) if symbol
; has a binding in bindings, () otherwise.
; Pre: symbol is a symbol, bindings is a list of bindings
(define get-binding
    (lambda (symbol bindings)
        (cond 
          ((null? bindings) '())
          ((eq? (bound-var (car bindings)) symbol) (car bindings))
          (else (get-binding symbol (cdr bindings)))
        ) 
    )
)

; Returns a new binding list with the binding ( symbol . value ) inserted
; Pre: symbol is a symbol, bindings is a list of bindings
(define set-binding
    (lambda (symbol value bindings)
        (cons (cons symbol value) bindings) 
    )
)

; Returns the variable symbol of the binding
;Pre: binding is a binding
(define bound-var
    (lambda (binding)
        (if (pair? binding) (car binding) '()) 
    )
)

; Returns the value of the binding
;Pre: binding is a binding
(define bound-value
    (lambda (binding)
        (if (pair? binding) (cdr binding) '())
    )
)

; Validate an expression from the user
; Returns true if every symbol in expr that is not
; a builtin procedure, ^ , or % has a binding in the
; bindings list
; Prints "Undefined Symbol: symbol" for every symbol that
; does not have a binding and returns false if
; any such symbols exist
; Pre: bindings is a list of bindings 
(define validate-bindings
  (lambda (expr bindings)
    (let ((validate-atom (lambda (atom) (if (or (number? atom) 
                                                (eq? '^ atom) 
                                                (eq? '% atom) 
                                                (get-binding atom bindings)) 
                                            #t 
                                            (display-error 'unbound atom)
                                        ))))  
      (cond 
        ((null? expr) #t) 
        ((pair? expr) (eval (cons 'and (map validate-atom (build-symbol-list expr '()))) '()))
        (else (validate-atom expr))
      )
    )
  )
)

; Builds and returns a list of symbols present in list
; A symbol is any member of list that is not a number or a builtin procedure
; Pre: list is a list, thelist is a list
(define build-symbol-list
  (lambda (list thelist)
    (if (null? list) 
        thelist
        (build-symbol-list (cdr list) (if (pair? (car list))
                                          (build-symbol-list (car list) thelist)
                                          (if (not (or (number? (car list)) 
                                                       (builtin? (car list)) 
                                                       (member (car list) thelist))
                                              ) 
                                              (cons (car list) thelist) 
                                              thelist
                                          )
                                      )
       )
     )
  )
)

; Build a proper Scheme expression from expr with bindings resolved using
; bindings, or builtin procedures
; Pre: bindings is a list of bindings
(define resolve-bindings
    (lambda (expr bindings)
        (if (list? expr) (resolve-list expr bindings) (or (resolve-atom expr bindings) (display-error 'internal)))
    )
)

; Build a proper Scheme expression from expr with bindings resolved using
; bindings, or builtin procedures
; Pre: expr is a list, bindings is a list of bindings
(define resolve-list
  (lambda (expr bindings)
    (cond 
      ((null? expr) '())
      ((list? (car expr)) (cons (resolve-list (car expr) bindings) (resolve-list (cdr expr) bindings)))
      (else (cons (or (resolve-atom (car expr) bindings) (display-error 'internal)) (resolve-list (cdr expr) bindings)))
    )
  )
)  
 
   
; If atom is number or a builtin procedure, or ^, or % returns atom
; Otherwise finds the binding associated with atom in the bindings list
; and returns the value associated with that binding
; Pre: bindings is a list of bindings
(define resolve-atom
  (lambda (atom bindings)
    (or (if (or (number? atom) (builtin? atom) (eq? '^ atom) (eq? '% atom)) atom '())
        (bound-value (get-binding atom bindings))
     )
   )
 )
        
              
;Convert any infix subexpressions to prefix notation
;Substitute expt for every occurence of ^, and modulo for every occurence of %
;Pre: If expr is a list then (length expr) <> 1, ^ and % are only used as infix operators
(define infix-to-prefix
  (lambda (expr)
    (if (or (null? expr) (not (list? expr))) 
        expr 
        (cond ((procedure? (car expr)) (cons (car expr) (map infix-to-prefix (cdr expr))))
              ((infix-operator? (cadr expr)) (map infix-to-prefix (cons* (cond ((eq? '^ (cadr expr)) expt)
                                                                               ((eq? '% (cadr expr)) modulo)
                                                                               (else (cadr expr))
                                                                         )
                                                                         (car expr) 
                                                                         (caddr expr) 
                                                                         '()
                                                                   )))
              (else (map infix-to-prefix expr))
        )
    )
  )
)
                                             