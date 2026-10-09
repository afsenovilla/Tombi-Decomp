// FUNC 801332cc 380 X003
// MATCHING 801332cc 380
#include "TOBJ.H"

extern unsigned short D_8009C962;
extern unsigned char D_8009CDC1;
extern void func_801318E4(TObj *);
extern void func_80131E44(TObj *);
extern void func_80132214(TObj *);
extern void func_80131FEC(TObj *);
extern void func_80132960(TObj *);
extern void func_80132D80(TObj *);
extern void FUN_80018980(TObj *);

void func_801332CC(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_801318E4(o);
        o->b04++;
        if (D_8009C962 == 0 && D_8009CDC1 == 0) o->step = 2;
        break;
    case 1:
        if (D_8009C962 == 0) {
            switch (o->step) {
            case 0:
                func_80131E44(o);
                break;
            case 1:
                func_80132214(o);
                break;
            case 2:
                func_80131FEC(o);
                break;
            }
        } else if (D_8009C962 == 1) {
            if (o->step == 0) func_80132960(o);
        } else {
            func_80132D80(o);
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
