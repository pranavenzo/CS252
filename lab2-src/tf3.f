; This tests the ability to support defined functions.
(define (append t1 t2) 
 (ifn t1 t2
  (list (head t1) (append (tail t1) t2))))

(append [1 2] [3 4 5])       ; [ 1 2 3 4 5 ]
(append [1 [2]] [[[3]] 4 5]) ; [ 1 [ 2 ] [ [ 3 ] ] 4 5 ]
