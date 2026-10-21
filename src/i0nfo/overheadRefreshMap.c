// FUNC 800ec084 432 I0NFO
// MATCHING 800ec084 432
#include "TOBJ.H"
#include "spad.h"
extern void func_800174C0(void *p, int x, int y, int w, int h);
extern void func_8005F0FC(int a);
extern void overheadBuildMapStrip(int n, void *p);
extern void overheadBuildVisibleTileList(TObj *o);
extern short D_1F800176[];
extern short D_1F80017A[];
extern unsigned char D_800A60A3;
extern void *D_800EE8CC;

static __inline__ void ld(int n, int x, int y, int w, int h)
{
    void *p = D_800EE8CC;
    overheadBuildMapStrip(n, p);
    func_800174C0(p, x, y, w, h);
}

void overheadRefreshMap(TObj *o)
{
    void *p;
    o->d30 = D_1F800176[0] + 0xA1;
    o->d34 = (short)((SPADBlock *)0x1F800000)->s17a - 0xEF;
    o->h->p.whole = 0x1F - ((D_1F800176[0] + 0xA0) & 0x1F);
    o->y.p.whole = 0x1F - (((short)((SPADBlock *)0x1F800000)->s17a - 0xF0) & 0x1F);
    overheadBuildVisibleTileList(o);
    ld(0, 0x300, 0, 0x80, 0x100);
    func_8005F0FC(0);
    ld(1, 0x380, 0, 0x40, 0x100);
    ld(2, 0x40, 0, 0x40, 0x20);
    ld(3, 0x40, 0x20, 0x40, 0x20);
    ld(4, 0x40, 0x40, 0x40, 0x20);
    switch (D_800A60A3) {
    case 0: o->b6b = 0; break;
    case 1: o->b6b = 0x80; break;
    default: o->b6b = 0xFF; break;
    }
}
