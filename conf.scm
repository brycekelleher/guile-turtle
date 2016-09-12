(use-modules (ice-9 readline))
(activate-readline)
(begin
 (start-gnuplot)
 (turtle-reset)
 (top-repl)
 (stop-gnuplot))

