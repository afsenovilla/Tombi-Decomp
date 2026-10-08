// FUNC 8002052c 172 MAIN0
// MATCHING 8002052c 172
#include "TOBJ.H"
extern short D_800A4582;
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;

void func_8002052C(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (D_800A4582 + 0xa0 > y) {
        if ((unsigned short)(o->h->p.whole - D_1F800176 + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(D_1F800186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
