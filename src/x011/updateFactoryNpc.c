// FUNC 80119940 168 X011
// MATCHING 80119940 168
#include "TOBJ.H"
extern void func_8011977C(TObj *);
extern int FUN_800202b4(TObj *);
extern void updateFactoryNpc(TObj *);
extern void freeObjectLayer2(TObj *);

void updateFactoryNpc(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8011977C(o);
        break;
    case 1:
        FUN_800202b4(o);
        updateFactoryNpc(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
