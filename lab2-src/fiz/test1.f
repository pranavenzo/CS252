; Evaluates to 0 if x < y, and 1 otherwise
(define (lt x y)
 (ifz y 1                       ; if y==0, x<y cannot be true, evaluates to 1
      (ifz x 0                  ; else if x==0, it must be x<y, evaluates to 0 
           (lt (dec x) (dec y))))) ; else x<y if and only if x-1 < y-1

; Evalutes x - y
(define (sub x y) 
  (ifz y x                      ; if y==0, x-y = x
       (ifz x (halt)            ; else if x==0, we halt because the result is negative
            (sub (dec x) (dec y)))))  ; else x-y = (x-1) - (y-1)

; Evaluets x mod y (i.e., x % y)
(define (rem x y)
  (ifz y (halt)                  ; Division by 0
       (ifz (lt x y) x           ; if x<y, x mod y = x
            (rem (sub x y) y)))) ; else x mod y = (x-y) mod y

; return 1 if a number between x and x+y-1, inclusive, divides z
(define (check x y z)
  (ifz y 0
     (ifz (rem z x) 1
         (check (inc x) (dec y) z))))

; return 0 if x is a prime number
(define (isp x)
  (ifz x 1
       (ifz (dec x) 1
            (check 2 (dec (dec x)) x))))
