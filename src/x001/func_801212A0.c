// FUNC 801212a0 304 X001
// MATCHING 801212a0 304
#include "TOBJ.H"
extern TObj *D_8009F0EC;
extern unsigned char D_8009CEAB, D_8009CDB2, D_8009CEF7, D_8009CDB8, D_8009D0BE;
extern int D_8009C984;
extern unsigned short D_8009C962;
extern void func_801374C0(int, int, int, int);
extern void func_801368D4(int, int, int);
extern void FUN_80026e0c(int, int);

void func_801212A0(TObj *o)
{
    D_8009F0EC = 0;
    if (D_8009CEAB == 3)
        func_801374C0(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    if (D_8009CDB2 != 0xff) {
        if (!(D_8009C984 & 0x100))
            goto clr;
        if (D_8009C962 == 3 || D_8009C962 == 1)
            func_801368D4(o->a.p.whole, o->y.p.whole, o->b.p.whole);
    } else {
        if (!(D_8009C984 & 0x100))
            goto clr;
        D_8009C984 &= ~0x100;
    }
    if (!(D_8009C984 & 0x100)) {
    clr:
        D_8009CEF7 = 0;
    }
    if (D_8009CDB8 == 0xff && D_8009D0BE != 0)
        FUN_80026e0c(0x1a, 1);
}
