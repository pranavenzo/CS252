; Below are my implementations of the basic functions
; adds two numbers x and y
(define (add x y)
        (ifz y x (add (inc x) (dec y))))

; multiplies two numbers x and y
(define (mul x y)
        (ifz y 0 (add x (mul x (dec y)))))

; subtracts y from x
(define (sub x y)
        (ifz y x (sub (dec x) (dec y))))

; returns 1 if x < y
(define (lt x y)
        (ifz y 0
                (ifz x 1 (lt (dec x) (dec y)))))

; returns 1 if x = y
(define (eq x y)
        (ifz y (ifz x 1 0) (ifz x 0 (eq (dec x) (dec y)))))


; divide x by y
(define (div x y)
        (ifz y (halt)
                (ifz (lt x y) (inc (div (sub x y) y)) 0 )))

; compute gcd(x,y) using the euclidean algorithm
(define (gcd x y)
        (ifz x 0 
                (ifz y 0
                        (ifz (eq x y) (ifz (lt x y) (gcd (sub x y) y) (gcd x (sub y x)))
                                x ))))


; compute the lcm(x,y) using the relation: x*y = gcd(x,y)*lcm(x,y)
(define (lcm x y)
       (div (mul x y) (gcd x y)))

; helper to the sroot function
(define (srootH z x)
        (ifz z 0 
                (ifz (lt z (mul x x)) (srootH z (inc x)) (dec x))))

; compute the squareroot of z. Return the largest x^2 <= z.
(define (sroot z)
        (srootH z 0))
        

; Finds x%y
(define (modulus x y)
        (ifz x 0 
                (ifz (lt x y) (modulus (sub x y) y)  x)))


; helper to (minv x z)
(define (minvH x z y)
        (ifz (eq 1 (modulus (mul x y) z)) (minvH x z (inc y)) y))
        

; Implement (minv x z), which gives the smallest positive integer y such that  xy % z=1.  (Dividing xy with z results in a remainder
;of 1.)  (minv x z) exists only when gcd(x,z)=1.  When gcd(x,z) does not equal 1, just use (halt) to quite the interpreter.
(define (minv x z)
        (ifz (eq 1 (gcd x z)) (halt) (minvH x z 1)))










