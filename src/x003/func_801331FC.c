// FUNC 801331fc 196 X003
// MATCHING 801331fc 196
#include "TOBJ.H"
extern unsigned short D_8009C962;
extern void func_80131E44(TObj *);
extern void func_80132214(TObj *);
extern void func_80131FEC(TObj *);
extern void func_80132960(TObj *);
extern void func_80132D80(TObj *);

void func_801331FC(TObj *o)
{
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
}
