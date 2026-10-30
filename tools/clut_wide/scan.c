// raw tolerant scan: for every word offset and stride, weighted share of target indices within 6/9/12 levels
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>
#define K 300
typedef struct { long off; double sc, c6, c9, c12, md, u9; } Hit;
typedef struct { Hit h[K]; int n; } Top;
static void push(Top *t, Hit x) {
  for (int i = 0; i < t->n; i++)  // suppression: one hit per 256-word neighbourhood
    if (labs(t->h[i].off - x.off) < 256) { if (x.sc > t->h[i].sc) t->h[i] = x; return; }
  if (t->n < K) { t->h[t->n++] = x; return; }
  int m = 0; for (int i = 1; i < K; i++) if (t->h[i].sc < t->h[m].sc) m = i;
  if (x.sc > t->h[m].sc) t->h[m] = x;
}
int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "rb"); fseek(f, 0, 2); long N = ftell(f) / 2; fseek(f, 0, 0);
  uint16_t *w = malloc(N * 2); fread(w, 2, N, f); fclose(f);
  f = fopen(argv[2], "rb"); uint16_t tw[256]; float wt[256]; fread(tw, 2, 256, f); fread(wt, 4, 256, f); fclose(f);
  int stride = atoi(argv[3]);
  int ui[256], nu = 0; double W = 0; for (int i = 0; i < 256; i++) if (wt[i] > 0) { ui[nu++] = i; W += wt[i]; }
  uint8_t *D = malloc(256L * 32768);
  for (int k = 0; k < nu; k++) { int i = ui[k], t = tw[i];
    for (int c = 0; c < 32768; c++) { int m = 0;
      for (int s = 0; s < 15; s += 5) { int d = abs(((c >> s) & 31) - ((t >> s) & 31)); if (d > m) m = d; }
      D[(long)k * 32768 + c] = m; } }
  int nt = omp_get_max_threads(); int mindist = atoi(argv[4]); int **stamp = malloc(nt*sizeof(int*)); int *gen = calloc(nt, sizeof(int)); for (int t=0;t<nt;t++) stamp[t]=calloc(65536,sizeof(int));
  Top *tops = calloc(2 * nt, sizeof(Top));
  long last = N - 255L * stride;
  #pragma omp parallel for schedule(dynamic, 1 << 16)
  for (long o = 0; o < last; o++) {
    int th = omp_get_thread_num(); double c6 = 0, c9 = 0, c12 = 0, md = 0; int u9 = 0;
    const uint16_t *p = w + o;
    if (p[0] == p[255L*stride] && p[0] == p[128L*stride] && p[0] == p[64L*stride]) {
      // fast skip of flat runs: still evaluate, cheap enough? skip exact-constant windows
      int flat = 1; for (int i = 0; i < 256 && flat; i += 8) flat = p[(long)i*stride] == p[0]; if (flat) continue; }
    for (int k = 0; k < nu; k++) { int i = ui[k]; int d = D[(long)k * 32768 + (p[(long)i * stride] & 0x7fff)];
      float x = wt[i]; md += x * d; if (d <= 12) { c12 += x; if (d <= 9) { c9 += x; u9++; if (d <= 6) c6 += x; } } }
    { int g = ++gen[th], nd = 0; for (int k = 0; k < nu; k++) { int v = p[(long)ui[k]*stride]; if (stamp[th][v] != g) { stamp[th][v] = g; nd++; } } if (nd < mindist) continue; }
    Hit h = { o, 0, c6 / W, c9 / W, c12 / W, md / W, (double)u9 / nu };
    h.sc = h.c9; push(&tops[2 * th], h);
    h.sc = -h.md; push(&tops[2 * th + 1], h);
  }
  Top all[2]; memset(all, 0, sizeof all);
  for (int t = 0; t < nt; t++) for (int m = 0; m < 2; m++) for (int i = 0; i < tops[2*t+m].n; i++) push(&all[m], tops[2*t+m].h[i]);
  for (int m = 0; m < 2; m++) for (int i = 0; i < all[m].n; i++) { Hit h = all[m].h[i];
    printf("%s\t%d\t%ld\t%.4f\t%.4f\t%.4f\t%.3f\t%.4f\n", m ? "mean" : "c9", stride, h.off, h.c6, h.c9, h.c12, h.md, h.u9); }
  return 0;
}
