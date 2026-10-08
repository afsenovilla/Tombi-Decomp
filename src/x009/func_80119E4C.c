// FUNC 80119e4c 272 X009
// MATCHING 80119e4c 272
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } P3;
extern P3 D_8012AC88[];
extern short D_800A604A[];
extern short D_800A604E[];
extern short D_800A6052;
extern int rsin(int);
extern int rcos(int);

#define DBC (*(int *)&o->wbc)

void func_80119E4C(TObj *o)
{
    o->d38 = rsin(o->wb8) * DBC >> 20;
    o->d30 = rcos(o->wb8) * DBC >> 20;
    o->d34 = rsin(o->wba) * DBC >> 20;
    o->a.p.whole = D_8012AC88[o->wb4].x + o->d30;
    o->y.p.whole = D_8012AC88[o->wb4].y + (short)(o->d34 - 8);
    o->b.p.whole = D_8012AC88[o->wb4].z + o->d38;
    D_800A604A[0] = o->a.p.whole;
    D_800A604E[0] = o->y.p.whole;
    D_800A6052 = o->b.p.whole;
}
