;;; The LINC memory cycle is 8 microseconds.  Within that cycle, there
;;; are four time pulses.  For e.g. the ROR instruction, one bit is
;;; shifted each time pulse; hence the rate is 500 kHz.  This is very
;;; much above the audible range, but the idea is this can be used to
;;; enact a duty cycle for the output signal.  Thus the amplitude of
;;; output can be modulated in the audible range.
;;;
;;; As for the emulator, to downsample the 500 kHz sample rate to 48
;;; kHz suitable for a sound card, the signal can be passed through a
;;; Gaussian filter.  This will filter out high frequencies and
;;; distribute their energy into lower frequencies.  The filtered
;;; signal is sampled at a 48 kHz rate.

;; The ratio between the input and the output sample rate.
(defvar *ratio* (/ 500e3 48e3))

(defun gaussian (x)
  (let* ((q 1.0)
         (area (* q *ratio*))
         (y (exp (* -1 (/ pi (* area area)) (* x x)))))
    (/ y q)))

;;; The input samples.  The plot functions work withing a window from
;;; 1000 to 1065.  This vector provides plenty of data outside that
;;; window for the filter extent to act on.
(defvar *data*
  (let ((x (make-vector 3000 0)))
    ;;Two bursts of square waves, similar to what two ROR instructions might do.
    (loop for i from 1005 to 1018 do (setf (elt x i) (logand i 1)))
    (loop for i from 1040 to 1053 do (setf (elt x i) (logand i 1)))
    x))

(defun scale-x (x)
  (* 20 (- x 1000)))

(defun scale-y (y)
  (- 200 (* 100 y)))

(defun plot-square (svg data color)
  (svg-polyline svg 
                (loop for x from 1000 to 1065
                      for x1 = (scale-x x)
                      for sample0 = (elt *data* x)
                      for sample1 = (elt *data* (1+ x))
                      append `((,(+ x1 -10) . ,(scale-y sample0))
                               (,(+ x1 -10) . ,(scale-y sample1))
                               (,(+ x1 +10) . ,(scale-y sample1))))
                :stroke-color color :stroke-width 2 :fill "none"))

(defun plot-smooth (svg fn color)
  (svg-polyline svg
                (loop with dx = (/ 1 20.0)
                      for x from 1000 to 1065 by dx
                      for x1 = (scale-x x)
                      for y0 = (scale-y (funcall fn x))
                      for y1 = (scale-y (funcall fn (+ x dx)))
                      append `((,x1 . ,y0) (,(1+ x1) . ,y1)))
                :stroke-color color :stroke-width 2 :fill "none"))

(defun filter (fn data x extent dx)
  (loop for i from (- extent) to extent by dx
        sum (* (funcall fn i) (elt data (round (+ x i))) dx)))

(defun filtered (x)
  (/ (filter #'gaussian *data* x 15 0.2) *ratio*))

(defun downsampled (x)
  (filtered (* (round (/ x *ratio*)) *ratio*)))

(let ((svg (svg-create 1300 300)))
  ;;Input square wave at 500 kHz rate.
  (plot-square svg *data* "red")
  ;;Shape of the gaussian filter.
  (plot-smooth svg (lambda (x) (gaussian (- x 1027))) "blue")
  ;;Filtered signal.
  (plot-smooth svg #'filtered "green")
  ;;Output downsampled signal at 48 kHz rate.
  (plot-smooth svg #'downsampled "black")
  (insert-image (svg-image svg)))
