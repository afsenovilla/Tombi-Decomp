// FUNC 80118cd4 344 X010
// MATCHING 80118cd4 344
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern void playSFX(int);

void sinkingLedgeCheckPlayer(TObj *o)
{
    unsigned char *p;
    unsigned int x;
    unsigned char st;

    st = o->state;
    switch (st) {
    case 0:
        if ((*(short*)((char*)&SPAD.i170 + 2)) != 0x5a) break;
        if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x54) break;
        p = &GVARS.b3f;
        if (*p) break;
        if (o->subtype == 0) x = (*(unsigned short*)((char*)&SPAD.i168 + 2)) - 0x68b;
        else x = (*(unsigned short*)((char*)&SPAD.i168 + 2)) - 0xab3;
        if (x >= 0x45) break;
        o->state = st + 1;
        o->timer = 0x10;
        GCD.bcf06 = 0;
        *p = 1;
        GVARS.hurtState = 1;
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
