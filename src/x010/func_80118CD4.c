// FUNC 80118cd4 344 X010
// MATCHING 80118cd4 344
#include "TOBJ.H"

extern short D_1F800172, D_1F80016E;
extern unsigned short D_1F80016A;
extern unsigned char D_8009C93F, D_8009CF06, D_8009C938, D_8009C93E;
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
        p = &D_8009C93F;
        if (*p) break;
        if (o->subtype == 0) x = D_1F80016A - 0x68b;
        else x = D_1F80016A - 0xab3;
        if (x >= 0x45) break;
        o->state = st + 1;
        o->timer = 0x10;
        D_8009CF06 = 0;
        *p = 1;
        D_8009C938 = 1;
        D_8009C93E = 1;
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
