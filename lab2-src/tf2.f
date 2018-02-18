; This tests support for built-in functions

(head [1 2 3])                      ; 1
(head [[1 2] 3 [4 5]])              ; [ 1 2 ]
(tail [1])                          ; [ ] 
(tail [[] [1 2] [3 4]])             ; [ 1 2 3 4]
(list (head [1 2]) (tail [1 3 4]))  ; [ 1 3 4 ]
(ifn (tail [1]) [] [1])             ; [ ]
(ifa 1 1 [1])                       ; 1
