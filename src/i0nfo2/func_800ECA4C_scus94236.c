// VERSION SCUS_94236 info2 800ECA4C 280
#if defined(VERSION_SCUS_94236)
// USA version of PAL i0nfo 800ec084 (overheadRefreshMap.c)
#include "TOBJ.H"
#include "spad.h"
extern void func_8001745C(void *p, int x, int y, int w, int h);
extern void func_8005EB54(int a);
extern void overheadBuildMapStrip(int n, void *p);
extern void overheadBuildVisibleTileList(TObj *o);
extern short SCREEN_ORIGIN_X[];
extern short D_1F80017A[];
extern unsigned char D_800A5403;
extern void *OVERHEAD_MAP_STRIP_BUFFER;

static __inline__ void ld(int n, int x, int y, int w, int h)
{
    void *p = OVERHEAD_MAP_STRIP_BUFFER;
    overheadBuildMapStrip(n, p);
    func_8001745C(p, x, y, w, h);
}

void overheadRefreshMap(TObj *o)
{
    void *p;
    o->d30 = SCREEN_ORIGIN_X[0] + 0xA1;
    o->d34 = (short)((SPADBlock *)0x1F800000)->s17a - 0xEF;
    o->h->p.whole = 0x1F - ((SCREEN_ORIGIN_X[0] + 0xA0) & 0x1F);
    o->y.p.whole = 0x1F - (((short)((SPADBlock *)0x1F800000)->s17a - 0xF0) & 0x1F);
    overheadBuildVisibleTileList(o);
    ld(0, 0x300, 0, 0x80, 0x100);
    func_8005EB54(0);
    ld(1, 0x380, 0, 0x40, 0x100);
    if (D_800A5403 == 0) {
        o->b6b = 0x80;
    } else {
        o->b6b = 0;
    }
}
#endif
