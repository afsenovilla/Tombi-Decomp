// FUNC 80119940 168 X011
// MATCHING 80119940 168
#include "TOBJ.H"
extern void func_8011977C(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_80119498(TObj *);
extern void FUN_80018790(TObj *);

void func_80119940(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8011977C(o);
        break;
    case 1:
        FUN_800202b4(o);
        func_80119498(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
