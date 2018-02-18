(define (append t1 t2) 
 (ifn t1 t2
  (list (head t1) (append (tail t1) t2))))

(define (flatten x)
   (ifn x x
      (ifa x (list x []) 
         (append (flatten (head x)) (flatten (tail x))))))

(define (f x) (append [1 2] [3 4 5])) 
(define (g y) (list (head (tail y)) (list (head y) [])))

(g (f 1)) 
(g (f 2))

(flatten [1 [2 [3 4] 5]])         ; [ 1 2 3 4 5 ]
(flatten [1 [2 [] [[] 3 [4]] 5]] ) ; [ 1 2 3 4 5 ]
(append [1 2] [3 4 5])       ; [ 1 2 3 4 5 ]
(append [1 [2]] [[[3]] 4 5]) ; [ 1 [ 2 ] [ [ 3 ] ] 4 5 ]

;; Exit at this point, record memory usage info.

(g (f 3)) 
(g (f 4))
(g (f 5)) 
(g (f 6))

(flatten [1 [2 [3 4] 5]])         ; [ 1 2 3 4 5 ]
(flatten [1 [2 [] [[] 3 [4]] 5]] ) ; [ 1 2 3 4 5 ]
(append [1 2] [3 4 5])       ; [ 1 2 3 4 5 ]
(append [1 [2]] [[[3]] 4 5]) ; [ 1 [ 2 ] [ [ 3 ] ] 4 5 ]
(flatten (append [1 [2 [3 [4 5 6 7 9 10] 11] 12]] [[3] [4] [5]]))


;; Exit again at this point, record memory usage info.
(g (f 3)) 
(g (f 4))
(g (f 5)) 
(g (f 6))

(flatten [1 [2 [3 4] 5]])         ; [ 1 2 3 4 5 ]
(flatten [1 [2 [] [[] 3 [4]] 5]] ) ; [ 1 2 3 4 5 ]
(append [1 2] [3 4 5])       ; [ 1 2 3 4 5 ]
(append [1 [2]] [[[3]] 4 5]) ; [ 1 [ 2 ] [ [ 3 ] ] 4 5 ]
(flatten (append [1 [2 [3 [4 5 6 7 9 10] 11] 12]] [[3] [4] [5]]))


