for p in A06100 A07600 A08000 A08600; do for s in 1 2 4 16; do ./scan corpus.u16 t_$p.bin $s 80 > res_${p}_$s.tsv; done; done; echo done > scan.done
