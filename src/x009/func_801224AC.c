// FUNC 801224ac 272 X009
// MATCHING 801224ac 272
#include "spad.h"
#include "gv2.h"
#include "TOBJ.H"

extern unsigned char D_8012B2D4[];
extern int Rand(void);

static __inline__ int Other(TObj *o)
{
    if (o->wb4 == 3) return 0;
    return o->wb4 != GV2.bb1;
}

static __inline__ int Near(TObj *o)
{
    if ((unsigned short)((*(unsigned short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole + 0x64) >= 0xf1) return 0;
    return (unsigned short)((*(unsigned short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole + 0x50) < 0xc9;
}

void func_801224AC(TObj *o)
{
    if (Other(o)) {
        o->step = 3;
        o->state = 0;
        return;
    }
    if (o->timer != 0) return;
    o->timer = 0xb4;
    if (Near(o)) {
        if (D_8012B2D4[Rand() & 7] == 0) {
            o->step = 7;
            o->state = 0;
        }
    } else {
        if (D_8012B2D4[Rand() & 7] == 0) {
            o->step = 2;
            o->state = 0;
        }
    }
}
