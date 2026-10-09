// FUNC 80119648 688 X010
// MATCHING 80119648 688
#include "TOBJ.H"
#include "raw7.h"

static __inline__ void mv(TObj *o)
{
    int x, l, v;
    short f;

    v = o->velH;
    if (v > 0) {
        x = o->d38 + v;
        l = S32(o, 0xb4);
        o->d38 = x;
        if (l < x) goto set;
        if (o->b0c) return;
        l = o->d34;
        f = l < x;
    } else {
        x = o->d38 + v;
        l = S32(o, 0xb8);
        o->d38 = x;
        if (x < l) goto set;
        if (o->b0c) return;
        l = o->d34;
        f = x < l;
    }
    if (f) {
    set:
        o->d38 = l;
        o->velH = 0;
    }
}

void func_80119648(TObj *o)
{
    switch (o->subtype) {
    case 0:
        mv(o);
        o->d30 = (o->d38 >> 8) & 0xfff;
        o->d8c = (0x1000 - o->d30) & 0xfff;
        break;
    case 1:
        mv(o);
        o->d30 = (o->d38 >> 8) & 0xfff;
        o->d8c = (0x1800 - o->d30) & 0xfff;
        break;
    case 2:
        mv(o);
        o->d30 = (o->d38 >> 8) & 0xfff;
        o->d8c = (0x1800 - o->d30) & 0xfff;
        break;
    case 3:
        mv(o);
        o->d30 = (o->d38 >> 8) & 0xfff;
        o->d8c = (0x1400 - o->d30) & 0xfff;
        break;
    }
}
