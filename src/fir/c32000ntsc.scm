(title "32000 NTSC")

(verbose #t)

(cosine-symmetry)
(filter-length 1024)
(sampling-frequency 1789772.727272727272)
(limit-= (band 0 11000) 1)
(limit-= (band 16000 894886.363636) 0 .5)
(output-file "c32000ntsc.coef")
(plot-file "c32000ntsc.plot")
(go)
