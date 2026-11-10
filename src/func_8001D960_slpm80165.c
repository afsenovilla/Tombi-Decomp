// VERSION SLPM_80165 slpm80165 8001D960 160
#if defined(VERSION_SLPM_80165)
// Demo version: BETA_980203 C of src/startMovieStream_beta980203.c (startMovieStream, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// Beta version: PAL startMovieStream.c with the CdControl and CdRead2 retry loops one after the other (PAL nested).
extern void g1(int a);
extern void g2(void *a);
extern void g3(void *a);
extern void g4(void *a, int b);
extern void g5(int a, int b, int c, int d, int e);
extern int g6(int a, int b, int c);
extern int g7(int a);
extern char mdecSliceCallback[], F3[], SPRITE_UNPACK_BUFFER[];

void startMovieStream(int a)
{
    int n = 0x1c0;
    g1(0);
    g2(mdecSliceCallback);
    g3(F3);
    g4(SPRITE_UNPACK_BUFFER, 0x20);
    g5(0, 1, -1, 0, 0);
    while (!g6(2, a, 0)) ;
    a = n;
    while (!g7(a)) ;
}
#endif
