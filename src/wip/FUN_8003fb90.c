// FUNC 8003fb90 488 MAIN0
#include "TOBJ.H"
extern short DAT_1f80027e;
extern unsigned char DAT_1f8001d2;
extern unsigned short DAT_1f800284[], DAT_1f800282;
typedef struct { char pad[0xa0]; unsigned char ba0; } XA0;
#define BA0(o) (((XA0 *)(o))->ba0)

void FUN_8003fb90(TObj *o)
{
    char pad[4];
    short a;
    unsigned int w;
    unsigned int hi;
    int n, m;
    a = DAT_1f80027e;
    if (a < 0)
        a = -a;
    if (a > 8)
        a = 8;
    if (DAT_1f80027e < 0)
        a = -a;
    o->b69 = 1;
    o->wae = DAT_1f800284[0];
    DAT_1f8001d2 = 1;
    BA0(o) = 0;
    o->bbe = 0;
    o->wb0 = a;
    w = DAT_1f800282;
    hi = w >> 15;
    m = (w >> 5) & 0xf;
    n = m;
    if (!(w & 0x3000)) {
        switch (n) {
        case 0: o->bbe = 0; break;
        case 1: o->bbe = 2; break;
        case 2: o->bbe = 0x10; break;
        case 3: o->bbe = 0x20; break;
        }
        o->bbe = hi | o->bbe;
        return;
    }
    if (w & 0x2000) {
        n = (unsigned)n >> 2;
        switch (n) {
        case 0: BA0(o) = 2; break;
        case 1: BA0(o) = 1; break;
        case 2: BA0(o) = 2; break;
        case 3: break;
        }
    }
    if (DAT_1f800282 & 0x1000) {
        switch (m & 3) {
        case 0: BA0(o) |= 0x10; break;
        case 1: BA0(o) |= 0x20; break;
        case 2: BA0(o) |= 0x30; break;
        case 3: BA0(o) |= 0x40; break;
        }
    }
}
