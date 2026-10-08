// FUNC 8011709c 120 X014
// MATCHING 8011709c 120
#include "TOBJ.H"

extern TObj *ObjAlloc(void);

/* implicit int return (no value): keeps v0 live at the exit, so the delay slot stays empty */
int func_8011709C(TObj *o, unsigned char n)
{
    TObj *e = ObjAlloc();
    if (e) {
        e->active = 2;
        e->type = 0x23;
        e->subtype = n;
        e->a.raw = o->a.raw;
        e->y.raw = o->y.raw;
        e->b.raw = o->b.raw;
    }
}
