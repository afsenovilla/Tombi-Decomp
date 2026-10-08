// FUNC 800f43d0 224 X006
// MATCHING 800f43d0 224
#include "TOBJ.H"
typedef struct { char p[7]; signed char b7; } U;
extern U *D_8009C330;

void func_800F43D0(TObj *o)
{
    switch (D_8009C330->b7) {
    case 0:
    case 2:
        o->h->p.whole += 0x10;
        o->y.p.whole += 4;
        break;
    case 1:
    case 3:
        o->h->p.whole -= 0x10;
        o->y.p.whole += 4;
        break;
    case 4:
        o->h->p.whole -= 0xc;
        o->y.p.whole -= 0xc;
        break;
    case 5:
        o->h->p.whole -= 0xc;
        o->y.p.whole -= 0xc;
        break;
    case 6:
    case 7:
        o->y.p.whole -= 0x10;
        break;
    }
    o->b69 = 0;
}
