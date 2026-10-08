// FUNC 80115ee4 252 X005
// MATCHING 80115ee4 252
#include "TOBJ.H"

extern unsigned char D_8009C93A;
extern short D_1F80016A, D_1F80016E;
extern void (*D_80079AD0[])(TObj *);
extern void func_80027810(TObj *);
extern void FUN_800270a0(TObj *, int, int);
extern void FUN_8002a4d0(TObj *);

void func_80115EE4(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        if (D_1F80016A < 0x14 && D_1F80016E >= -0x3b) FUN_800270a0(o, 1, 0);
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
