// FUNC 8011a528 180 X001
// MATCHING 8011a528 180
#include "TOBJ.H"

extern void (*D_8013C44C[])(TObj *);
extern void (*D_8013C460[])(TObj *);
extern void ObjListPush_1F800224(TObj *o);

void func_8011A528(TObj *o)
{
    TObj *p;

    if (o->subtype != 0) return;
    D_8013C44C[o->step](o);
    D_8013C460[o->step]((TObj *)o->d94);
    ((TObj *)o->d94)->state = o->state;
    p = (TObj *)o->d94;
    if (p->visible == 0) {
        p->visible = 1;
        ObjListPush_1F800224((TObj *)o->d94);
    }
}
