// FUNC 8011886c 300 X001
// MATCHING 8011886c 300
#include "spad.h"
#include "TOBJ.H"

extern short NEG_SIN_TABLE[];
extern short COS_TABLE[];
extern int ObjCullRegister(TObj *);

void blockDebrisFly(TObj *o)
{
    int n;
    int amp;

    n = o->d88;
    amp = COS_TABLE[n];
    o->d88 = n + 1;
    o->h->raw += (COS_TABLE[o->d84] * ((short)amp >> 3)) >> 4;
    o->y.raw += (NEG_SIN_TABLE[o->d84] * ((short)amp >> 3)) >> 4;
    if (o->timer < 0x1e) {
        if ((SPAD.s1f8 + SPAD.i198) & 1) {
            ObjCullRegister(o);
        }
    } else if (ObjCullRegister(o) == 0) {
        o->b04 = 3;
    }
    if (--o->timer == -1) {
        o->b04 = 3;
    }
}
