// FUNC 801175a4 308 X000
// MATCHING 801175a4 308
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
    if (DAT_8009c962 == 3) {
        switch (o->step) {
        case 0:
            if (DAT_8009c93a != 0) o->step++;
            FUN_8002a3d4(o);
            break;
        case 1:
            if (DAT_1f80016e < -0xdb && DAT_1f80016a >= 0x12b) FUN_800270a0(o, 2, 1);
            else if (DAT_1f80016a >= 0x157 && DAT_1f80016e >= -0x38) FUN_800270a0(o, 2, 0);
            FUN_8002a3d4(o);
            break;
        case 2:
            FUN_8002a4d0(o);
            break;
        }
    } else {
        FUN_80116d14(o);
    }
}
