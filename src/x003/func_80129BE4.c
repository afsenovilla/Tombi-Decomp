// FUNC 80129be4 220 X003
// MATCHING 80129be4 220
#include "TOBJ.H"
extern void func_80129A2C(TObj *);
extern void func_80129868(TObj *);
extern int FUN_800202b4(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_80018790(TObj *);

void func_80129BE4(TObj *o)
{
    unsigned char b = o->b04;

    switch (b) {
    case 0:
        func_80129A2C(o);
        break;
    case 1:
        FUN_800202b4(o);
        func_80129868(o);
        break;
    case 2:
        if ((unsigned short)o->wba == 3) {
            FUN_800202b4(o);
            AnimAdvance(o);
            if (o->visible == 0)
                o->b04 = 3;
        } else {
            o->b04 = b + 1;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
