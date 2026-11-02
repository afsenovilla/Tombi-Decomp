// FUNC 8001db38 164 MAIN0
// MATCHING 8001db38 164
// VERSION SLPS_01144 slps01144 8001DA50 160
#if defined(VERSION_SLPS_01144)
extern void DecDCTReset(int);
extern void DecDCToutCallback(void *);
extern void CdMix(void *);
extern void StSetRing(void *, int);
extern void StSetStream(int, int, int, int, int);
extern int CdControl(int, void *, void *);
extern int CdRead2(int);
extern void mdecSliceCallback(void);
extern char D_80071D68[], D_800D98F8[];

/* JP: CdControl(CdlSetloc, pos) is retried until it succeeds before the CdRead2 loop */
void func_8001DA50(int pos)
{
    DecDCTReset(0);
    DecDCToutCallback(mdecSliceCallback);
    CdMix(D_80071D68);
    StSetRing(D_800D98F8, 0x20);
    StSetStream(0, 1, -1, 0, 0);
    do {
    } while (!CdControl(2, (void *)pos, 0));
    pos = 0x1c0;
    do {
    } while (!CdRead2(pos));
}
#else
extern void g1(int a);
extern void g2(void *a);
extern void g3(void *a);
extern void g4(void *a, int b);
extern void g5(int a, int b, int c, int d, int e);
extern int g6(int a, int b, int c);
extern int g7(int a);
extern char F2[], F3[], SPRITE_UNPACK_BUFFER[];

void startMovieStream(int a)
{
    int n = 0x1c0;
    g1(0);
    g2(F2);
    g3(F3);
    g4(SPRITE_UNPACK_BUFFER, 0x20);
    g5(0, 1, -1, 0, 0);
    do {
        while (!g6(2, a, 0)) ;
    } while (!g7(n));
}
#endif
