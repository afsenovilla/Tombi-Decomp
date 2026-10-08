// FUNC 801270ac 300 X010
// MATCHING 801270ac 300
#include "TOBJ.H"
extern void func_80126F48(TObj *);
extern void func_80125E00(TObj *);
extern void func_801268D4(TObj *);
extern void func_80125FB4(TObj *);
extern void func_801264B4(TObj *);
extern int FUN_800202b4(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_80018790(TObj *);

void func_801270AC(TObj *o)
{
    unsigned char b = o->b04;

    switch (b) {
    case 0:
        func_80126F48(o);
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            func_80125E00(o);
            break;
        case 1:
            func_801268D4(o);
            break;
        case 2:
            func_80125FB4(o);
            break;
        case 3:
            func_801264B4(o);
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04 = b + 1;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
