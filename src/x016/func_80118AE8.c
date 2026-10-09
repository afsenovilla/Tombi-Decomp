// FUNC 80118ae8 364 X016
// MATCHING 80118ae8 364
#include "TOBJ.H"

extern unsigned char D_8009C940, D_8009C941, D_8009D12F;
extern void func_80118974(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_801180FC(TObj *);
extern void func_80118458(TObj *);
extern void FUN_80018790(TObj *);

void func_80118AE8(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80118974(o);
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            if (D_8009C940 != 0) {
                if (D_8009C941 == 3 && D_8009D12F == 0) {
                    o->step = 2;
                    o->state = 0;
                }
                D_8009C940 = 0;
            } else if (o->b68 != 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_801180FC(o);
            break;
        case 2:
            func_80118458(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
