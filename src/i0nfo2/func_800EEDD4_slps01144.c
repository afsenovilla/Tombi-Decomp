// VERSION SLPS_01144 info2 800EEDD4 280
// JP version of PAL i0nfo 800ec084 (overheadRefreshMap.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "spad.h"
extern void func_80016B94(void *p, int x, int y, int w, int h);
extern void func_8005BA48(int a);
extern void func_800EE1E4(int n, void *p);
extern void overheadBuildVisibleTileList(TObj *o);
extern short SCREEN_ORIGIN_X[];
extern short D_1F80017A[];
extern unsigned char D_800A7BE3;
extern void *D_800FFDF8;

static __inline__ void ld(int n, int x, int y, int w, int h)
{
    void *p = D_800FFDF8;
    func_800EE1E4(n, p);
    func_80016B94(p, x, y, w, h);
}

void func_800EEDD4(TObj *o)
{
    void *p;
    o->d30 = SCREEN_ORIGIN_X[0] + 0xA1;
    o->d34 = (short)((SPADBlock *)0x1F800000)->s17a - 0xEF;
    o->h->p.whole = 0x1F - ((SCREEN_ORIGIN_X[0] + 0xA0) & 0x1F);
    o->y.p.whole = 0x1F - (((short)((SPADBlock *)0x1F800000)->s17a - 0xF0) & 0x1F);
    overheadBuildVisibleTileList(o);
    ld(0, 0x300, 0, 0x80, 0x100);
    func_8005BA48(0);
    ld(1, 0x380, 0, 0x40, 0x100);
    if (D_800A7BE3 == 0) {
        o->b6b = 0x80;
    } else {
        o->b6b = 0;
    }
}
#endif
