// FUNC 8011a528 180 X001
// MATCHING 8011a528 180
#include "TOBJ.H"

extern void (*D_8013C44C[])(TObj *);
extern void springVineIdle();
extern void func_8011AEFC();
extern void func_8011B498();
extern void func_8011B74C();
extern void func_8011BADC();
static void (*D_8013C460[5])(TObj *) = {
    springVineIdle, func_8011AEFC, func_8011B498, func_8011B74C,
    func_8011BADC,
};
extern void ObjListPush_1F800224(TObj *o);

void stepSpringVine(TObj *o)
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
