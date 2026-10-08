// FUNC 80033040 328 MAIN0
// MATCHING 80033040 328
#include "TOBJ.H"
extern unsigned char D_800A60FF;
extern unsigned char *D_8009C338;
extern unsigned char *D_8009C330;
extern signed char D_800A611B;
extern void FUN_80031664(TObj *);
extern void ObjCullRegister(TObj *);
extern void func_800317C0(TObj *);
extern void FUN_80018744(TObj *);
void func_80033040(TObj *o)
{
    if (D_800A60FF != 0)
        o->b04 = 2;
    switch (o->b04) {
    case 0:
        FUN_80031664(o);
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_800317C0(o);
            break;
        case 1:
            func_800317C0(o);
            break;
        case 2:
            func_800317C0(o);
            break;
        }
        break;
    case 2:
        D_8009C338[0xc] = 0;
        *D_8009C330 = 0;
        if (--D_800A611B < 0)
            D_800A611B = 0;
        o->b04 = 3;
        break;
    case 3:
        FUN_80018744(o);
        break;
    }
}
