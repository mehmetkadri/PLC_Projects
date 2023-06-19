#lang racket
(require typed-stack)

(define stack (empty-stack))

(define pop1 0)
(define pop2 0)

(define (postfix operation)
  (for/list ([i operation])
    (case (> (stack-length stack) 1)
      [(#t) (set! pop1 (pop! stack))])
    (case (> (stack-length stack) 0)
      [(#t) (set! pop2 (pop! stack))])
    (case (integer? i)
      [(#t) (push! stack pop2)(push! stack pop1)(push! stack i)]
      [(#f) (push! stack (i pop2 pop1))]))
  (pop! stack))

;;Example Input:
;;(postfix (list 3 2 - 1 5 8 + / 6 + *))