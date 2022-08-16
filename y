(declare-const vA Int)
(assert (< vA 10))
(assert (not (< vA 0)))

(declare-const vB Int)
(assert (< vB 10))
(assert (not (< vB 0)))

(declare-const vC Int)
(assert (< vC 10))
(assert (not (< vC 0)))

(declare-const vD Int)
(assert (< vD 10))
(assert (not (< vD 0)))

(declare-const vE Int)
(assert (< vE 10))
(assert (not (< vE 0)))

(declare-const x0_0 Int)
(declare-const y0_0 Int)
(declare-const x0_1 Int)
(declare-const y0_1 Int)
(declare-const x0_2 Int)
(declare-const y0_2 Int)


(assert (not (= 0 vA)))
(assert (not (= 0 vD)))
(assert (not (= 0 vE)))

(assert (= x0_0 (- (+ 0 vC) vD)))
(assert (= x0_1 (- (+ 0 vB) vB)))
(assert (= x0_2 (- (+ 0 vA) vD)))

(assert (= y0_0 (+ x0_0)))
(assert (= y0_1 (+ x0_1 (div y0_0 10))))
(assert (= y0_2 (+ x0_2 (div y0_1 10))))

(assert (>= y0_2 0))

(assert (= vE (mod y0_0 10)))
(assert (= vE (mod y0_1 10)))

(check-sat)
(get-model)
