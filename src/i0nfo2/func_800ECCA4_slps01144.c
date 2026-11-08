// VERSION SLPS_01144 info2 800ECCA4 320
// JP version of PAL i0nfo2 800eb550 (topdownPlayerHandler.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { unsigned short v; } SpW;
#define SPAD16(a) (((SpW *)(0x1F800000 + (a)))->v)
extern void func_800EB430(TObj *o);
extern void func_800EC5A8(TObj *o);
extern void func_800ECAA8(TObj *o);
extern unsigned short D_80076B94, D_80076B96;
extern signed char D_8009EE58[];
extern unsigned short D_800A7B80, D_800A7C3C;

void func_800ECCA4(TObj *o)
{
    signed char *m;
    if (o->subtype == 1) {
        D_800A7B80 = D_80076B94;
        D_800A7C3C = D_80076B96;
    }
    if (o->subtype == 2) {
        D_800A7B80 = D_80076B94;
        D_800A7C3C = D_80076B96;
    }
    m = D_8009EE58;
    if (*m >= 5) goto clr;
    if (*m < 3) {
    clr:
        *m = 0;
    }
    switch (o->b04) {
    case 0:
        func_800EB430(o);
        break;
    case 1:
        func_800EC5A8(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        break;
    case 4:
        func_800ECAA8(o);
        break;
    }
    SPAD16(0x16A) = o->h->p.whole;
    SPAD16(0x16E) = o->y.p.whole;
    SPAD16(0x172) = o->d->p.whole;
}
#endif
