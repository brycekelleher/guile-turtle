(define (draw-line len numsides)
  (begin (turtle-move len) 
         (turtle-turn (/ 360 numsides))))

(define (draw-polygon len numsides)
  (define (draw-polygon-iter len n)
    (begin (draw-line len numsides)
           (if (> n 0)
	       (draw-polygon-iter len (- n 1)))))
  (draw-polygon-iter len numsides))

(define (draw-triangle len)
  (draw-polygon len 3))

(define (draw-square len)
  (draw-polygon len 4))

(define (draw-hex len)
  (draw-polygon len 6))

