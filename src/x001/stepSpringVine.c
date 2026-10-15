// FUNC 8011a528 180 X001
// MATCHING 8011a528 180
#include "TOBJ.H"

extern void (*D_8013C44C[])(TObj *);
extern void springVineIdle();
extern void springVineRopeStep1();
extern void springVineRopeStep2();
extern void springVineRopeStep3();
extern void springVineRopeStep4();
static void (*D_8013C460[5])(TObj *) = {
    springVineIdle, springVineRopeStep1, springVineRopeStep2, springVineRopeStep3,
    springVineRopeStep4,
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
