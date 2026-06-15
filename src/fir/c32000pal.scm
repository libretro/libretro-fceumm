(title "32000 PAL")

(verbose #t)

(cosine-symmetry)
(filter-length 1024)
(sampling-frequency 1662607.125)
(limit-= (band 0 11500) 1)
(limit-= (band 16000 831303.5625) 0 .5)
(output-file "c32000pal.coef")
(plot-file "c32000pal.plot")
(go)
