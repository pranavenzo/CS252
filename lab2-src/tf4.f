; This tests the ability to support defined functions
(define (append t1 t2) 
 (ifn t1 t2
  (list (head t1) (append (tail t1) t2))))

(define (flatten x)
   (ifn x x
      (ifa x (list x []) 
         (append (flatten (head x)) (flatten (tail x))))))

(flatten [1 [2 [3 4] 5]])         ; [ 1 2 3 4 5 ]
(flatten [1 [2 [] [[] 3 [4]] 5]] ) ; [ 1 2 3 4 5 ]
