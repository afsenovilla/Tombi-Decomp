// FUNC 80118cd4 344 X010
// MATCHING 80118cd4 344
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern short D_1F800172, D_1F80016E;
extern unsigned short D_1F80016A;
extern void playSFX(int);

void func_80118CD4(TObj *o)
{
    unsigned char *p;
    unsigned int x;
    unsigned char st;

    st = o->state;
    switch (st) {
    case 0:
        if (D_1F800172 != 0x5a) break;
        if (D_1F80016E < -0x54) break;
        p = &GVARS.b3f;
        if (*p) break;
        if (o->subtype == 0) x = D_1F80016A - 0x68b;
        else x = D_1F80016A - 0xab3;
        if (x >= 0x45) break;
        o->state = st + 1;
        o->timer = 0x10;
        GCD.bcf06 = 0;
        *p = 1;
        GVARS.b38 = 1;
        GVARS.b3e = 1;
        break;
    case 1:
        o->y.p.whole += 3;
        if (--o->timer == -1) o->state++;
        break;
    case 2:
        o->state = st + 1;
        playSFX(0xa3);
        break;
    }
}
