// FUNC 801249d0 256 X014
// MATCHING 801249d0 256
#include "spad.h"
#include "TOBJ.H"

extern unsigned char D_801266F8[], D_80126708[];
extern int Rand(void);

static __inline__ int Near(TObj *o, int r, short lim)
{
    if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + r) > lim) return 0;
    return (unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + r) <= lim;
}

int deepJunglePigShotPickHop(TObj *o)
{
    if (Near(o, 0x40, 0x80)) return D_801266F8[Rand() & 0xf];
    if (Near(o, 0x80, 0x100)) return D_80126708[Rand() & 0xf];
    return 0;
}
