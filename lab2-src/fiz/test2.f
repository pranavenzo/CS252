; Evalutes x+y
(define (add x y) 
  (ifz y x                       ; if y==0, x+y=x
       (add (inc x) (dec y))))   ; otherwise x+y = (x+1) + (y-1)

; Evalutes x - y
(define (sub x y) 
  (ifz y x                      ; if y==0, x-y = x
       (ifz x (halt)            ; else if x==0, we halt because the result is negative
            (sub (dec x) (dec y)))))  ; else x-y = (x-1) - (y-1)

; Evaluates to 0 if x < y, and 1 otherwise
(define (lt x y)
 (ifz y 1                       ; if y==0, x<y cannot be true, evaluates to 1
      (ifz x 0                  ; else if x==0, it must be x<y, evaluates to 0 
           (lt (dec x) (dec y))))) ; else x<y if and only if x-1 < y-1

; Evaluets x * y
(define (mul x y)
  (ifz y 0                       ; if y==0, x*y=0
       (add (mul x (dec y)) x))) ; esle x*y = x + x*(y-1)

; Evaluets x / y, only the quotient part
(define (div x y)
  (ifz y (halt)                  ; Division by 0
       (ifz (lt x y) 0           ; if x < y, evaluates to 0
            (inc (div (sub x y) y)))))  ; else x/y = 1+(x-y)/y

; Evaluates factorial of x
(define (fac x)
  (ifz x 1
       (mul (fac (dec x)) x)))

; Evaluates x^e, in an inefficient way
(define (sexp x e)
  (ifz e 1 
       (mul (sexp x (dec e)) x)))

(define (exp x e)
  (ifz e 1
       (ifz (rem e 2)
            (square (exp x (div e 2)))
            (mul (exp x (dec e)) x))))

; Evaluets x mod y (i.e., x % y)
(define (rem x y)
  (ifz y (halt)                  ; Division by 0
       (ifz (lt x y) x           ; if x<y, x mod y = x
            (rem (sub x y) y)))) ; else x mod y = (x-y) mod y

(define (square x)
  (mul x x))

; Evaluets x^e mod y in a slow way
(define (msexp x e y)
  (ifz e 1
     (rem (mul (msexp x (dec e) y) x) y)))
  
; Evaluets x^e mod y
(define (mexp x e y)
  (ifz e 1                       ; if e==0, then x^e = 1
       (ifz (rem e 2)            ; if e % 2 == 0
            (rem (square (mexp x (div e 2) y)) y) ; x^e mod y = (x^(e/2) mod y)^2 mod y
            (rem (mul (mexp x (dec e) y) x) y)))) ; x^e mod y = (x * x^(e-1) mod y) mod y

; Helper function to compute gcd of x, y.  
; On entering the function, we are guaranteed that 
;      among 1 to z, the largest common divisor of (x,y) is c
;      and x<y
; Logic of the code
; (gcdh c z x y) = c if x<z
; (gcdh c z x y) = (gcdh z (z+1) x y) if z divides both x and y
; (gcdh c z x y) = (gcdh c (z+1) x y) if z does not divide x or z does not divide y
(define (gcdh c z x y)
  (ifz (lt x z) c
       (ifz (rem x z)
            (ifz (rem y z)
                 (gcdh z (inc z) x y)
                 (gcdh c (inc z) x y))
            (gcdh c (inc z) x y))))

(define (gcd x y)
  (ifz (lt x y)
       (gcdh 1 2 x y)
       (gcdh 1 2 y x)))

; Logic of the above gcd implementation expressed in C
; int gcd(int x, int y) {
;   c=1
;   for (int z=2; z<=x; z++} {
;     if ((x%z==0) && (y%z==0)) {
;       c=z;
;     }
;   }
; }

; Below is gcd using euclidean algorithm
(define (gcde x y) 
  (ifz (lt x y) (gcde y x)
    (ifz y x (gcde y (rem x y)))))

(gcde 12 18)
(mexp 2 4 11)
