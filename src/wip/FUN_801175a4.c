// FUNC 801175a4 308 X000
#include "TOBJ.H"
extern unsigned short DAT_8009c962;
extern unsigned char DAT_8009c93a;
extern short DAT_1f80016e, DAT_1f80016a;
extern void FUN_80116d14(TObj *);
extern void FUN_800270a0(TObj *, int, int);
extern void FUN_8002a3d4(TObj *);
extern void FUN_8002a4d0(TObj *);

void FUN_801175a4(TObj *o)
{
    int u;
    unsigned char s;
    if (DAT_8009c962 != 3) {
        FUN_80116d14(o);
        return;
    }
    s = o->step;
    if (s == 1) {
        if (DAT_1f80016e < -0xdb && DAT_1f80016a > 0x12a) {
            u = 1;
        } else {
            if (DAT_1f80016a < 0x157 || DAT_1f80016e < -0x38) goto L;
            u = 0;
        }
        FUN_800270a0(o, 2, u);
    } else {
        if (s > 1) {
            if (s != 2) return;
            FUN_8002a4d0(o);
            return;
        }
        if (s != 0) return;
        if (DAT_8009c93a != 0) o->step = 1;
    }
L:
    FUN_8002a3d4(o);
}
