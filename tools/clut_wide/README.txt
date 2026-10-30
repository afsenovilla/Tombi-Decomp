Wide-tolerance search for the Civilization scenery CLUTs (docs/CIVILIZATION.md, "Wide-tolerance palette search").
Work dir /dev/shm/clut2 (scripts expect it; run from the repo root). Order:
  python3 targets.py      custom FMV-derived CLUTs + page index weights (needs /dev/shm/clutgen)
  python3 corpus.py       every file of every disc, raw + GAM-unpacked, deduped, both byte phases -> corpus.u16
  gcc -O3 -fopenmp scan.c -o scan; bash runall.sh   raw max-channel scan, every word offset, strides 1/2/4/16
  python3 affine.py       FFT per-channel affine fit (gain 0.5..2) at every offset + best 16-entry block per CLUT row
  python3 posctl.py ...   positive control (perturbed real TIM CLUT); report.py: ranking + renders
Nothing produced here is committed.
