// VERSION SLPM_80165 slpm80165 8001F888 224
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of queueAreaSectionLists (src/queueAreaSectionLists_beta980203.c): two demo calls instead of
// SfxStopAll. Beta version: PAL queueAreaSectionLists.c with the beta's two tables, the beta queueLoadList (800215BC) and the load thread started here.
extern int LOAD_NEXT_ADDR, LOAD_SLOTS, g2b0;
extern unsigned char LOAD_COMPLETE_SYM;
extern int D_800A1478[];   /* per-area load lists (PAL AREA_LOAD_LISTS[a][0]) */
extern int *D_800A1704[];  /* per-area section load lists */
extern unsigned short D_801FD8E0;
extern void func_8001EC84(void);
extern void func_8001EC14(void);
extern void func_8001F968(int);
extern void func_8001EE10(void);
extern void ThreadCreate(int, void (*)(void));

void queueAreaSectionLists(int a, int b, int c)
{
    if (c != 0)
        func_8001EC84();
    else
        func_8001EC14();
    if (c != 0) {
        func_8001F968(D_800A1478[a]);
        LOAD_SLOTS = LOAD_NEXT_ADDR;
    }
    func_8001F968(D_800A1704[a][b]);
    LOAD_COMPLETE_SYM = 0;
    g2b0 = LOAD_NEXT_ADDR;
    if (D_801FD8E0 == 0)
        ThreadCreate(2, func_8001EE10);
}
#endif
