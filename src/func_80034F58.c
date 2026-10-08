// FUNC 80034f58 324 MAIN0
// MATCHING 80034f58 324
#include "TOBJ.H"
extern unsigned char D_800A60FF;
extern unsigned char D_8009D2B2[];
extern signed char D_800A611B;
void FUN_8003509c(TObj *o, int n);
void ObjCullRegister(TObj *o);
void func_80033B44(TObj *o);
void FUN_80018744(TObj *o);

void func_80034F58(TObj *o)
{
    unsigned char st;
    if (D_800A60FF != 0) {
        o->b04 = 2;
    }
    st = o->b04;
    switch (st) {
    case 0:
        if (o->state == 0) {
            o->w22 = 0;
            FUN_8003509c(o, D_8009D2B2[0] - 5);
        }
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_80033B44(o);
            break;
        case 1:
            func_80033B44(o);
            break;
        case 2:
            func_80033B44(o);
            break;
        }
        break;
    case 2:
        if (--D_800A611B < 0) {
            D_800A611B = 0;
        }
        o->b04 = 3;
        break;
    case 3:
        FUN_80018744(o);
        break;
    }
}
