// FUNC 80119e4c 272 X009
// MATCHING 80119e4c 272
#include "player.h"
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } P3;
extern P3 D_8012AC88[];
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
    ((short*)&PLAYER.obj.a.p.whole)[0] = o->a.p.whole;
    ((short*)&PLAYER.obj.y.p.whole)[0] = o->y.p.whole;
    PLAYER.obj.b.p.whole = o->b.p.whole;
}
