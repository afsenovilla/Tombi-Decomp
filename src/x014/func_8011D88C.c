// FUNC 8011d88c 108 X014
// MATCHING 8011d88c 108
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);

void func_8011D88C(TObj *o, short n)
{
    unsigned char *p = (unsigned char *)o->d90 + n * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    o->wac = n;
    o->anim = (*(void ***)&o->wa8)[n];
    AnimLoadDuration(o);
}
