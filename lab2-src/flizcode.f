;tests the ability to support defined functions
 
 (define (append t1 t2) 
      (ifn t1 t2
       (list (head t1) (append (tail t1) t2))))       
 
 (define (flatten x)
        (ifn x x
                (ifa x (list x []) 
                        (append (flatten (head x)) (flatten (tail x))))))


(define (intlist x)
        (ifn x 0 
                (ifa (head x) (intlist (tail x)) 1)))


(define (intlist2 x)
        (ifn x [] 
                (ifa (head x) (intlist2 (tail x)) [1])))




(define (reverse x)
        (ifn x x
                (ifa x (list x [])
                        (append (reverse(tail x)) (list (head x) [])))))


(define (recreverse x)
        (ifn x x
                (ifa x x
                        (append (recreverse(tail x)) (list (recreverse (head x)) [])))))



                        













