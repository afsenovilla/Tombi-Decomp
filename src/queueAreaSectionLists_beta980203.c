// VERSION BETA_980203 beta980203 800214F4 200
#if defined(VERSION_BETA_980203)
// Beta version: PAL queueAreaSectionLists.c with the beta's two tables, the beta queueLoadList (800215BC) and the load thread started here.
extern int LOAD_NEXT_ADDR, LOAD_SLOTS, g2b0;
extern unsigned char LOAD_COMPLETE_SYM;
extern int D_800A1478[];   /* per-area load lists (PAL AREA_LOAD_LISTS[a][0]) */
extern int *D_800A1704[];  /* per-area section load lists */
extern unsigned short D_801FD8E0;
extern void SfxStopAll(int);
extern void func_800215BC_beta980203(int);
extern void func_80020A24(void);
extern void ThreadCreate(int, void (*)(void));

void queueAreaSectionLists(int a, int b, int c)
{
    if (c == 0) {
        SfxStopAll(1);
    } else {
        func_800215BC_beta980203(D_800A1478[a]);
        LOAD_SLOTS = LOAD_NEXT_ADDR;
    }
    func_800215BC_beta980203(D_800A1704[a][b]);
    LOAD_COMPLETE_SYM = 0;
    g2b0 = LOAD_NEXT_ADDR;
    if (D_801FD8E0 == 0)
        ThreadCreate(2, func_80020A24);
}
#endif
